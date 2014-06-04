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

#include <metashell/pragma_help.hpp>
#include <metashell/shell.hpp>
#include <metashell/metashell.hpp>
#include <metashell/version.hpp>
#include "indenter.hpp"

#include <boost/foreach.hpp>

using namespace metashell;

pragma_help::pragma_help(shell& shell_) :
  _shell(shell_)
{}

pragma_handler_interface* pragma_help::clone() const
{
  return new pragma_help(_shell);
}

std::string pragma_help::arguments() const
{
  return "";
}

std::string pragma_help::description() const
{
  return "Displays a help message.";
}

void pragma_help::run(const metashell_pragma&) const
{
  indenter ind(_shell.width(), " * ");
  ind
    .raw("/*")
    .left_align("Template metaprogramming shell " + version())
    .empty_line()
    .left_align(
      "Metashell Copyright (C) 2013 Abel Sinkovics (abel@sinkovics.hu)"
    )
    .left_align(
      "This program comes with ABSOLUTELY NO WARRANTY. This is free software,"
      " and you are welcome to redistribute it under certain conditions;"
      " for details visit <http://www.gnu.org/licenses/>."
    )
    .empty_line()
    .left_align("Metashell has the following built-in pragmas:");

  typedef std::pair<std::string, pragma_handler> sp;
  BOOST_FOREACH(const sp& p, _shell.pragma_handlers())
  {
    const std::string args = p.second.arguments();
    ind
      .empty_line()
      .left_align(
        "#pragma metashell "
          + p.first + (args.empty() ? std::string() : " " + args),
        " *   "
      )
      .left_align(p.second.description(), " *     ");
  }

  ind
    .empty_line()
    .left_align(
      "To quit Metashell press Ctrl+D"
    )
    .empty_line()
    .raw(" */");

  _shell.display_normal(ind.str());
}

