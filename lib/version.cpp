// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/version.hpp>

#include "cxstring.hpp"

#include <clang-c/Index.h>

#include <boost/wave/cpp_context.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>
#include <boost/preprocessor/stringize.hpp>

#ifdef USE_EDITLINE
#  include <editline/readline.h>
#else
#  include <readline/readline.h>
#endif

using namespace metashell;

namespace
{
  template <char C>
  std::string remove(const std::string& s_)
  {
    std::ostringstream s;
    for (std::string::const_iterator i = s_.begin(), e = s_.end(); i != e; ++i)
    {
      if (*i != C)
      {
        s << *i;
      }
    }
    return s.str();
  }
}

std::string metashell::libclang_version()
{
  return cxstring(clang_getClangVersion());
}

std::string metashell::wave_version()
{
  return
    remove<'"'>(
      boost::wave::context<
        const char*,
        boost::wave::cpplexer::lex_iterator<boost::wave::cpplexer::lex_token<> >
      >::get_version_string()
    );
}

std::string metashell::readline_version()
{
#ifdef USE_EDITLINE
  return " ";
#else
  return
    BOOST_PP_STRINGIZE(RL_VERSION_MAJOR)
    "." BOOST_PP_STRINGIZE(RL_VERSION_MINOR);
#endif
}

std::string metashell::version()
{
#ifndef METASHELL_MAJOR
  #error METASHELL_MAJOR not defined
#endif

#ifndef METASHELL_MINOR
  #error METASHELL_MINOR not defined
#endif

#ifndef METASHELL_PATCH
  #error METASHELL_PATCH not defined
#endif

  return
    BOOST_PP_STRINGIZE(METASHELL_MAJOR)
    "." BOOST_PP_STRINGIZE(METASHELL_MINOR)
    "." BOOST_PP_STRINGIZE(METASHELL_PATCH);
}


