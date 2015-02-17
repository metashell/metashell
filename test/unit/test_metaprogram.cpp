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
    const std::vector<metaprogram::optional_edge_descriptor>& edge_stack)
{
  JUST_ASSERT(state.discovered == discovered);
  JUST_ASSERT(state.parent_edge == parent_edge);
  compare_stack_with_vector(state.edge_stack, edge_stack);
}

JUST_TEST_CASE(test_metaprogram_constructor) {
  metaprogram mp(
      false, "some_type", data::type_or_error(data::type("the_result_type")));

  JUST_ASSERT_EQUAL(
      mp.get_evaluation_result(),
      data::type_or_error(data::type("the_result_type")));

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 1u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 0u);

  JUST_ASSERT_EQUAL(
      mp.get_vertex_property(mp.get_root_vertex()).name,
      "some_type");

  assert_state_equal(mp.get_state(),
        {false},
        {boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true},
        {boost::none},
        {}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(mp.is_finished());
}

JUST_TEST_CASE(test_metaprogram_with_single_non_root_vertex) {
  metaprogram mp(
      false, "some_type", data::type_or_error(data::type("the_result_type")));

  JUST_ASSERT(!mp.is_in_full_mode());

  metaprogram::vertex_descriptor vertex_a = mp.add_vertex("A");
  metaprogram::edge_descriptor edge_root_a =
    mp.add_edge(mp.get_root_vertex(), vertex_a,
        data::instantiation_kind::template_instantiation,
        data::file_location("foo.cpp", 10, 20));

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 2u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 1u);

  JUST_ASSERT_EQUAL(mp.get_vertex_property(vertex_a).name, "A");
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a).kind,
      data::instantiation_kind::template_instantiation);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a).point_of_instantiation,
      data::file_location("foo.cpp", 10, 20));

  assert_state_equal(mp.get_state(),
        {false, false},
        {boost::none, boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, false},
        {boost::none, edge_root_a},
        {edge_root_a}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, true},
        {boost::none, edge_root_a},
        {}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(mp.is_finished());
}

JUST_TEST_CASE(test_metaprogram_with_single_non_root_vertex_parallel_edge) {
  metaprogram mp(
      false, "some_type", data::type_or_error(data::type("the_result_type")));

  JUST_ASSERT(!mp.is_in_full_mode());

  metaprogram::vertex_descriptor vertex_a = mp.add_vertex("A");
  metaprogram::edge_descriptor edge_root_a_ti =
    mp.add_edge(mp.get_root_vertex(), vertex_a,
        data::instantiation_kind::template_instantiation,
        data::file_location("bar.cpp", 20, 10));
  metaprogram::edge_descriptor edge_root_a_me =
    mp.add_edge(mp.get_root_vertex(), vertex_a,
        data::instantiation_kind::memoization,
        data::file_location("foobar.cpp", 21, 11));

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 2u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 2u);

  JUST_ASSERT_EQUAL(mp.get_vertex_property(vertex_a).name, "A");
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_ti).kind,
      data::instantiation_kind::template_instantiation);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_ti).point_of_instantiation,
      data::file_location("bar.cpp", 20, 10));
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_me).kind,
      data::instantiation_kind::memoization);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_me).point_of_instantiation,
      data::file_location("foobar.cpp", 21, 11));

  assert_state_equal(mp.get_state(),
        {false, false},
        {boost::none, boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, false},
        {boost::none, edge_root_a_ti},
        {edge_root_a_me, edge_root_a_ti}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, true},
        {boost::none, edge_root_a_me},
        {edge_root_a_me}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, true},
        {boost::none, edge_root_a_me},
        {}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(mp.is_finished());
}

JUST_TEST_CASE(test_metaprogram_step_back_with_single_non_root_vertex) {
  metaprogram mp(
      false, "some_type", data::type_or_error(data::type("the_result_type")));

  metaprogram::vertex_descriptor vertex_a = mp.add_vertex("A");
  metaprogram::edge_descriptor edge_root_a =
    mp.add_edge(mp.get_root_vertex(), vertex_a,
        data::instantiation_kind::template_instantiation,
      data::file_location("foobar.cpp", 21, 11));

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 2u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 1u);

  JUST_ASSERT_EQUAL(mp.get_vertex_property(vertex_a).name, "A");
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a).kind,
      data::instantiation_kind::template_instantiation);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a).point_of_instantiation,
      data::file_location("foobar.cpp", 21, 11));

  assert_state_equal(mp.get_state(),
        {false, false},
        {boost::none, boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, false},
        {boost::none, edge_root_a},
        {edge_root_a}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(),
        {false, false},
        {boost::none, boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());
}

JUST_TEST_CASE(
    test_metaprogram_step_back_with_single_non_root_vertex_parallel_edge)
{
  metaprogram mp(
      false, "some_type", data::type_or_error(data::type("the_result_type")));

  metaprogram::vertex_descriptor vertex_a = mp.add_vertex("A");
  metaprogram::edge_descriptor edge_root_a_ti =
    mp.add_edge(mp.get_root_vertex(), vertex_a,
        data::instantiation_kind::template_instantiation,
        data::file_location("xx.cpp", 1, 2));
  metaprogram::edge_descriptor edge_root_a_me =
    mp.add_edge(mp.get_root_vertex(), vertex_a,
        data::instantiation_kind::memoization,
        data::file_location("yy.cpp", 1, 2));

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 2u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 2u);

  JUST_ASSERT_EQUAL(mp.get_vertex_property(vertex_a).name, "A");
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_ti).kind,
      data::instantiation_kind::template_instantiation);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_ti).point_of_instantiation,
      data::file_location("xx.cpp", 1, 2));
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_me).kind,
      data::instantiation_kind::memoization);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge_root_a_me).point_of_instantiation,
      data::file_location("yy.cpp", 1, 2));

  assert_state_equal(mp.get_state(),
        {false, false},
        {boost::none, boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, false},
        {boost::none, edge_root_a_ti},
        {edge_root_a_me, edge_root_a_ti}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, true},
        {boost::none, edge_root_a_me},
        {edge_root_a_me}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(),
        {true, true},
        {boost::none, edge_root_a_me},
        {}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(),
        {true, true},
        {boost::none, edge_root_a_me},
        {edge_root_a_me}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(),
        {true, false},
        {boost::none, edge_root_a_ti},
        {edge_root_a_me, edge_root_a_ti}
    );
  JUST_ASSERT(!mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(),
        {false, false},
        {boost::none, boost::none},
        {boost::none}
    );
  JUST_ASSERT(mp.is_at_start());
  JUST_ASSERT(!mp.is_finished());
}

JUST_TEST_CASE(test_metaprogram_constructor_full_mode_true) {
  metaprogram mp(
      true, "some_type", data::type_or_error(data::type("the_result_type")));

  JUST_ASSERT(mp.is_in_full_mode());
}

JUST_TEST_CASE(test_metaprogram_constructor_full_mode_false) {
  metaprogram mp(
      false, "some_type", data::type_or_error(data::type("the_result_type")));

  JUST_ASSERT(!mp.is_in_full_mode());
}
