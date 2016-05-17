// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/engine_clang.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/includes.hpp>
#include <metashell/exception.hpp>
#include <metashell/for_each_line.hpp>
#include <metashell/source_position.hpp>
#include <metashell/unsaved_file.hpp>
#include <metashell/metashell.hpp>
#include <metashell/clang_binary.hpp>
#include <metashell/has_prefix.hpp>
#include <metashell/cached.hpp>

#include <metashell/boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <just/lines.hpp>

#include <memory>
#include <algorithm>
#include <iterator>

using namespace metashell;

namespace
{
  const std::string default_clang_search_path[] = {""
#include "default_clang_search_path.hpp"
  };

  bool stdinc_allowed(const std::vector<std::string>& extra_clang_args_)
  {
    return find_if(extra_clang_args_.begin(), extra_clang_args_.end(),
                   [](const std::string& s_)
                   {
                     return s_ == "-nostdinc" || s_ == "-nostdinc++";
                   }) == extra_clang_args_.end();
  }

  std::string extract_clang_binary(const std::vector<std::string>& engine_args_,
                                   iface::environment_detector& env_detector_,
                                   const std::string& metashell_path_,
                                   const std::string& engine_name_)
  {
    if (engine_args_.empty())
    {
      const std::string sample_path =
          env_detector_.on_windows() ?
              "c:\\Program Files\\LLVM\\bin\\clang++.exe" :
              "/usr/bin/clang++";
      throw std::runtime_error(
          "The engine requires that you specify the path to the clang compiler"
          " after --. For example: " +
          metashell_path_ + " --engine " + engine_name_ + " -- " + sample_path +
          " -std=c++11");
    }
    else
    {
      const std::string path = engine_args_.front();
      if (env_detector_.file_exists(path))
      {
        return path;
      }
      else
      {
        throw std::runtime_error(
            "The path specified as the Clang binary to use (" + path +
            ") does not exist.");
      }
    }
  }

  template <class InputIt>
  std::string remove_double_backslashes(InputIt begin_, InputIt end_)
  {
    std::string result(end_ - begin_, 'X');
    std::string::iterator j = result.begin();
    bool wasBackslash = false;
    for (InputIt i = begin_; i != end_; ++i)
    {
      if (!wasBackslash || *i != '\\')
      {
        *j = *i;
        ++j;
      }
      wasBackslash = !wasBackslash && (*i == '\\');
    }
    result.erase(j, result.end());
    return result;
  }

  boost::filesystem::path
  templight_shipped_with_metashell(iface::environment_detector& env_detector_)
  {
    return env_detector_.directory_of_executable() /
           (env_detector_.on_windows() ? "\\templight\\templight.exe" :
                                         "/templight_metashell");
  }

  boost::filesystem::path
  detect_clang_binary(iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      logger* logger_)
  {
    METASHELL_LOG(logger_, "Searching Clang binary");

    const boost::filesystem::path clang_metashell =
        templight_shipped_with_metashell(env_detector_);

    METASHELL_LOG(logger_, "Path of Clang shipped with Metashell: " +
                               clang_metashell.string());

    if (env_detector_.file_exists(clang_metashell))
    {
      METASHELL_LOG(
          logger_, "Clang shipped with Metashell is there. Choosing that.");
      return clang_metashell;
    }
    else
    {
      METASHELL_LOG(
          logger_,
          "Clang binary shipped with Metashell is missing. Searching for"
          " another Clang binary at the following locations: " +
              boost::algorithm::join(default_clang_search_path, ", "));
      const boost::filesystem::path clang = env_detector_.search_clang_binary();

      if (clang.empty())
      {
        METASHELL_LOG(logger_, "No Clang binary found.");

        std::ostringstream s;
        s << "clang++ not found. Checked:" << std::endl
          << clang_metashell << std::endl;
        std::copy(default_clang_search_path + 1,
                  default_clang_search_path +
                      sizeof(default_clang_search_path) / sizeof(const char*),
                  std::ostream_iterator<std::string>(s, "\n"));

        displayer_.show_error(s.str());
      }
      else
      {
        METASHELL_LOG(logger_, "Clang binary found: " + clang.string());
      }

      return clang;
    }
  }

