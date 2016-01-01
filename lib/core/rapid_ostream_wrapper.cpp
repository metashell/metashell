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

#include <metashell/rapid_ostream_wrapper.hpp>

#include <ostream>
#include <cassert>

using namespace metashell;

rapid_ostream_wrapper::rapid_ostream_wrapper(std::ostream& os_) : _os(os_) {}

void rapid_ostream_wrapper::Put(Ch c_) { _os.put(c_); }

void rapid_ostream_wrapper::Flush() { _os.flush(); }

rapid_ostream_wrapper::Ch rapid_ostream_wrapper::Peek() const
{
  assert(false);
  return '\0';
}

rapid_ostream_wrapper::Ch rapid_ostream_wrapper::Take()
{
  assert(false);
  return '\0';
}

size_t rapid_ostream_wrapper::Tell() const
{
  assert(false);
  return 0;
}

rapid_ostream_wrapper::Ch* rapid_ostream_wrapper::PutBegin()
{
  assert(false);
  return nullptr;
}

size_t rapid_ostream_wrapper::PutEnd(Ch*)
{
  assert(false);
  return 0;
}

void rapid_ostream_wrapper::new_line() { _os << std::endl; }
