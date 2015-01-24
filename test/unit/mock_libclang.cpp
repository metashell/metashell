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

#include "mock_libclang.hpp"

#include <cassert>

using namespace metashell;

std::unique_ptr<iface::cxindex> mock_libclang::create_index(
  const iface::environment&,
  logger*
) 
{
  assert(!_create_index_results.empty());

  std::unique_ptr<iface::cxindex> r = move(_create_index_results.front());
  _create_index_results.pop_front();

  return move(r);
}

void mock_libclang::create_index_returns(
  std::unique_ptr<iface::cxindex> result_
)
{
  _create_index_results.push_back(move(result_));
}

