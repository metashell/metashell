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
#include <metashell/exception.hpp>
#include <metashell/for_each_line.hpp>
#include <metashell/source_position.hpp>
#include <metashell/unsaved_file.hpp>
#include <metashell/metashell.hpp>

#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>

#include <fstream>
#include <memory>

using namespace metashell;

namespace
{
  data::process_output run_clang(
    iface::executable& clang_binary_,
    std::vector<std::string> clang_args_,
    const std::string& input_
  )
  {
    clang_args_.push_back("-"); //Compile from stdin

    return clang_binary_.run(clang_args_, input_);
  }

  std::pair<std::string, std::string> find_completion_start(
    const std::string& s_
  )
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
      if (
        last_token.category() == data::token_category::identifier
        || last_token.category() == data::token_category::keyword
      )
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

  boost::optional<std::string> remove_prefix(
    const std::string& s_,
    const std::string& prefix_
  )
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
        return
          std::string(
            pattern->begin(),
            prefix_end == std::string::npos ?
              pattern->end() :
              pattern->begin() + prefix_end
          );
      }
      else
      {
        return
          std::string(
            without_completion->begin(),
            std::find(
              without_completion->begin(),
              without_completion->end(),
              ' '
            )
          );
      }
    }
    else
    {
      return boost::none;
    }
  }

  class engine_clang : public iface::engine
  {
  public:
    engine_clang(iface::executable& clang_binary_, logger* logger_) :
      _clang_binary(clang_binary_),
      _logger(logger_)
    {}

    virtual data::result eval_tmp_formatted(
      const iface::environment& env_,
      const std::string& tmp_exp_
    ) override
    {
      using std::string;
      using std::pair;
    
      METASHELL_LOG(
        _logger,
        "Checking if metaprogram can be evaluated without metashell::format: "
        + tmp_exp_
      );
    
      const data::result simple = eval_tmp(env_, tmp_exp_);
    
      METASHELL_LOG(
        _logger,
        !simple.successful ?
          "Errors occured during metaprogram evaluation. Displaying errors"
          " coming from the metaprogram without metashell::format" :
          "No errors occured during metaprogram evaluation. Re-evaluating it"
          " with metashell::format"
      );
    
      return
        simple.successful ?
          eval_tmp(env_, "::metashell::format<" + tmp_exp_ + ">::type") :
          simple;
    }

    virtual data::result eval_tmp(
      const iface::environment& env_,
      const std::string& tmp_exp_
    ) override
    {
      const data::process_output output = run_clang(
        _clang_binary,
        env_.clang_arguments(),
        env_.get_appended(
          "::metashell::impl::wrap< " + tmp_exp_ + " > __metashell_v;\n"));
    
      if (output.exit_code() != data::exit_code_t(0)) {
        return data::result{false, "", output.standard_error(), ""};
      }
    
      return data::result{
        true, get_type_from_ast_string(output.standard_output()), "", ""};
    }

    virtual data::result eval_environment(
      const iface::environment& env_
    ) override
    {
      const data::process_output output = run_clang(
        _clang_binary,
        env_.clang_arguments(),
        env_.get());
    
      if (output.exit_code() != data::exit_code_t(0)) {
        return data::result{false, "", output.standard_error(), ""};
      }
      return data::result{true, "", "", ""};
    }

    virtual data::result validate_code(
      const std::string& src_,
      const data::config& config_,
      const iface::environment& env_
    ) override
    {
      METASHELL_LOG(_logger, "Validating code " + src_);
    
      try
      {
        const std::string src = env_.get_appended(src_);
    
        const data::process_output
          output = run_clang(_clang_binary, env_.clang_arguments(), src);
    
        const bool accept =
          output.exit_code() == data::exit_code_t(0)
          && output.standard_error().empty();
    
        return
          data::result{
            accept,
            "",
            output.standard_error(),
            accept && config_.verbose ? src : ""
          };
      }
      catch (const std::exception& e)
      {
        return data::result(false, "", e.what(), "");
      }
    }

    virtual void code_complete(
      const iface::environment& env_,
      const std::string& src_,
      std::set<std::string>& out_
    ) override
    {
      using boost::starts_with;
    
      using std::pair;
      using std::string;
      using std::set;
    
      METASHELL_LOG(_logger, "Code completion of " + src_);
    
      const pair<string, string> completion_start = find_completion_start(src_);
    
      METASHELL_LOG(
        _logger,
        "Part kept for code completion: " + completion_start.first
      );
    
      const data::unsaved_file src(
        env_.internal_dir() + "/code_complete.cpp",
        env_.get_appended(completion_start.first)
      );
    
      generate(src);
    
      const source_position sp = source_position_of(src.content());
    
      std::vector<std::string> clang_args = env_.clang_arguments();
    
      const auto ast_dump =
        std::find(clang_args.begin(), clang_args.end(), "-ast-dump");
      if (ast_dump != clang_args.end() && ast_dump != clang_args.begin())
      {
        const auto xclang = ast_dump - 1;
        if (*xclang == "-Xclang")
        {
          clang_args.erase(xclang, ast_dump + 1);
        }
      }
    
      clang_args.push_back("-fsyntax-only");
      clang_args.push_back("-Xclang");
      clang_args.push_back(
        "-code-completion-at=" + src.filename() + ":" + to_string(sp)
      );
      clang_args.push_back(src.filename());
    
      const data::process_output o = _clang_binary.run(clang_args, "");
    
      METASHELL_LOG(_logger, "Exit code of clang: " + to_string(o.exit_code()));
    
      const std::string out = o.standard_output();
      out_.clear();
      const int prefix_len = completion_start.second.length();
      for_each_line(
        out,
        [&out_, &completion_start, prefix_len](const std::string& line_)
        {
          if (const boost::optional<std::string> comp = parse_completion(line_))
          {
            if (
              starts_with(*comp, completion_start.second)
              && *comp != completion_start.second
            )
            {
              out_.insert(string(comp->begin() + prefix_len, comp->end()));
            }
          }
        }
      );
    }  private:
    iface::executable& _clang_binary;
    logger* _logger;
  };

} // anonymous namespace

std::unique_ptr<iface::engine> metashell::create_clang_engine(
  iface::executable& clang_binary_,
  logger* logger_
)
{
  return
    std::unique_ptr<iface::engine>(new engine_clang(clang_binary_, logger_));
}

