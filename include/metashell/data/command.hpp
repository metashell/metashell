#ifndef METASHELL_COMMAND_HPP
#define METASHELL_COMMAND_HPP

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

#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class command
    {
    public:
      explicit command(const std::string& cmd_);

      typedef std::vector<data::token>::const_iterator iterator;

      iterator begin() const;
      iterator end() const;
    private:
      std::string _cmd;
      std::vector<data::token> _tokens;
    };

    command::iterator skip(command::iterator i_);
    command::iterator skip_whitespace(
      command::iterator begin_,
      const command::iterator& end_
    );

    std::string tokens_to_string(
      command::iterator begin_,
      const command::iterator& end_
    );
  }
}

#endif

