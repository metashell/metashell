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

#include <metashell/path_builder.hpp>

using namespace metashell;

path_builder::path_builder(const std::string& s_) : _path(s_) {}

path_builder metashell::operator/(
  const path_builder& a_,
  const std::string& b_
)
{
#ifdef _WIN32
  const char sep = '\\';
#else
  const char sep = '/';
#endif
  const std::string a(a_);
  return path_builder(a.empty() ? b_ : a + sep + b_);
}

path_builder::operator std::string() const
{
  return _path;
}

