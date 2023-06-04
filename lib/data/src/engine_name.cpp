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

#include <metashell/data/engine_name.hpp>

#include <iostream>
#include <variant>

namespace metashell
{
  namespace data
  {
    engine_name parse_engine_name(const std::string_view& s_)
    {
      return s_ == "auto" ? engine_name(auto_engine_name()) :
                            engine_name(parse_real_engine_name(s_));
    }

    std::string to_string(engine_name e_)
    {
      return std::visit([](auto name_) { return to_string(name_); }, e_);
    }

    std::ostream& operator<<(std::ostream& out_, engine_name e_)
    {
      return out_ << to_string(e_);
    }

    std::string operator+(const std::string& lhs_, engine_name rhs_)
    {
      return lhs_ + to_string(rhs_);
    }

    std::string operator+(engine_name lhs_, const std::string& rhs_)
    {
      return to_string(lhs_) + rhs_;
    }
  } // namespace data
} // namespace metashell
