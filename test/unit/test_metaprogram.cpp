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

#include <gtest/gtest.h>

using namespace metashell;

template <class T>
void compare_stack_with_vector(std::stack<T> stack, std::vector<T> vector)
{
  while (!stack.empty() && !vector.empty())
  {
    T x = stack.top();
    stack.pop();

    T y = vector.back();
    vector.pop_back();

    ASSERT_TRUE(x == y);
  }
  ASSERT_TRUE(stack.empty() == vector.empty());
}

void assert_state_equal(
    const metaprogram::state_t& state,
    const metaprogram::discovered_t& discovered,
    const metaprogram::parent_edge_t& parent_edge,
    const std::vector<metaprogram::optional_edge_descriptor>& edge_stack)
{
  ASSERT_TRUE(state.discovered == discovered);
  ASSERT_TRUE(state.parent_edge == parent_edge);
  compare_stack_with_vector(state.edge_stack, edge_stack);
}

TEST(metaprogram, constructor)
{
  metaprogram mp(metaprogram::mode_t::normal, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_evaluation_result(),
            data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_num_vertices(), 1u);
  ASSERT_EQ(mp.get_num_edges(), 0u);

  ASSERT_EQ(mp.get_vertex_property(mp.get_root_vertex()).type,
            data::type("some_type"));

  assert_state_equal(mp.get_state(), {false}, {boost::none}, {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(), {true}, {boost::none}, {});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram, with_single_non_root_vertex)
{
  metaprogram mp(metaprogram::mode_t::normal, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_mode(), metaprogram::mode_t::normal);

  auto vertex_a =
      mp.add_vertex(data::type("A"), data::file_location("a.hpp", 10, 20));
  auto edge_root_a =
      mp.add_edge(mp.get_root_vertex(), vertex_a,
                  data::instantiation_kind::template_instantiation,
                  data::file_location("foo.cpp", 10, 20), 10.0);

  ASSERT_EQ(mp.get_num_vertices(), 2u);
  ASSERT_EQ(mp.get_num_edges(), 1u);

  ASSERT_EQ(mp.get_vertex_property(vertex_a).type.name(), "A");
  ASSERT_EQ(mp.get_vertex_property(vertex_a).source_location,
            data::file_location("a.hpp", 10, 20));

  ASSERT_EQ(mp.get_edge_property(edge_root_a).kind,
            data::instantiation_kind::template_instantiation);
  ASSERT_EQ(mp.get_edge_property(edge_root_a).point_of_instantiation,
            data::file_location("foo.cpp", 10, 20));

  assert_state_equal(mp.get_state(), {false, false}, {boost::none, boost::none},
                     {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(
      mp.get_state(), {true, false}, {boost::none, edge_root_a}, {edge_root_a});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(
      mp.get_state(), {true, true}, {boost::none, edge_root_a}, {});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram, with_single_non_root_vertex_parallel_edge)
{
  metaprogram mp(metaprogram::mode_t::normal, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_mode(), metaprogram::mode_t::normal);

  auto vertex_a =
      mp.add_vertex(data::type("A"), data::file_location("b.hpp", 40, 50));
  auto edge_root_a_ti =
      mp.add_edge(mp.get_root_vertex(), vertex_a,
                  data::instantiation_kind::template_instantiation,
                  data::file_location("bar.cpp", 20, 10), 10.0);
  auto edge_root_a_me = mp.add_edge(
      mp.get_root_vertex(), vertex_a, data::instantiation_kind::memoization,
      data::file_location("foobar.cpp", 21, 11), 10.0);

  ASSERT_EQ(mp.get_num_vertices(), 2u);
  ASSERT_EQ(mp.get_num_edges(), 2u);

  ASSERT_EQ(mp.get_vertex_property(vertex_a).type.name(), "A");
  ASSERT_EQ(mp.get_vertex_property(vertex_a).source_location,
            data::file_location("b.hpp", 40, 50));

  ASSERT_EQ(mp.get_edge_property(edge_root_a_ti).kind,
            data::instantiation_kind::template_instantiation);
  ASSERT_EQ(mp.get_edge_property(edge_root_a_ti).point_of_instantiation,
            data::file_location("bar.cpp", 20, 10));
  ASSERT_EQ(mp.get_edge_property(edge_root_a_me).kind,
            data::instantiation_kind::memoization);
  ASSERT_EQ(mp.get_edge_property(edge_root_a_me).point_of_instantiation,
            data::file_location("foobar.cpp", 21, 11));

  assert_state_equal(mp.get_state(), {false, false}, {boost::none, boost::none},
                     {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(), {true, false},
                     {boost::none, edge_root_a_ti},
                     {edge_root_a_me, edge_root_a_ti});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(), {true, true},
                     {boost::none, edge_root_a_me}, {edge_root_a_me});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(
      mp.get_state(), {true, true}, {boost::none, edge_root_a_me}, {});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram, step_back_with_single_non_root_vertex)
{
  metaprogram mp(metaprogram::mode_t::normal, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  auto vertex_a =
      mp.add_vertex(data::type("A"), data::file_location("c.hpp", 30, 35));
  auto edge_root_a =
      mp.add_edge(mp.get_root_vertex(), vertex_a,
                  data::instantiation_kind::template_instantiation,
                  data::file_location("foobar.cpp", 21, 11), 10.0);

  ASSERT_EQ(mp.get_num_vertices(), 2u);
  ASSERT_EQ(mp.get_num_edges(), 1u);

  ASSERT_EQ(mp.get_vertex_property(vertex_a).type.name(), "A");
  ASSERT_EQ(mp.get_vertex_property(vertex_a).source_location,
            data::file_location("c.hpp", 30, 35));

  ASSERT_EQ(mp.get_edge_property(edge_root_a).kind,
            data::instantiation_kind::template_instantiation);
  ASSERT_EQ(mp.get_edge_property(edge_root_a).point_of_instantiation,
            data::file_location("foobar.cpp", 21, 11));

  assert_state_equal(mp.get_state(), {false, false}, {boost::none, boost::none},
                     {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(
      mp.get_state(), {true, false}, {boost::none, edge_root_a}, {edge_root_a});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(), {false, false}, {boost::none, boost::none},
                     {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());
}

TEST(metaprogram, step_back_with_single_non_root_vertex_parallel_edge)
{
  metaprogram mp(metaprogram::mode_t::normal, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  auto vertex_a =
      mp.add_vertex(data::type("A"), data::file_location("d.hpp", 10, 11));
  auto edge_root_a_ti =
      mp.add_edge(mp.get_root_vertex(), vertex_a,
                  data::instantiation_kind::template_instantiation,
                  data::file_location("xx.cpp", 1, 2), 10.0);
  auto edge_root_a_me = mp.add_edge(mp.get_root_vertex(), vertex_a,
                                    data::instantiation_kind::memoization,
                                    data::file_location("yy.cpp", 1, 2), 10.0);

  ASSERT_EQ(mp.get_num_vertices(), 2u);
  ASSERT_EQ(mp.get_num_edges(), 2u);

  ASSERT_EQ(mp.get_vertex_property(vertex_a).type.name(), "A");
  ASSERT_EQ(mp.get_vertex_property(vertex_a).source_location,
            data::file_location("d.hpp", 10, 11));

  ASSERT_EQ(mp.get_edge_property(edge_root_a_ti).kind,
            data::instantiation_kind::template_instantiation);
  ASSERT_EQ(mp.get_edge_property(edge_root_a_ti).point_of_instantiation,
            data::file_location("xx.cpp", 1, 2));
  ASSERT_EQ(mp.get_edge_property(edge_root_a_me).kind,
            data::instantiation_kind::memoization);
  ASSERT_EQ(mp.get_edge_property(edge_root_a_me).point_of_instantiation,
            data::file_location("yy.cpp", 1, 2));

  assert_state_equal(mp.get_state(), {false, false}, {boost::none, boost::none},
                     {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(), {true, false},
                     {boost::none, edge_root_a_ti},
                     {edge_root_a_me, edge_root_a_ti});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(mp.get_state(), {true, true},
                     {boost::none, edge_root_a_me}, {edge_root_a_me});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  assert_state_equal(
      mp.get_state(), {true, true}, {boost::none, edge_root_a_me}, {});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_TRUE(mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(), {true, true},
                     {boost::none, edge_root_a_me}, {edge_root_a_me});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(), {true, false},
                     {boost::none, edge_root_a_ti},
                     {edge_root_a_me, edge_root_a_ti});
  ASSERT_FALSE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step_back();

  assert_state_equal(mp.get_state(), {false, false}, {boost::none, boost::none},
                     {boost::none});
  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());
}

TEST(metaprogram, step_sorting_in_profile_mode)
{
  metaprogram mp(metaprogram::mode_t::profile, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  auto vertex_a =
      mp.add_vertex(data::type("A"), data::file_location("a.hpp", 0, 0));
  auto vertex_b =
      mp.add_vertex(data::type("B"), data::file_location("b.hpp", 1, 2));

  auto edge_root_a_ti =
      mp.add_edge(mp.get_root_vertex(), vertex_a,
                  data::instantiation_kind::template_instantiation,
                  data::file_location("xx.cpp", 1, 2), 0.0);
  auto edge_root_b_ti = mp.add_edge(mp.get_root_vertex(), vertex_b,
                                    data::instantiation_kind::memoization,
                                    data::file_location("yy.cpp", 1, 2), 10.0);

  mp.get_edge_property(edge_root_a_ti).time_taken = 10.0;
  mp.get_edge_property(edge_root_b_ti).time_taken = 30.0;

  mp.init_full_time_taken();

  ASSERT_EQ(3u, mp.get_num_vertices());
  ASSERT_EQ(2u, mp.get_num_edges());

  ASSERT_EQ(0.0, mp.get_edge_property(edge_root_a_ti).begin_timestamp);
  ASSERT_EQ(10.0, mp.get_edge_property(edge_root_b_ti).begin_timestamp);

  ASSERT_EQ(10.0, mp.get_edge_property(edge_root_a_ti).time_taken);
  ASSERT_EQ(30.0, mp.get_edge_property(edge_root_b_ti).time_taken);

  mp.step();

  // B should come first, since that took longer to finish

  {
    auto frame = mp.get_current_frame();

    ASSERT_TRUE(frame.is_full());
    ASSERT_TRUE(frame.is_profiled());

    ASSERT_EQ("B", frame.type().name());
    ASSERT_EQ("yy.cpp", frame.point_of_instantiation().name);
    ASSERT_EQ(30.0, frame.time_taken());
    ASSERT_EQ(0.75, frame.time_taken_ratio());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_TRUE(frame.is_full());
    ASSERT_TRUE(frame.is_profiled());

    // A should come second, since that was faster

    ASSERT_EQ("A", frame.type().name());
    ASSERT_EQ("xx.cpp", frame.point_of_instantiation().name);
    ASSERT_EQ(10.0, frame.time_taken());
    ASSERT_EQ(0.25, frame.time_taken_ratio());
  }

  mp.step();

  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram, constructor_normal_mode)
{
  metaprogram mp(metaprogram::mode_t::normal, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_mode(), metaprogram::mode_t::normal);
}

TEST(metaprogram, constructor_full_mode)
{
  metaprogram mp(metaprogram::mode_t::full, "some_type", data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_mode(), metaprogram::mode_t::full);
}

TEST(metaprogram, constructor_profile_mode)
{
  metaprogram mp(metaprogram::mode_t::profile, "some_type",
                 data::file_location{},
                 data::type_or_error(data::type("the_result_type")));

  ASSERT_EQ(mp.get_mode(), metaprogram::mode_t::profile);
}
