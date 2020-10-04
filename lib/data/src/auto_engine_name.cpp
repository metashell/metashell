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

#include <metashell/data/auto_engine_name.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    bool operator==(auto_engine_name, auto_engine_name) { return true; }

    bool operator<(auto_engine_name, auto_engine_name) { return false; }

    std::string operator+(const std::string& lhs_, auto_engine_name rhs_)
    {
      return lhs_ + to_string(rhs_);
    }

    std::string operator+(auto_engine_name lhs_, const std::string& rhs_)
    {
      return to_string(lhs_) + rhs_;
    }

    std::string to_string(auto_engine_name) { return "auto"; }

    std::ostream& operator<<(std::ostream& out_, auto_engine_name name_)
    {
      return out_ << to_string(name_);
    }
  } // namespace data
} // namespace metashell
