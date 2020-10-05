// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/status.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    bool exit_success(const status& s_)
    {
      if (auto e = mpark::get_if<proc_exit>(&s_))
      {
        return exit_success(*e);
      }
      else
      {
        return false;
      }
    }

    std::string to_string(const status& s_)
    {
      return mpark::visit([](const auto& x_) { return to_string(x_); }, s_);
    }

    std::ostream& operator<<(std::ostream& out_, const status& s_)
    {
      return out_ << to_string(s_);
    }
  } // namespace data
} // namespace metashell
