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

#include <metashell/disable_precompiled_header_guard.hpp>

using namespace metashell;

disable_precompiled_header_guard::disable_precompiled_header_guard(
    shell& shell_)
  : _shell(shell_), _was_using(shell_.using_precompiled_headers())
{
  if (_was_using)
  {
    _shell.using_precompiled_headers(false);
  }
}

disable_precompiled_header_guard::~disable_precompiled_header_guard()
{
  if (_was_using)
  {
    _shell.using_precompiled_headers(true);
  }
}

std::unique_ptr<disable_precompiled_header_guard>
disable_precompiled_header_guard::create(shell& shell_)
{
  return std::unique_ptr<disable_precompiled_header_guard>(
      new disable_precompiled_header_guard(shell_));
}
