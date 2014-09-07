// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/metaprogram.hpp>

#include <just/test.hpp>

using namespace metashell;

template<class T>
void compare_stack_with_vector(
    std::stack<T> stack,
    std::vector<T> vector)
{
  while (!stack.empty() && !vector.empty()) {
    T x = stack.top();
    stack.pop();

    T y = vector.back();
    vector.pop_back();

    JUST_ASSERT(x == y);
  }
  JUST_ASSERT(stack.empty() == vector.empty());
}

void assert_state_equal(
    const metaprogram::state_t& state,
    const metaprogram::discovered_t& discovered,
    const metaprogram::parent_edge_t& parent_edge,
    const std::vector<metaprogram::vertex_descriptor>& vertex_stack,
    const std::vector<metaprogram::edge_descriptor>& edge_stack)
{
  JUST_ASSERT(state.discovered == discovered);
  JUST_ASSERT(state.parent_edge == parent_edge);
  compare_stack_with_vector(state.vertex_stack, vertex_stack);
  compare_stack_with_vector(state.edge_stack, edge_stack);
}

JUST_TEST_CASE(test_metaprogram_default_constuctor) {
  metaprogram mp;

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 1u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 0u);

  JUST_ASSERT_EQUAL(
      mp.get_vertex_property(mp.get_root_vertex()).name,
      "<root>");

  assert_state_equal(mp.get_state(),
        {false},
        {boost::none},
        {mp.get_root_vertex()},
        {}
    );
}

JUST_TEST_CASE(test_metaprogram_create_empty_finished) {
  metaprogram mp = metaprogram::create_empty_finished();

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 1u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 0u);

  JUST_ASSERT_EQUAL(
      mp.get_vertex_property(mp.get_root_vertex()).name,
      "<root>");

  assert_state_equal(mp.get_state(),
        {true},
        {boost::none},
        {},
        {}
    );
}
