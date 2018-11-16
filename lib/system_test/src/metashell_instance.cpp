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

#include <boost/filesystem.hpp>

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
            return *engine;
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

      std::vector<std::string>
      construct_cmd(const std::vector<std::string>& extra_args_,
                    bool allow_user_defined_args_,
                    bool allow_standard_headers_)
      {
        std::vector<std::string> cmd{"--console=json", "--nosplash"};
        if (allow_user_defined_args_)
        {
          cmd.insert(cmd.end(), system_test_config::metashell_args().begin(),
                     system_test_config::metashell_args().end());
        }
        if (!extra_args_.empty())
        {
          if (!contains(cmd, "--") || extra_args_.front() != "--")
          {
            cmd.insert(cmd.end(), extra_args_.begin(), extra_args_.end());
          }
          else
          {
            cmd.insert(cmd.end(), extra_args_.begin() + 1, extra_args_.end());
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

      void
      append_with_prefix(std::vector<std::string>& args_,
                         const std::string& prefix_,
                         const std::vector<boost::filesystem::path>& paths_)
      {
        args_.reserve(args_.size() + paths_.size());
        for (const boost::filesystem::path& p : paths_)
        {
          args_.push_back(prefix_ + p.string());
        }
      }
    }

    metashell_instance::metashell_instance(
        const std::vector<std::string>& extra_args_,
        const boost::filesystem::path& cwd_,
        bool allow_user_defined_args_,
        bool allow_standard_headers_)
      : _include((allow_standard_headers_ || !using_msvc()) ?
                     nullptr :
                     std::make_unique<just::environment::override_guard>(
                         "INCLUDE", "")),
        _process_execution(construct_cmd(
            extra_args_, allow_user_defined_args_, allow_standard_headers_)),
        _child(absolute(system_test_config::metashell_binary()),
               _process_execution.cmd(),
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

    std::vector<std::string>
    with_sysincludes(std::vector<std::string> args_,
                     const std::vector<boost::filesystem::path>& paths_)
    {
      if (using_wave())
      {
        append_with_prefix(args_, "-S", paths_);
      }
      else
      {
        append_with_prefix(args_, using_msvc() ? "/I" : "-I", paths_);
      }
      return args_;
    }

    std::vector<std::string>
    with_quoteincludes(std::vector<std::string> args_,
                       const std::vector<boost::filesystem::path>& paths_)
    {
      if (using_msvc())
      {
        append_with_prefix(args_, "/I", paths_);
      }
      else if (using_wave())
      {
        append_with_prefix(args_, "-I", paths_);
      }
      else
      {
        args_.reserve(args_.size() + paths_.size() * 2);
        for (const boost::filesystem::path& p : paths_)
        {
          args_.push_back("-iquote");
          args_.push_back(p.string());
        }
      }
      return args_;
    }

    bool using_msvc() { return current_engine() == "msvc"; }

    bool using_wave() { return current_engine() == "wave"; }
  }
}
