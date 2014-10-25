#ifndef METASHELL_READLINE_COMPLETION_FUNCTION_HPP
#define METASHELL_READLINE_COMPLETION_FUNCTION_HPP

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

// Needed for the OpenBSD build. Including this header after
// <readline/readline.h> breaks.
#include <boost/optional.hpp>

#ifdef USE_EDITLINE
#  include <editline/readline.h>
#else
#  include <readline/readline.h>
#endif

namespace metashell
{
#ifdef __APPLE__
  typedef CPPFunction readline_completion_function;
#else
  typedef rl_completion_func_t readline_completion_function;
#endif
}

#endif