  std::string set_max_template_depth(int v_)
  {
    return "-ftemplate-depth=" + std::to_string(v_);
  }

  bool cpp_standard_set(const std::vector<std::string>& args_)
  {
    return metashell::has_prefix(args_, {"--std", "-std"});
  }

  bool max_template_depth_set(const std::vector<std::string>& args_)
  {
    return metashell::has_prefix(args_, {"-ftemplate-depth"});
  }

  data::process_output run_clang(iface::executable& clang_binary_,
                                 std::vector<std::string> clang_args_,
                                 const std::string& input_)
  {
    clang_args_.push_back("-"); // Compile from stdin

    return clang_binary_.run(clang_args_, input_);
  }

  std::pair<std::string, std::string>
  find_completion_start(const std::string& s_)
  {
    typedef std::pair<std::string, std::string> string_pair;

    const data::command cmd(s_);

    std::ostringstream o;
    data::token last_token;
    bool first = true;
    for (auto i = cmd.begin(), e = cmd.end(); i != e; ++i)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        o << last_token.value();
      }
      last_token = *i;
    }

    if (first) // no token
    {
      return string_pair("", "");
    }
    else
    {
      if (last_token.category() == data::token_category::identifier ||
          last_token.category() == data::token_category::keyword)
      {
        return string_pair(o.str(), last_token.value());
      }
      else
      {
        o << last_token.value();
        return string_pair(o.str(), "");
      }
    }
  }

  boost::optional<std::string> remove_prefix(const std::string& s_,
                                             const std::string& prefix_)
  {
    if (boost::starts_with(s_, prefix_))
    {
      return std::string(s_.begin() + prefix_.length(), s_.end());
    }
    else
    {
      return boost::none;
    }
  }

  boost::optional<std::string> parse_completion(std::string line_)
  {
    if (const auto without_completion = remove_prefix(line_, "COMPLETION: "))
    {
      if (const auto pattern = remove_prefix(*without_completion, "Pattern : "))
      {
        const auto prefix_end = pattern->find("<#");
        return std::string(pattern->begin(), prefix_end == std::string::npos ?
                                                 pattern->end() :
                                                 pattern->begin() + prefix_end);
      }
      else
      {
        return std::string(without_completion->begin(),
                           std::find(without_completion->begin(),
                                     without_completion->end(), ' '));
      }
    }
    else
    {
      return boost::none;
    }
  }

  std::vector<boost::filesystem::path>
  determine_include_path(const boost::filesystem::path& clang_binary_path_,
                         iface::environment_detector& env_detector_,
                         logger* logger_)
  {
    METASHELL_LOG(logger_, "Determining include path of Clang: " +
                               clang_binary_path_.string());

    std::vector<boost::filesystem::path> result;

    const boost::filesystem::path dir_of_executable =
        env_detector_.directory_of_executable();

    if (env_detector_.on_windows())
    {
      // mingw headers shipped with Metashell
      const boost::filesystem::path mingw_headers =
          dir_of_executable / "windows_headers";

      result.push_back(mingw_headers);
      result.push_back(mingw_headers / "mingw32");
      if (clang_binary_path_.empty() ||
          clang_binary_path_ == templight_shipped_with_metashell(env_detector_))
      {
        result.push_back(dir_of_executable / "templight" / "include");
      }
    }
    else
    {
      if (env_detector_.on_osx())
      {
        result.push_back(dir_of_executable / ".." / "include" / "metashell" /
                         "libcxx");
      }
      result.push_back(dir_of_executable / ".." / "include" / "metashell" /
                       "templight");
    }

    METASHELL_LOG(
        logger_, "Include path determined: " +
                     boost::algorithm::join(
                         result | boost::adaptors::transformed(
                                      [](const boost::filesystem::path& p_)
                                      {
                                        return p_.string();
                                      }),
                         ";"));

    return result;
  }

  template <bool UseInternalTemplight>
  std::vector<std::string>
  clang_args(const data::config& config_,
             const boost::filesystem::path& internal_dir_,
             iface::environment_detector& env_detector_,
             logger* logger_,
             const boost::filesystem::path& clang_path_)
  {
    std::vector<std::string> args{"-iquote", ".", "-x", "c++-header"};

    if (stdinc_allowed(config_.extra_clang_args))
    {
      args.push_back("-I");
      args.push_back(internal_dir_.string());
    }

    if (UseInternalTemplight)
    {
      args.push_back("-Wfatal-errors");

      if (env_detector_.on_windows())
      {
        args.push_back("-fno-ms-compatibility");
        args.push_back("-U_MSC_VER");
      }

      if (!cpp_standard_set(config_.extra_clang_args))
      {
        args.push_back("-std=c++0x");
      }

      if (!max_template_depth_set(config_.extra_clang_args))
      {
        args.push_back(set_max_template_depth(256));
      }

      if (stdinc_allowed(config_.extra_clang_args))
      {
        const std::vector<boost::filesystem::path> include_path =
            determine_include_path(clang_path_, env_detector_, logger_);
        args.reserve(args.size() + include_path.size());
        for (const boost::filesystem::path& p : include_path)
        {
          args.push_back("-I" + p.string());
        }
      }

      args.insert(args.end(), config_.extra_clang_args.begin(),
                  config_.extra_clang_args.end());
    }
    else if (config_.extra_clang_args.size() > 1)
    {
      args.insert(args.end(), config_.extra_clang_args.begin() + 1,
                  config_.extra_clang_args.end());
    }

    return args;
  }

  template <class InputIt>
  std::set<boost::filesystem::path> split_file_list(InputIt begin_,
                                                    InputIt end_)
  {
    std::set<boost::filesystem::path> result;
    bool escaped = false;
    std::string current;
    for (InputIt i = begin_; i != end_; ++i)
    {
      if (escaped)
      {
        switch (*i)
        {
        case ' ':
        case '\t':
          current += *i;
          break;
        case '\r':
        case '\n':
          if (!current.empty())
          {
            result.insert(current);
            current.clear();
          }
          break;
        default:
          current += '\\';
          current += *i;
        }
        escaped = false;
      }
      else
      {
        switch (*i)
        {
        case ' ':
        case '\r':
        case '\n':
        case '\t':
          if (!current.empty())
          {
            result.insert(current);
            current.clear();
          }
          break;
        case '\\':
          escaped = true;
          break;
        default:
          current += *i;
        }
      }
    }
    if (escaped)
    {
      current += '\\';
    }
    if (!current.empty())
    {
      result.insert(current);
    }
    return result;
  }

  template <class ForwardIt>
  ForwardIt next(ForwardIt i_, ForwardIt end_)
  {
    if (i_ != end_)
    {
      ++i_;
    }
    return i_;
  }

  template <bool UseInternalTemplight>
  class engine_clang : public iface::engine
  {
  public:
    engine_clang(const data::config& config_,
                 const boost::filesystem::path& internal_dir_,
                 const boost::filesystem::path& env_filename_,
                 iface::environment_detector& env_detector_,
                 iface::displayer& displayer_,
                 logger* logger_)
      : engine_clang(
            config_,
            internal_dir_,
            env_filename_,
            env_detector_,
            UseInternalTemplight ?
                detect_clang_binary(env_detector_, displayer_, logger_) :
                extract_clang_binary(config_.extra_clang_args,
                                     env_detector_,
                                     config_.metashell_binary,
                                     config_.engine),
            logger_)
    {
    }

    engine_clang(const data::config& config_,
                 const boost::filesystem::path& internal_dir_,
                 const boost::filesystem::path& env_filename_,
                 iface::environment_detector& env_detector_,
                 const boost::filesystem::path& clang_path_,
                 logger* logger_)
      : _clang_binary(
            clang_path_,
            clang_args<UseInternalTemplight>(
                config_, internal_dir_, env_detector_, logger_, clang_path_),
            logger_),
        _internal_dir(internal_dir_),
        _env_path(internal_dir_ / env_filename_),
        _includes([this]()
                  {
                    return this->determine_includes();
                  }),
        _logger(logger_)
    {
    }

    virtual data::result precompile(const std::string& exp_) override
    {
      const data::process_output output =
          run_clang(_clang_binary, {"-E"}, exp_);

      const bool success = output.exit_code() == data::exit_code_t(0);

      return data::result{success, success ? output.standard_output() : "",
                          success ? "" : output.standard_error(), ""};
    }

    virtual data::result
    eval(const iface::environment& env_,
         const boost::optional<std::string>& tmp_exp_,
         const boost::optional<boost::filesystem::path>& templight_dump_path_,
         bool use_precompiled_headers_) override
    {
      std::vector<std::string> clang_args{"-Xclang", "-ast-dump"};
      if (use_precompiled_headers_)
      {
        clang_args.push_back("-include");
        clang_args.push_back(_env_path.string());
      }
      if (templight_dump_path_)
      {
        clang_args.push_back("-Xtemplight");
        clang_args.push_back("-profiler");
        clang_args.push_back("-Xtemplight");
        clang_args.push_back("-safe-mode");

        // templight can't be forced to generate output file with
        // -Xtemplight -output=<file> for some reason
        // A workaround is to specify a standard output location with -o
        // then append ".trace.pbf" to the specified file (on the calling side)
        clang_args.push_back("-o");
        clang_args.push_back(templight_dump_path_->string());
      }

      const data::process_output output =
          run_clang(_clang_binary, clang_args,
                    tmp_exp_ ?
                        env_.get_appended("::metashell::impl::wrap< " +
                                          *tmp_exp_ + " > __metashell_v;\n") :
                        env_.get());

      const bool success = output.exit_code() == data::exit_code_t(0);

      return data::result{
          success, success && tmp_exp_ ?
                       get_type_from_ast_string(output.standard_output()) :
                       "",
          success ? "" : output.standard_error(), ""};
    }

    virtual data::result validate_code(const std::string& src_,
                                       const data::config& config_,
                                       const iface::environment& env_,
                                       bool use_precompiled_headers_) override
    {
      METASHELL_LOG(_logger, "Validating code " + src_);

      try
      {
        const std::string src = env_.get_appended(src_);
        std::vector<std::string> clang_args{"-fsyntax-only"};
        if (use_precompiled_headers_)
        {
          clang_args.push_back("-include");
          clang_args.push_back(_env_path.string());
        }

        const data::process_output output =
            run_clang(_clang_binary, clang_args, src);

        const bool accept = output.exit_code() == data::exit_code_t(0) &&
                            output.standard_error().empty();

        return data::result{accept, "", output.standard_error(),
                            accept && config_.verbose ? src : ""};
      }
      catch (const std::exception& e)
      {
        return data::result(false, "", e.what(), "");
      }
    }

    virtual void code_complete(const iface::environment& env_,
                               const std::string& src_,
                               std::set<std::string>& out_,
                               bool use_precompiled_headers_) override
    {
      using boost::starts_with;

      using std::pair;
      using std::string;
      using std::set;

      METASHELL_LOG(_logger, "Code completion of " + src_);

      const pair<string, string> completion_start = find_completion_start(src_);

      METASHELL_LOG(
          _logger, "Part kept for code completion: " + completion_start.first);

      const data::unsaved_file src(env_.internal_dir() / "code_complete.cpp",
                                   env_.get_appended(completion_start.first));

      generate(src);

      const source_position sp = source_position_of(src.content());

      std::vector<std::string> clang_args{"-fsyntax-only", "-Xclang",
                                          "-code-completion-at=" +
                                              src.filename().string() + ":" +
                                              to_string(sp),
                                          src.filename().string()};

      if (use_precompiled_headers_)
      {
        clang_args.push_back("-include");
        clang_args.push_back(_env_path.string());
      }

      const data::process_output o = _clang_binary.run(clang_args, "");

      METASHELL_LOG(_logger, "Exit code of clang: " + to_string(o.exit_code()));

      const std::string out = o.standard_output();
      out_.clear();
      const int prefix_len = completion_start.second.length();
      for_each_line(
          out, [&out_, &completion_start, prefix_len](const std::string& line_)
          {
            if (const boost::optional<std::string> comp =
                    parse_completion(line_))
            {
              if (starts_with(*comp, completion_start.second) &&
                  *comp != completion_start.second)
              {
                out_.insert(string(comp->begin() + prefix_len, comp->end()));
              }
            }
          });
    }

    virtual void
    generate_precompiled_header(const boost::filesystem::path& fn_) override
    {
      using boost::algorithm::trim_copy;

      const std::string filename = fn_.string();

      METASHELL_LOG(_logger, "Generating percompiled header for " + filename);

      std::vector<std::string> args{
          "-iquote", ".", "-w", "-o", filename + ".pch", filename};

      const data::process_output o = _clang_binary.run(args, "");
      const std::string err = o.standard_output() + o.standard_error();
      if (!err.empty()
          // clang displays this even when "-w" is used. This can be ignored
          &&
          trim_copy(err) !=
              "warning: precompiled header used __DATE__ or __TIME__.")
      {
        throw exception("Error precompiling header " + filename + ": " + err);
      }
    }

    virtual std::string macros(const iface::environment& env_) override
    {
      const data::process_output output =
          run_clang(_clang_binary, {"-dM", "-E"}, env_.get_all());

      if (output.exit_code() == data::exit_code_t(0))
      {
        return output.standard_output();
      }
      else
      {
        throw std::runtime_error("Error getting list of macros: " +
                                 output.standard_error());
      }
    }

    virtual std::vector<boost::filesystem::path>
    include_path(data::include_type type_) override
    {
      return type_ == data::include_type::sys ? _includes->sys :
                                                _includes->quote;
    }

    virtual std::set<boost::filesystem::path>
    files_included_by(const std::string& exp_) override
    {
      const data::process_output output =
          run_clang(_clang_binary, {"-H", "-E"}, exp_);

      const boost::xpressive::sregex included_header =
          boost::xpressive::bos >> +boost::xpressive::as_xpr('.') >> ' ';

      const just::lines::view lines(output.standard_error());

      const auto result =
          lines |
          boost::adaptors::filtered([&included_header](const std::string& s_)
                                    {
                                      return regex_search(s_, included_header);
                                    }) |
          boost::adaptors::transformed(
              [](const std::string& s_)
              {
                const auto i = std::find(s_.begin(), s_.end(), ' ');

                return i == s_.end() ?
                           boost::filesystem::path() :
                           boost::filesystem::path(
                               remove_double_backslashes(i + 1, s_.end()));
              });

      return std::set<boost::filesystem::path>(result.begin(), result.end());
    }

  private:
    clang_binary _clang_binary;
    boost::filesystem::path _internal_dir;
    boost::filesystem::path _env_path;
    cached<data::includes> _includes;
    logger* _logger;

    template <data::include_type Type, class LineView>
    std::vector<boost::filesystem::path>
    include_path_of_type(const LineView& clang_output_)
    {
      using boost::algorithm::starts_with;
      using boost::algorithm::trim_copy;

      const std::string prefix = data::include_dotdotdot<Type>();

      std::vector<boost::filesystem::path> result;
      const auto includes_begin =
          next(std::find_if(clang_output_.begin(), clang_output_.end(),
                            [&prefix](const std::string& line_)
                            {
                              return starts_with(line_, prefix);
                            }),
               clang_output_.end());

      transform(includes_begin, find_if(includes_begin, clang_output_.end(),
                                        [](const std::string& line_)
                                        {
                                          return !starts_with(line_, " ");
                                        }),
                back_inserter(result), [](const std::string& s_)
                {
                  return trim_copy(s_);
                });

      return result;
    }

    data::includes determine_includes()
    {
      const data::process_output o =
          run_clang(_clang_binary, {"-v", "-xc++", "-E"}, "");

      const std::string s = o.standard_output() + o.standard_error();
      const auto lines = just::lines::view_of(s);

      data::includes result{
          include_path_of_type<data::include_type::sys>(lines),
          include_path_of_type<data::include_type::quote>(lines)};

      result.quote.insert(
          result.quote.end(), result.sys.begin(), result.sys.end());
      return result;
    }
  };
} // anonymous namespace

engine_entry metashell::get_engine_clang_entry()
{
  return engine_entry(
      factory_for<engine_clang<false>>(),
      "<Clang binary> -std=<standard to use> [<Clang args>]",
      "Uses the Clang compiler or Templight. <Clang args> are passed to the"
      " compiler as command line-arguments. Note that Metashell requires C++11"
      " or above. If your Clang uses such a standard by default, you can omit"
      " the -std argument.");
}

engine_entry metashell::get_internal_templight_entry()
{
  return engine_entry(
      factory_for<engine_clang<true>>(), "[<Clang args>]",
      "Uses the Templight shipped with Metashell. <Clang args> are passed to"
      " the compiler as command line-arguments.");
}
