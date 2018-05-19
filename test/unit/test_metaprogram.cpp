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

#include <metashell/data/in_memory_event_data_sequence.hpp>
#include <metashell/metaprogram.hpp>

#include <metashell/event_data_sequence.hpp>

#include <gtest/gtest.h>

#include <vector>

using namespace metashell;
using namespace metashell::data;

namespace
{
  template <metaprogram_mode Mode>
  std::unique_ptr<iface::event_data_sequence> events()
  {
    return make_event_data_sequence_ptr(in_memory_event_data_sequence(
        cpp_code("some_type"), Mode,
        {event_details<event_kind::evaluation_end>{
            {type_or_code_or_error(type("the_result_type"))}}}));
  }
}

TEST(metaprogram, constructor)
{
  metaprogram mp(events<metaprogram_mode::normal>());

  ASSERT_EQ(mp.get_evaluation_result(),
            type_or_code_or_error(type("the_result_type")));

  ASSERT_TRUE(boost::get<cpp_code>(&mp.get_root_frame().node()));
  ASSERT_EQ(
      boost::get<cpp_code>(mp.get_root_frame().node()), cpp_code("some_type"));

  ASSERT_TRUE(mp.is_at_start());
  ASSERT_FALSE(mp.is_finished());

  mp.step();

  ASSERT_FALSE(mp.is_at_start());
  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram, constructor_normal_mode)
{
  metaprogram mp(events<metaprogram_mode::normal>());

  ASSERT_EQ(mp.get_mode(), metaprogram_mode::normal);
}

TEST(metaprogram, constructor_full_mode)
{
  metaprogram mp(events<metaprogram_mode::full>());

  ASSERT_EQ(mp.get_mode(), metaprogram_mode::full);
}

TEST(metaprogram, constructor_profile_mode)
{
  metaprogram mp(events<metaprogram_mode::profile>());

  ASSERT_EQ(mp.get_mode(), metaprogram_mode::profile);
}
