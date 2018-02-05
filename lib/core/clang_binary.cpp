// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/clang_binary.hpp>
#include <metashell/has_prefix.hpp>
#include <metashell/metashell.hpp>
#include <metashell/process/run.hpp>
#include <metashell/process/util.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  const std::string default_clang_search_path[] = {""
#include "default_clang_search_path.hpp"
  };

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
        using boost::adaptors::sliced;
        using boost::algorithm::join;

        METASHELL_LOG(logger_, "No Clang binary found.");

        const auto search_path_len =
            sizeof(default_clang_search_path) / sizeof(const char*);

        displayer_.show_error(
            "clang++ not found. Checked:\n" + clang_metashell.string() + "\n" +
            join(default_clang_search_path | sliced(1, search_path_len), "\n") +
            "\n");
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

  bool stdinc_allowed(const std::vector<std::string>& extra_clang_args_)
  {
    return find_if(extra_clang_args_.begin(), extra_clang_args_.end(),
                   [](const std::string& s_) {
                     return s_ == "-nostdinc" || s_ == "-nostdinc++";
                   }) == extra_clang_args_.end();
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
                                      [](const boost::filesystem::path& p_) {
                                        return p_.string();
                                      }),
                         ";"));

    return result;
  }

  std::vector<std::string>
  clang_args(bool use_internal_templight_,
             const std::vector<std::string>& extra_clang_args_,
             const boost::filesystem::path& internal_dir_,
             iface::environment_detector& env_detector_,
             logger* logger_,
             const boost::filesystem::path& clang_path_)
  {
    std::vector<std::string> args{"-iquote", ".", "-x", "c++-header"};

    if (stdinc_allowed(extra_clang_args_))
    {
      args.push_back("-I");
      args.push_back(internal_dir_.string());
    }

    if (use_internal_templight_)
    {
      args.push_back("-Wfatal-errors");

      if (env_detector_.on_windows())
      {
        args.push_back("-fno-ms-compatibility");
        args.push_back("-U_MSC_VER");
      }

      if (!cpp_standard_set(extra_clang_args_))
      {
        args.push_back("-std=c++0x");
      }

      if (!max_template_depth_set(extra_clang_args_))
      {
        args.push_back(set_max_template_depth(256));
      }

      if (stdinc_allowed(extra_clang_args_))
      {
        const std::vector<boost::filesystem::path> include_path =
            determine_include_path(clang_path_, env_detector_, logger_);
        args.reserve(args.size() + include_path.size());
        for (const boost::filesystem::path& p : include_path)
        {
          args.push_back("-I" + p.string());
        }
      }

      args.insert(
          args.end(), extra_clang_args_.begin(), extra_clang_args_.end());
    }
    else if (extra_clang_args_.size() > 1)
    {
      args.insert(
          args.end(), extra_clang_args_.begin() + 1, extra_clang_args_.end());
    }

    return args;
  }

  data::result
  preprocess(const iface::environment& env_,
             const boost::optional<data::cpp_code>& tmp_exp_,
             const boost::optional<boost::filesystem::path>& env_path_,
             clang_binary& clang_binary_)
  {
    return clang_binary_.precompile(
        env_path_ ? std::vector<std::string>{"-include", env_path_->string()} :
                    std::vector<std::string>{},
        tmp_exp_ ?
            env_.get_appended("::metashell::impl::wrap< " + *tmp_exp_ +
                              " > __metashell_v;\n") :
            env_.get());
  }

  std::vector<std::string> dump_ast() { return {"-Xclang", "-ast-dump"}; }

  std::vector<std::string> dump_templight_to_file(
      std::vector<std::string> clang_args_,
      const boost::optional<boost::filesystem::path>& templight_dump_path_)
  {
    if (templight_dump_path_)
    {
      clang_args_.push_back("-Xtemplight");
      clang_args_.push_back("-profiler");
      clang_args_.push_back("-Xtemplight");
      clang_args_.push_back("-safe-mode");

      // templight can't be forced to generate output file with
      // -Xtemplight -output=<file> for some reason
      // A workaround is to specify a standard output location with -o
      // then append ".trace.pbf" to the specified file (on the calling side)
      clang_args_.push_back("-o");
      clang_args_.push_back(templight_dump_path_->string());
    }

    return clang_args_;
  }

  data::process_output compile(const data::cpp_code& preprocessed_code_,
                               std::vector<std::string> clang_args_,
                               clang_binary& clang_binary_)
  {
    clang_args_.reserve(clang_args_.size() + 3);
    clang_args_.push_back("-c");
    clang_args_.push_back("-x");
    clang_args_.push_back("c++-cpp-output");

    return run_clang(clang_binary_, clang_args_, preprocessed_code_);
  }

  data::result
  compile(const boost::optional<data::cpp_code>& tmp_exp_,
          data::cpp_code precompiled_exp_,
          const boost::optional<boost::filesystem::path>& templight_dump_path_,
          clang_binary& clang_binary_)
  {
    const data::process_output output =
        compile(std::move(precompiled_exp_),
                dump_templight_to_file(dump_ast(), templight_dump_path_),
                clang_binary_);

    const bool success = output.exit_code == data::exit_code_t(0);

    return data::result{success,
                        success && tmp_exp_ ?
                            get_type_from_ast_string(output.standard_output) :
                            "",
                        success ? "" : output.standard_error, ""};
  }
}

