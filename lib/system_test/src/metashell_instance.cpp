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

#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/metashell_terminated.hpp>
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/system_test_config.hpp>

#include <algorithm>

namespace metashell
{
  namespace system_test
  {
    namespace
    {
      std::string current_engine()
      {
        const auto& args = system_test_config::metashell_args();
        auto engine = std::find(args.begin(), args.end(), "--engine");
        if (engine != args.end())
        {
          ++engine;
          if (engine != args.end())
          {
            return to_string(*engine);
          }
        }
        return "internal";
      }

      template <class Container, class T>
      bool contains(const Container& container_, const T& item_)
      {
        return std::find(container_.begin(), container_.end(), item_) !=
               container_.end();
      }

      data::command_line_argument_list
      construct_cmd(const data::command_line_argument_list& extra_args_,
                    bool allow_user_defined_args_,
                    bool allow_standard_headers_)
      {
        data::command_line_argument_list cmd{"--console=json", "--nosplash"};
        if (allow_user_defined_args_)
        {
          cmd += system_test_config::metashell_args();
        }
        if (const auto first_extra = extra_args_.front())
        {
          if (!contains(cmd, "--") ||
              *first_extra != data::command_line_argument("--"))
          {
            cmd += extra_args_;
          }
          else
          {
            cmd.append(extra_args_.begin() + 1, extra_args_.end());
          }
        }
        if (!allow_standard_headers_ && !using_msvc())
        {
          if (!contains(cmd, "--"))
          {
            cmd.push_back("--");
          }
          if (using_wave())
          {
            cmd.push_back("--nostdinc++");
          }
          else
          {
            cmd.push_back("-nostdinc");
            cmd.push_back("-nostdinc++");
          }
        }

        return cmd;
      }
    }

    metashell_instance::metashell_instance(
        const data::command_line_argument_list& extra_args_,
        const boost::filesystem::path& cwd_,
        bool allow_user_defined_args_,
        bool allow_standard_headers_)
      : _include((allow_standard_headers_ || !using_msvc()) ?
                     nullptr :
                     std::make_unique<just::environment::override_guard>(
                         "INCLUDE", "")),
        _process_execution(construct_cmd(
            extra_args_, allow_user_defined_args_, allow_standard_headers_)),
        _child(
            data::command_line(absolute(system_test_config::metashell_binary()),
                               _process_execution.args()),
            cwd_),
        _lines(),
        _last_line(),
        _initial_responses(responses_until_prompt())
    {
    }

    metashell_instance::~metashell_instance()
    {
      close_stdin();
      _child.wait();
    }

    std::vector<json_string> metashell_instance::responses_until_prompt()
    {
      using pattern::_;

      std::vector<json_string> result;
      while (result.empty() || result.back() != prompt(_))
      {
        result.push_back(next_response());
      }

      return result;
    }

    std::vector<json_string> metashell_instance::send(const json_string& cmd_)
    {
      const std::string cmd = cmd_.get() + "\n";

      _process_execution.standard_input() += cmd;
      _child.standard_input().write(cmd);

      return responses_until_prompt();
    }

    void metashell_instance::close_stdin() { _child.standard_input().close(); }

    json_string metashell_instance::next_response()
    {
      if (_lines)
      {
        if (_last_line != _lines->end())
        {
          ++_last_line;
        }
      }
      else
      {
        _lines.reset(new just::lines::basic_view<process::input_file>(
            _child.standard_output()));
        _last_line = _lines->begin();
      }

      if (_last_line == _lines->end())
      {
        std::string err;
        read_all(std::tie(_child.standard_error(), err));
        throw metashell_terminated(_process_execution, err);
      }
      else
      {
        _process_execution.standard_output() += *_last_line;
        return json_string(*_last_line);
      }
    }

    const process_execution& metashell_instance::get_process_execution() const
    {
      return _process_execution;
    }

    std::vector<json_string>
    metashell_instance::command(const std::string& cmd_)
    {
      return send(system_test::command(cmd_));
    }

    std::vector<json_string>
    metashell_instance::code_completion(const std::string& cmd_)
    {
      return send(system_test::code_completion(cmd_));
    }

    const std::vector<json_string>&
    metashell_instance::initial_responses() const
    {
      return _initial_responses;
    }

    data::command_line_argument_list
    with_sysincludes(data::command_line_argument_list args_,
                     const std::vector<boost::filesystem::path>& paths_)
    {
      if (using_wave())
      {
        args_.append_with_prefix("-S", paths_);
      }
      else if (using_msvc())
      {
        args_.append_with_prefix("/I", paths_);
      }
      else
      {
        args_.append_with_prefix("-I", paths_);
      }
      return args_;
    }

    data::command_line_argument_list
    with_quoteincludes(data::command_line_argument_list args_,
                       const std::vector<boost::filesystem::path>& paths_)
    {
      if (using_msvc())
      {
        args_.append_with_prefix("/I", paths_);
      }
      else if (using_wave())
      {
        args_.append_with_prefix("-I", paths_);
      }
      else
      {
        for (const boost::filesystem::path& p : paths_)
        {
          args_.push_back("-iquote", p);
        }
      }
      return args_;
    }

    bool using_msvc()
    {
      const auto engine = current_engine();
      if (engine == "auto")
      {
        const auto& args = system_test_config::metashell_args();
        return std::find_if(args.begin(), args.end(),
                            [](const data::command_line_argument& arg) {
                              return find(arg, "cl.exe") != std::string::npos;
                            }) != args.end();
      }
      else
      {
        return engine == "msvc";
      }
    }

    bool using_wave() { return current_engine() == "wave"; }
  }
}
