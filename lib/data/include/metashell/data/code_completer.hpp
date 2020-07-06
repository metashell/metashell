#ifndef METASHELL_DATA_CODE_COMPLETER_HPP
#define METASHELL_DATA_CODE_COMPLETER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/code_completion.hpp>
#include <metashell/data/user_input.hpp>

#include <functional>

namespace metashell
{
  namespace data
  {
    using code_completer = std::function<code_completion(const user_input&)>;
  }
}

#endif
