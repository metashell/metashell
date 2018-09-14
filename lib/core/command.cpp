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
#include <metashell/core/wave_tokeniser.hpp>

namespace metashell
{
  namespace core
  {
    data::command to_command(const data::cpp_code& code_)
    {
      std::vector<data::token> tokens;
      for (auto t = core::create_wave_tokeniser(code_, "<command>");
           t->has_further_tokens(); t->move_to_next_token())
      {
        tokens.push_back(t->current_token());
      }
      return data::command(std::move(tokens));
    }
  }
}
