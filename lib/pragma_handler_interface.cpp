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

#include <metashell/pragma_handler_interface.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/token_iterator.hpp>

using namespace metashell;

void metashell::run(
  const pragma_handler_interface& handler_,
  const std::string& args_
)
{
  const token_iterator b = begin_tokens(args_, "<str>");
  handler_.run(b, end_of_pragma_argument_list(b));
}