clang_binary::clang_binary(boost::filesystem::path clang_path_,
                           std::vector<std::string> base_args_,
                           logger* logger_)
  : _clang_path(std::move(clang_path_)),
    _base_args(std::move(base_args_)),
    _logger(logger_)
{
  process::quote_arguments(_base_args);
}

clang_binary::clang_binary(bool use_internal_templight_,
                           boost::filesystem::path clang_path_,
                           const std::vector<std::string>& extra_clang_args_,
                           const boost::filesystem::path& internal_dir_,
                           iface::environment_detector& env_detector_,
                           logger* logger_)
  : clang_binary(std::move(clang_path_),
                 clang_args(use_internal_templight_,
                            extra_clang_args_,
                            internal_dir_,
                            env_detector_,
                            logger_,
                            clang_path_),
                 logger_)
{
}

data::process_output clang_binary::run(const std::vector<std::string>& args_,
                                       const std::string& stdin_) const
{
  std::vector<std::string> cmd(_base_args.size() + args_.size());

  process::quote_arguments(
      args_.begin(), args_.end(),
      std::copy(_base_args.begin(), _base_args.end(), cmd.begin()));

  METASHELL_LOG(_logger, "Running Clang: " + _clang_path.string() + " " +
                             boost::algorithm::join(cmd, " "));

  const data::process_output o = process::run(_clang_path, cmd, stdin_);

  METASHELL_LOG(_logger, "Clang's exit code: " + to_string(o.exit_code));
  METASHELL_LOG(_logger, "Clang's stdout: " + o.standard_output);
  METASHELL_LOG(_logger, "Clang's stderr: " + o.standard_error);

  return o;
}

data::result clang_binary::precompile(std::vector<std::string> args_,
                                      const data::cpp_code& exp_) const
{
  args_.push_back("-E");

  const data::process_output output = run_clang(*this, args_, exp_);

  const bool success = output.exit_code == data::exit_code_t(0);

  return data::result{success, success ? output.standard_output : "",
                      success ? "" : output.standard_error, ""};
}

data::process_output
metashell::run_clang(const iface::executable& clang_binary_,
                     std::vector<std::string> clang_args_,
                     const data::cpp_code& input_)
{
  clang_args_.push_back("-"); // Compile from stdin

  return clang_binary_.run(clang_args_, input_.value());
}

std::tuple<data::result, std::string>
metashell::eval_with_templight_dump_on_stdout(
    const iface::environment& env_,
    const boost::optional<data::cpp_code>& tmp_exp_,
    const boost::optional<boost::filesystem::path>& env_path_,
    clang_binary& clang_binary_)
{
  data::result precompile_result =
      preprocess(env_, tmp_exp_, env_path_, clang_binary_);

  if (precompile_result.successful)
  {
    const data::cpp_code precompiled_code(std::move(precompile_result.output));

    const data::process_output templight_output = compile(
        precompiled_code, {"-Xclang", "-templight-dump"}, clang_binary_);

    return std::make_tuple(
        templight_output.exit_code == data::exit_code_t(0) ?
            compile(tmp_exp_, precompiled_code, boost::none, clang_binary_) :
            data::result{false, "", templight_output.standard_error, ""},
        templight_output.standard_output);
  }
  else
  {
    return std::make_tuple(precompile_result, "");
  }
}

data::result metashell::eval(
    const iface::environment& env_,
    const boost::optional<data::cpp_code>& tmp_exp_,
    const boost::optional<boost::filesystem::path>& env_path_,
    const boost::optional<boost::filesystem::path>& templight_dump_path_,
    clang_binary& clang_binary_)
{
  const data::result precompile_result =
      preprocess(env_, tmp_exp_, env_path_, clang_binary_);

  return precompile_result.successful ?
             compile(tmp_exp_, data::cpp_code(precompile_result.output),
                     templight_dump_path_, clang_binary_) :
             precompile_result;
}

boost::filesystem::path
metashell::find_clang(bool use_internal_templight_,
                      const std::vector<std::string>& extra_clang_args_,
                      const std::string& metashell_binary_,
                      const std::string& engine_,
                      iface::environment_detector& env_detector_,
                      iface::displayer& displayer_,
                      logger* logger_)
{
  return use_internal_templight_ ?
             detect_clang_binary(env_detector_, displayer_, logger_) :
             extract_clang_binary(
                 extra_clang_args_, env_detector_, metashell_binary_, engine_);
}
