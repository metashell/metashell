#ifndef METASHELL_DATA_PRAGMA_NAME_HPP
#define METASHELL_DATA_PRAGMA_NAME_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/command.hpp>
#include <metashell/data/identifier.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class pragma_name : boost::totally_ordered<pragma_name>
    {
    public:
      template <size_t... Lens>
      explicit pragma_name(const char (&... parts)[Lens])
        : _tokens{make_token<token_type::identifier>(cpp_code(parts))...}
      {
      }

      pragma_name(command::iterator, const command::iterator&);

      const std::vector<identifier>& tokens() const;

      void push_back(identifier);

    private:
      std::vector<identifier> _tokens;
    };

    bool operator<(const pragma_name&, const pragma_name&);
    bool operator==(const pragma_name&, const pragma_name&);

    std::string to_string(const pragma_name&);
    std::ostream& operator<<(std::ostream&, const pragma_name&);

    std::optional<command::iterator> is_this_pragma(const pragma_name&,
                                                    command::iterator,
                                                    const command::iterator&);

    template <class Sequence>
    bool prefix_of(Sequence prefix_, const pragma_name& n_)
    {
      auto i = prefix_.begin();
      for (const auto& token : n_.tokens())
      {
        if (i == prefix_.end())
        {
          return true;
        }
        else if (*i != token)
        {
          return false;
        }
        else
        {
          ++i;
        }
      }
      return i == prefix_.end();
    }
  }
}

#endif
