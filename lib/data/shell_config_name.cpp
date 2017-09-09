// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/shell_config_name.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    shell_config_name::shell_config_name(std::string s_) : _value(std::move(s_))
    {
    }

    const std::string& shell_config_name::value() const { return _value; }

    bool operator==(const shell_config_name& a_, const shell_config_name& b_)
    {
      return a_.value() == b_.value();
    }

    std::string to_string(const shell_config_name& name_)
    {
      return name_.value();
    }

    std::ostream& operator<<(std::ostream& o_, const shell_config_name& name_)
    {
      return o_ << name_.value();
    }

    std::string operator+(const std::string& a_, const shell_config_name& b_)
    {
      return a_ + b_.value();
    }

    std::string operator+(const shell_config_name& a_, const std::string& b_)
    {
      return a_.value() + b_;
    }
  }
}
