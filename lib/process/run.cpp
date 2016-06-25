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
#include <metashell/process/run.hpp>

#include "pipe.hpp"

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

#ifndef _WIN32
  std::string exit_reason(int status_)
  {
    if (WIFSIGNALED(status_))
    {
      std::ostringstream s;
      s << "\nProcess died due to unhandled signal " << WTERMSIG(status_)
        << ".";
      return s.str();
    }
    else if (WCOREDUMP(status_))
    {
      return "\nProcess core dumped.";
    }
    else if (WIFSTOPPED(status_))
    {
      std::ostringstream s;
      s << "\nProcess was stopped with signal " << WSTOPSIG(status_) << ".";
      return s.str();
    }
    else
    {
      return "";
    }
  }
#endif

#ifdef _WIN32
  template <class F>
  static DWORD WINAPI start_on_background_thread_cb(LPVOID instance_)
  {
    F* f = reinterpret_cast<F*>(instance_);
    try
    {
      (*f)();
    }
    catch (...)
    {
      // ignore
    }
    delete f;
    return 0;
  }
#endif

#ifdef _WIN32
  template <class F>
  HANDLE start_on_background_thread(F f_)
  {
    return CreateThread(
        NULL, 1, &start_on_background_thread_cb<F>, new F(f_), 0, NULL);
  }
#endif
}

namespace metashell
{
  namespace process
  {
#ifdef _WIN32
    data::process_output run(const std::vector<std::string>& cmd_,
                             const std::string& input_,
                             const boost::filesystem::path& cwd_)
    {
      const std::string cmds = boost::algorithm::join(cmd_, " ");
      std::vector<char> cmd(cmds.begin(), cmds.end());
      cmd.push_back(0);

      pipe standard_input;
      pipe standard_output;
      pipe standard_error;

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
      si.hStdInput = standard_input.input.copy_handle();
      si.hStdOutput = standard_output.output.copy_handle();
      si.hStdError = standard_error.output.copy_handle();

      PROCESS_INFORMATION pi;
      pi.hProcess = 0;
      pi.hThread = 0;
      pi.dwProcessId = 0;
      pi.dwThreadId = 0;

      if (CreateProcess(NULL, &cmd[0], NULL, NULL, TRUE, 0, NULL,
                        cwd_.empty() ? NULL : cwd_.string().c_str(), &si, &pi))
      {
        CloseHandle(si.hStdInput);
        CloseHandle(si.hStdOutput);
        CloseHandle(si.hStdError);

        standard_input.input.close();
        standard_input.output.write(input_);
        standard_input.output.close();

        standard_output.output.close();
        standard_error.output.close();

        std::string std_err;
        const HANDLE h_std_err =
            start_on_background_thread([&standard_error, &std_err]() {
              std_err = standard_error.input.read();
            });

        const std::string std_out = standard_output.input.read();

        WaitForSingleObject(h_std_err, INFINITE);
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exit_code;
        if (!GetExitCodeProcess(pi.hProcess, &exit_code))
        {
          throw exception("Can't get exit code");
        }
        return data::process_output(
            data::exit_code_t(exit_code), std_out, std_err);
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
    data::process_output run(const std::vector<std::string>& cmd_,
                             const std::string& input_,
                             const boost::filesystem::path& cwd_)
    {
      using std::vector;
      using std::transform;
      using std::string;

      assert(!cmd_.empty());

      vector<const char*> cmd(cmd_.size() + 1, 0);
      transform(cmd_.begin(), cmd_.end(), cmd.begin(), ::c_str);

      pipe standard_input;
      pipe standard_output;
      pipe standard_error;
      pipe error_reporting;

      const pid_t pid = fork();
      switch (pid)
      {
      case -1:
        return data::process_output(data::exit_code_t(-1), "", "");
      case 0: // in child
        if (cwd_.empty() || chdir(cwd_.c_str()) == 0)
        {
          standard_input.output.close();
          standard_output.input.close();
          standard_error.input.close();
          error_reporting.input.close();
          error_reporting.output.close_on_exec();

          standard_input.input.use_as(STDIN_FILENO);
          standard_output.output.use_as(STDOUT_FILENO);
          standard_error.output.use_as(STDERR_FILENO);

          execv(cmd[0], const_cast<char* const*>(&cmd[0]));
        }
        {
          const int err = errno;
          std::ostringstream s;
          s << "Error running" << boost::algorithm::join(cmd_, " ") << ": "
            << strerror(err);
          error_reporting.output.write(s.str());
        }
        _exit(0);
      default: // in parent
        standard_input.input.close();
        standard_input.output.write(input_);
        standard_input.output.close();

        standard_output.output.close();
        standard_error.output.close();

        error_reporting.output.close();

        // TODO
        // pipe's buffer is (1 << 16), so we need to read from it multiple
        // times while the process runs.
        // This is hackish, since we don't care about stderr
        std::string standard_output_string;
        while (!standard_output.input.eof())
        {
          standard_output_string += standard_output.input.read();
        }

        int status;
        waitpid(pid, &status, 0);

        const std::string err = error_reporting.input.read();
        if (err.empty())
        {
          return data::process_output(
              data::exit_code_t(WIFEXITED(status) ? WEXITSTATUS(status) : -1),
              standard_output_string,
              standard_error.input.read() + exit_reason(status));
        }
        else
        {
          throw exception(err);
        }
      }
    }
#endif
  }
}
