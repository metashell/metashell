#ifndef METASHELL_IFACE_TOKENISER_HPP
#define METASHELL_IFACE_TOKENISER_HPP

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

#include <metashell/data/token.hpp>

namespace metashell
{
  namespace iface
  {
    class tokeniser
    {
    public:
      typedef data::token token_type;

      virtual ~tokeniser() {}

      virtual bool has_further_tokens() const = 0;
      virtual data::token current_token() const = 0;
      virtual void move_to_next_token() = 0;

      // The error flag is never cleared once it was set
      virtual bool was_error() const = 0;
    };
  }
}

#endif
