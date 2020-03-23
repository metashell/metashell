// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/signal.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    std::string to_string(const signal& s_)
    {
      switch (s_)
      {
      case signal::sighup:
        return "SIGHUP";
      case signal::sigint:
        return "SIGINT";
      case signal::sigquit:
        return "SIGQUIT";
      case signal::sigill:
        return "SIGILL";
      case signal::sigabrt:
        return "SIGABRT";
      case signal::sigfpe:
        return "SIGFPE";
      case signal::sigkill:
        return "SIGKILL";
      case signal::sigsegv:
        return "SIGSEGV";
      case signal::sigpipe:
        return "SIGPIPE";
      case signal::sigalrm:
        return "SIGALRM";
      case signal::sigterm:
        return "SIGTERM";
      case signal::sigusr1:
        return "SIGUSR1";
      case signal::sigusr2:
        return "SIGUSR2";
      case signal::sigchld:
        return "SIGCHLD";
      case signal::sigcont:
        return "SIGCONT";
      case signal::sigstop:
        return "SIGSTOP";
      case signal::sigtstp:
        return "SIGTSTP";
      case signal::sigttin:
        return "SIGTTIN";
      case signal::sigttou:
        return "SIGTTOU";
      }
      return "";
    }

    std::ostream& operator<<(std::ostream& out_, const signal& s_)
    {
      return out_ << to_string(s_);
    }
  }
}
