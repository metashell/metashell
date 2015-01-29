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
#include "mock_cxindex.hpp"
#include "mock_cxtranslationunit.hpp"

#include <metashell/null_cxtranslationunit.hpp>

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

void metashell::expect_parsing_return_empty(
  mock_libclang& libclang_,
  int number_of_expected_parsings_with_same_index_
)
{
  assert(number_of_expected_parsings_with_same_index_ >= 0);

  std::unique_ptr<mock_cxindex> ind(new mock_cxindex);
  for (int i = 0; i != number_of_expected_parsings_with_same_index_; ++i)
  {
    ind->parse_code_returns(
      std::unique_ptr<iface::cxtranslationunit>(new null_cxtranslationunit)
    );
  }
  libclang_.create_index_returns(move(ind));
}

void metashell::expect_parsing_fails(mock_libclang& libclang_)
{
  std::unique_ptr<mock_cxindex> ind(new mock_cxindex);

  std::unique_ptr<mock_cxtranslationunit> tu(new mock_cxtranslationunit);
  tu->add_error("test error");

  ind->parse_code_returns(move(tu));
  libclang_.create_index_returns(move(ind));
}

