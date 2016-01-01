#ifndef METASHELL_DATA_EXIT_CODE_T_HPP
#define METASHELL_DATA_EXIT_CODE_T_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class exit_code_t : boost::equality_comparable<exit_code_t>
    {
    public:
      explicit exit_code_t(int value_);

      int value() const;

    private:
      int _value;
    };

    bool operator==(exit_code_t a_, exit_code_t b_);
    std::ostream& operator<<(std::ostream& out_, exit_code_t e_);
    std::string to_string(exit_code_t e_);
  }
}

#endif
