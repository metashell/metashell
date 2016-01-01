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

#include <metashell/data/logging_mode.hpp>

#include <ostream>

using namespace metashell::data;

std::ostream& metashell::data::operator<<(std::ostream& out_, logging_mode m_)
{
  switch (m_)
  {
  case logging_mode::none:
    return out_ << "none";
  case logging_mode::console:
    return out_ << "console";
  case logging_mode::file:
    return out_ << "file";
  }
  return out_;
}
