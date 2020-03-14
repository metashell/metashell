#ifndef METASHELL_DATA_SIGNAL_HPP
#define METASHELL_DATA_SIGNAL_HPP

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

#include <iosfwd>
#include <string>

#include <signal.h>

namespace metashell
{
  namespace data
  {
#ifdef METASHELL_SIGNAL_VALUE
#error METASHELL_SIGNAL_VALUE already defined
#endif
#ifdef _WIN32
#define METASHELL_SIGNAL_VALUE(value)
#else
#define METASHELL_SIGNAL_VALUE(value) = value
#endif

    enum class signal
    {
      sighup METASHELL_SIGNAL_VALUE(SIGHUP),
      sigint METASHELL_SIGNAL_VALUE(SIGINT),
      sigquit METASHELL_SIGNAL_VALUE(SIGQUIT),
      sigill METASHELL_SIGNAL_VALUE(SIGILL),
      sigabrt METASHELL_SIGNAL_VALUE(SIGABRT),
      sigfpe METASHELL_SIGNAL_VALUE(SIGFPE),
      sigkill METASHELL_SIGNAL_VALUE(SIGKILL),
      sigsegv METASHELL_SIGNAL_VALUE(SIGSEGV),
      sigpipe METASHELL_SIGNAL_VALUE(SIGPIPE),
      sigalrm METASHELL_SIGNAL_VALUE(SIGALRM),
      sigterm METASHELL_SIGNAL_VALUE(SIGTERM),
      sigusr1 METASHELL_SIGNAL_VALUE(SIGUSR1),
      sigusr2 METASHELL_SIGNAL_VALUE(SIGUSR2),
      sigchld METASHELL_SIGNAL_VALUE(SIGCHLD),
      sigcont METASHELL_SIGNAL_VALUE(SIGCONT),
      sigstop METASHELL_SIGNAL_VALUE(SIGSTOP),
      sigtstp METASHELL_SIGNAL_VALUE(SIGTSTP),
      sigttin METASHELL_SIGNAL_VALUE(SIGTTIN),
      sigttou METASHELL_SIGNAL_VALUE(SIGTTOU)
    };

#undef METASHELL_SIGNAL_VALUE

    std::string to_string(const signal&);
    std::ostream& operator<<(std::ostream&, const signal&);
  }
}

#endif
