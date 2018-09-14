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

#include <metashell/core/command.hpp>
#include <metashell/core/type.hpp>

#include <metashell/data/token.hpp>
#include <metashell/data/token_type.hpp>

#include <algorithm>

namespace metashell
{
  namespace core
  {
    bool is_template_type(const data::type& type_)
    {
      const data::command cmd = to_command(type_);
      return std::find_if(cmd.begin(), cmd.end(), [](const data::token& t_) {
               return t_.type() == data::token_type::operator_greater ||
                      t_.type() == data::token_type::operator_less;
             }) != cmd.end();
    }
  }
}
