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

#include "mock_cxindex.hpp"

#include <cassert>

using namespace metashell;

std::unique_ptr<iface::cxtranslationunit> mock_cxindex::parse_code(
  const data::unsaved_file&
)
{
  assert(!_parse_code_results.empty());

  std::unique_ptr<iface::cxtranslationunit>
    r = move(_parse_code_results.front());
  _parse_code_results.pop_front();

  return move(r);
}

void mock_cxindex::parse_code_returns(
  std::unique_ptr<iface::cxtranslationunit> result_
)
{
  _parse_code_results.push_back(move(result_));
}

