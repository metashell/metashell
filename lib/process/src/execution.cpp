// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/process/exception.hpp>
#include <metashell/process/execution.hpp>

#include "file_util.hpp"

#include <boost/algorithm/string/join.hpp>

#ifndef _WIN32
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include <algorithm>
#include <sstream>

namespace metashell
{
  namespace process
  {
    namespace
    {
#ifdef _WIN32
      PROCESS_INFORMATION no_process()
      {
        PROCESS_INFORMATION result;
        result.hProcess = 0;
        result.hThread = 0;
        result.dwProcessId = 0;
        result.dwThreadId = 0;
        return result;
      }
#endif

#ifndef _WIN32
      pid_t no_process() { return 0; }
#endif

#ifdef _WIN32
      data::status wait_process(const PROCESS_INFORMATION& process_information_)
      {
        WaitForSingleObject(process_information_.hProcess, INFINITE);

        DWORD exit_code;
        if (!GetExitCodeProcess(process_information_.hProcess, &exit_code))
        {
          throw exception("Can't get exit code");
        }
        return data::proc_exit{data::exit_status{exit_code}};
      }
#endif

#ifndef _WIN32
      data::status wait_process(pid_t pid_)
      {
        int status;
        if (waitpid(pid_, &status, 0) == -1)
        {
          const int err = errno;
          throw exception{"Error waiting for process " + std::to_string(pid_) +
                          ": " + std::string(strerror(err))};
        }

        if (WIFEXITED(status))
        {
          return data::proc_exit{data::exit_status{
              static_cast<unsigned long>(WEXITSTATUS(status))}};
        }

        if (WIFSIGNALED(status))
        {
          return data::proc_termsig{data::signal(WTERMSIG(status)),
                                    static_cast<bool>(WCOREDUMP(status))};
        }

        if (WIFSTOPPED(status))
        {
          return data::proc_stopsig{data::signal(WSTOPSIG(status))};
        }

        throw exception{"Invalid status " + std::to_string(status)};
      }
#endif
    } // namespace

#ifdef _WIN32
    execution::execution(const data::command_line& cmd_,
                         const boost::filesystem::path& cwd_)
      : _process(no_process())
    {
      const std::string cmds = to_string(cmd_);
      std::vector<char> cmd(cmds.begin(), cmds.end());
      cmd.push_back(0);

      STARTUPINFO si;
      si.cb = sizeof(si);
      si.lpReserved = NULL;
      si.lpDesktop = NULL;
      si.lpTitle = NULL;
      si.dwX = 0;
      si.dwY = 0;
      si.dwXSize = 0;
      si.dwYSize = 0;
      si.dwXCountChars = 0;
      si.dwYCountChars = 0;
      si.dwFillAttribute = 0;
      si.dwFlags = STARTF_USESTDHANDLES;
      si.wShowWindow = 0;
      si.cbReserved2 = 0;
      si.lpReserved2 = NULL;
      si.hStdInput = copy_handle(_standard_input.input);
      si.hStdOutput = copy_handle(_standard_output.output);
      si.hStdError = copy_handle(_standard_error.output);

      PROCESS_INFORMATION process_information;
      if (CreateProcess(NULL, &cmd[0], NULL, NULL, TRUE, 0, NULL,
                        cwd_.empty() ? NULL : cwd_.string().c_str(), &si,
                        &process_information))
      {
        CloseHandle(si.hStdInput);
        CloseHandle(si.hStdOutput);
        CloseHandle(si.hStdError);

        _standard_input.input.close();

        _standard_output.output.close();
        _standard_error.output.close();

        _process = process_information;
      }
      else
      {
        const DWORD e = GetLastError();
        std::ostringstream s;
        s << "Error " << e;
        {
          char buff[1024];
          if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, e, 0, buff,
                            sizeof(buff), 0))
          {
            s << ": " << buff;
          }
        }
        throw exception(s.str());
      }
    }
#endif

#ifndef _WIN32
    execution::execution(const data::command_line& cmd_,
                         const boost::filesystem::path& cwd_)
    {
      const std::vector<const char*> cmd = cmd_.argv();

      pipe error_reporting;

      const pid_t pid = fork();
      _process = pid;
      switch (pid)
      {
      case -1:
        throw exception("Failed to fork");
      case 0: // in child
        if (cwd_.empty() || chdir(cwd_.c_str()) == 0)
        {
          _standard_input.output.close();
          _standard_output.input.close();
          _standard_error.input.close();
          error_reporting.input.close();
          close_on_exec(error_reporting.output);

          use_as(_standard_input.input, STDIN_FILENO);
          use_as(_standard_output.output, STDOUT_FILENO);
          use_as(_standard_error.output, STDERR_FILENO);

          execv(cmd[0], const_cast<char* const*>(&cmd[0]));
        }
        {
          const int err = errno;
          std::ostringstream s;
          s << "Error running " << to_string(cmd_) << ": " << strerror(err);
          error_reporting.output.write(s.str());
        }
        _exit(0);
      default: // in parent
        _standard_input.input.close();

        _standard_output.output.close();
        _standard_error.output.close();

        error_reporting.output.close();

        std::string err;
        read_all(std::tie(error_reporting.input, err));

        if (!err.empty())
        {
          wait();
          throw exception(err);
        }
      }
    }
#endif

    execution::execution(execution&& e_)
      : _standard_input(std::move(e_._standard_input)),
        _standard_output(std::move(e_._standard_output)),
        _standard_error(std::move(e_._standard_error)),
        _process(e_._process)
    {
      e_._process = no_process();
    }

    execution& execution::operator=(execution&& e_)
    {
      if (this != &e_)
      {
        _standard_input = std::move(e_._standard_input);
        _standard_output = std::move(e_._standard_output);
        _standard_error = std::move(e_._standard_error);
        _process = e_._process;
        e_._process = no_process();
      }
      return *this;
    }

    execution::~execution()
    {
      // Defined only to make sure code using the execution class does not
      // need the destructor of execution_data
    }

    output_file& execution::standard_input() { return _standard_input.output; }

    input_file& execution::standard_output() { return _standard_output.input; }

    input_file& execution::standard_error() { return _standard_error.input; }

    data::status execution::wait()
    {
      if (auto* proc = mpark::get_if<running_process>(&_process))
      {
        _process = wait_process(*proc);
      }

      return mpark::get<data::status>(_process);
    }
  } // namespace process
} // namespace metashell
