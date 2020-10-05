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

#include <metashell/data/macro_def_or_undef.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    command_line_argument clang_argument(const macro_def_or_undef& m_)
    {
      return mpark::visit(
          [](const auto& d_) { return clang_argument(d_); }, m_);
    }

    std::string to_string(const macro_def_or_undef& m_)
    {
      return mpark::visit([](const auto& d_) { return to_string(d_); }, m_);
    }

    std::ostream& operator<<(std::ostream& out_, const macro_def_or_undef& m_)
    {
      return out_ << to_string(m_);
    }
  } // namespace data
} // namespace metashell
