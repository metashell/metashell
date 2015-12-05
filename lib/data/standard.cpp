// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/standard.hpp>

#include <ostream>
#include <stdexcept>

using namespace metashell::data;

standard metashell::data::parse_standard(const std::string& std_)
{
  if (std_ == "c++0x" || std_ == "c++11")
  {
    return standard::cpp11;
  }
  else if (std_ == "c++1y" || std_ == "c++14")
  {
    return standard::cpp14;
  }
  else if (std_ == "c++1z")
  {
    return standard::cpp1z;
  }
  else
  {
    throw std::runtime_error("Invalid standard version: " + std_);
  }
}

std::string metashell::data::clang_argument(standard std_)
{
  switch (std_)
  {
  case standard::cpp11: return "-std=c++0x";
  case standard::cpp14: return "-std=c++1y";
  case standard::cpp1z: return "-std=c++1z";
  }
  throw std::runtime_error("Invalid standard value");
}

std::ostream& metashell::data::operator<<(std::ostream& os, standard std_) {
  switch (std_)
  {
  case standard::cpp11: os << "C++11";
  case standard::cpp14: os << "C++14";
  case standard::cpp1z: os << "C++1z";
  default: os << "Unknown standard";
  }
  return os;
}

