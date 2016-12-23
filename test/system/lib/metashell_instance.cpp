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
#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/system_test_config.hpp>

#include <boost/filesystem.hpp>

#include <stdexcept>

namespace
{
  std::vector<std::string>
  construct_cmd(const std::vector<std::string>& extra_args_,
                bool allow_user_defined_args_)
  {
    using namespace metashell::system_test;

    std::vector<std::string> cmd{
        absolute(system_test_config::metashell_binary()).string(),
        "--console=json", "--nosplash"};
    if (allow_user_defined_args_)
    {
      cmd.insert(cmd.end(), system_test_config::metashell_args().begin(),
                 system_test_config::metashell_args().end());
    }
    if (!extra_args_.empty())
    {
      if (std::find(cmd.begin(), cmd.end(), "--") == cmd.end() ||
          extra_args_.front() != "--")
      {
        cmd.insert(cmd.end(), extra_args_.begin(), extra_args_.end());
      }
      else
      {
        cmd.insert(cmd.end(), extra_args_.begin() + 1, extra_args_.end());
      }
    }

    return cmd;
  }
}

namespace metashell
{
  namespace system_test
  {
    metashell_instance::metashell_instance(
        const std::vector<std::string>& extra_args_,
        const boost::filesystem::path& cwd_,
        bool allow_user_defined_args_)
      : _process_execution(
            construct_cmd(extra_args_, allow_user_defined_args_)),
        _child(_process_execution.cmd(), cwd_),
        _lines(),
        _last_line(),
        _initial_responses(responses_until_prompt())
    {
    }

    metashell_instance::~metashell_instance()
    {
      _child.standard_input().close();
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
        throw std::runtime_error("Metashell terminated");
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
  }
}
