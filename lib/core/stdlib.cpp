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

#include <metashell/stdlib.hpp>

#include <ostream>
#include <stdexcept>

using namespace metashell;

stdlib metashell::parse_stdlib(const std::string& std_)
{
  if (std_ == "libc++")
  {
    return stdlib::libcxx;
  }
  else if (std_ == "libstdc++")
  {
    return stdlib::libstdcxx;
  }
  else
  {
    throw std::runtime_error("Invalid stdlib: " + std_);
  }
}

std::string metashell::clang_argument(stdlib std_)
{
  switch (std_)
  {
  case stdlib::libstdcxx: return "-stdlib=libstdc++";
  case stdlib::libcxx: return "-stdlib=libc++";
  }
  throw std::runtime_error("Invalid stdlib value");
}

std::ostream& metashell::operator<<(std::ostream& os, stdlib std_) {
  switch (std_)
  {
  case stdlib::libstdcxx: os << "libstdc++";
  case stdlib::libcxx: os << "libc++";
  default: os << "Unknown stdlib";
  }
  return os;
}

