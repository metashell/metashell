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

#include <metashell/data/regex.hpp>

namespace metashell
{
  namespace data
  {
    regex::regex(std::string value_) : _str(std::move(value_)), _re(_str) {}

    const std::string& regex::as_string() const { return _str; }

    const std::regex& regex::as_regex() const { return _re; }

    std::string to_string(const regex& re_) { return re_.as_string(); }

    std::ostream& operator<<(std::ostream& out_, const regex& re_)
    {
      return out_ << re_.as_string();
    }
  } // namespace data
} // namespace metashell
