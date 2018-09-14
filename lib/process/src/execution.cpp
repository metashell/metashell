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

namespace
{
  const char* c_str(const std::string& s_) { return s_.c_str(); }

#ifdef _WIN32
  void clear(PROCESS_INFORMATION& pi_)
  {
    pi_.hProcess = 0;
    pi_.hThread = 0;
    pi_.dwProcessId = 0;
    pi_.dwThreadId = 0;
  }
#endif
}

namespace metashell
{
  namespace process
  {
#ifdef _WIN32
    execution::execution(const boost::filesystem::path& binary_,
                         const std::vector<std::string>& args_,
                         const boost::filesystem::path& cwd_)
    {
      const std::string cmds =
          binary_.string() + " " + boost::algorithm::join(args_, " ");
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

      clear(_process_information);

      if (CreateProcess(NULL, &cmd[0], NULL, NULL, TRUE, 0, NULL,
                        cwd_.empty() ? NULL : cwd_.string().c_str(), &si,
                        &_process_information))
      {
        CloseHandle(si.hStdInput);
        CloseHandle(si.hStdOutput);
        CloseHandle(si.hStdError);

        _standard_input.input.close();

        _standard_output.output.close();
        _standard_error.output.close();
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
    execution::execution(const boost::filesystem::path& binary_,
                         const std::vector<std::string>& args_,
                         const boost::filesystem::path& cwd_)
    {
      std::vector<const char*> cmd(args_.size() + 2, 0);
      cmd[0] = binary_.c_str();
      std::transform(args_.begin(), args_.end(), cmd.begin() + 1, ::c_str);

      pipe error_reporting;

      switch (_pid = fork())
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
          s << "Error running " << binary_ << " "
            << boost::algorithm::join(args_, " ") << ": " << strerror(err);
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
          int status;
          waitpid(_pid, &status, 0);
          throw exception(err);
        }
      }
    }
#endif

    execution::execution(execution&& e_)
      : _standard_input(std::move(e_._standard_input)),
        _standard_output(std::move(e_._standard_output)),
        _standard_error(std::move(e_._standard_error)),
#ifdef _WIN32
        _process_information(e_._process_information)
#else
        _pid(e_._pid)
#endif
    {
#ifdef _WIN32
      clear(e_._process_information);
#else
      e_._pid = 0;
#endif
    }

    execution& execution::operator=(execution&& e_)
    {
      if (this != &e_)
      {
        _standard_input = std::move(e_._standard_input);
        _standard_output = std::move(e_._standard_output);
        _standard_error = std::move(e_._standard_error);
#ifdef _WIN32
        _process_information = e_._process_information;
        clear(e_._process_information);
#else
        _pid = e_._pid;
        e_._pid = 0;
#endif
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

#ifdef _WIN32
    data::exit_code_t execution::wait()
    {
      WaitForSingleObject(_process_information.hProcess, INFINITE);

      DWORD exit_code;
      if (!GetExitCodeProcess(_process_information.hProcess, &exit_code))
      {
        throw exception("Can't get exit code");
      }
      return data::exit_code_t(exit_code);
    }
#endif

#ifndef _WIN32
    data::exit_code_t execution::wait()
    {
      int status;
      waitpid(_pid, &status, 0);
      return data::exit_code_t(WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    }
#endif
  }
}
