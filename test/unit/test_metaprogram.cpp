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

#include <metashell/data/in_memory_event_data_sequence.hpp>

#include <metashell/event_data_sequence.hpp>

#include "counting_event_data_sequence.hpp"

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

  std::unique_ptr<counting_event_data_sequence>
  build_counting_seq(metaprogram_mode mode_ = metaprogram_mode::normal)
  {
    const file_location loc;

    return std::unique_ptr<counting_event_data_sequence>(
        new counting_event_data_sequence(
            {event_details<event_kind::template_instantiation>{
                 {type("foo<int>"), loc, loc}, 0},
             event_details<event_kind::template_end>{{}, 10},
             event_details<event_kind::template_instantiation>{
                 {type("foo<char>"), loc, loc}, 10},
             event_details<event_kind::template_end>{{}, 20},
             event_details<event_kind::evaluation_end>{
                 {type_or_code_or_error(type("int"))}}},
            mode_));
  }
}

TEST(metaprogram, constructor)
{
  metaprogram mp(events<metaprogram_mode::normal>());

  ASSERT_EQ(mp.get_evaluation_result(),
            type_or_code_or_error(type("the_result_type")));

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

TEST(metaprogram, events_are_read_lazily)
{
  std::unique_ptr<counting_event_data_sequence> in_seq_ptr =
      build_counting_seq();
  counting_event_data_sequence& in_seq = *in_seq_ptr;

  ASSERT_EQ(0, in_seq.next_called_times());
  metaprogram mp(std::move(in_seq_ptr));

  ASSERT_EQ(0, in_seq.next_called_times());

  ASSERT_TRUE(mp.is_at_start());
  ASSERT_EQ(0, in_seq.next_called_times());

  ASSERT_FALSE(mp.is_finished());
  ASSERT_EQ(0, in_seq.next_called_times());

  ASSERT_FALSE(mp.is_empty());
  ASSERT_EQ(1, in_seq.next_called_times());

  mp.step();
  ASSERT_EQ(1, in_seq.next_called_times());

  mp.step();
  ASSERT_EQ(3, in_seq.next_called_times());

  mp.step();
  ASSERT_EQ(5, in_seq.next_called_times());
}

TEST(metaprogram, metaprogram_iterator_reads_events_lazily)
{
  std::unique_ptr<counting_event_data_sequence> in_seq_ptr =
      build_counting_seq();
  counting_event_data_sequence& in_seq = *in_seq_ptr;

  ASSERT_EQ(0, in_seq.next_called_times());
  metaprogram mp(std::move(in_seq_ptr));
  ASSERT_EQ(0, in_seq.next_called_times());

  metaprogram::iterator i = mp.begin();
  ASSERT_EQ(0, in_seq.next_called_times());

  const metaprogram::iterator e = mp.end();
  ASSERT_EQ(0, in_seq.next_called_times());

  for (unsigned int j = 1; j != 6; ++j)
  {
    ASSERT_FALSE(i == e);
    ++i;
    ASSERT_EQ(j, in_seq.next_called_times());
  }

  ASSERT_FALSE(i == e);
  ++i;
  ASSERT_EQ(5, in_seq.next_called_times());

  ASSERT_TRUE(i == e);
}

TEST(metaprogram, metaprogram_iterator_step_back_from_end_reads_everything)
{
  std::unique_ptr<counting_event_data_sequence> in_seq_ptr =
      build_counting_seq();
  counting_event_data_sequence& in_seq = *in_seq_ptr;

  ASSERT_EQ(0, in_seq.next_called_times());
  metaprogram mp(std::move(in_seq_ptr));
  ASSERT_EQ(0, in_seq.next_called_times());

  metaprogram::iterator i = mp.end();
  ASSERT_EQ(0, in_seq.next_called_times());

  --i;
  ASSERT_EQ(5, in_seq.next_called_times());
}

TEST(metaprogram, profiling_information)
{
  metaprogram mp(build_counting_seq(metaprogram_mode::profile));

  metaprogram::iterator i = mp.begin();
  ASSERT_FALSE(i == mp.end());

  {
    const data::frame* f = mpark::get_if<data::frame>(&*i);
    ASSERT_TRUE(f);
    ASSERT_TRUE(bool(f->time_taken()));
    ASSERT_EQ(20, int(*f->time_taken()));
    ASSERT_TRUE(bool(f->time_taken_ratio()));
    ASSERT_EQ(100, int(*f->time_taken_ratio() * 100));
  }

  ++i;
  ASSERT_FALSE(i == mp.end());

  {
    const data::frame* f = mpark::get_if<data::frame>(&*i);
    ASSERT_TRUE(f);
    ASSERT_TRUE(bool(f->time_taken()));
    ASSERT_EQ(10, int(*f->time_taken()));
    ASSERT_TRUE(bool(f->time_taken_ratio()));
    ASSERT_EQ(50, int(*f->time_taken_ratio() * 100));
  }

  ++i;
  ASSERT_FALSE(i == mp.end());

  ASSERT_TRUE(bool(mpark::get_if<data::pop_frame>(&*i)));

  ++i;
  ASSERT_FALSE(i == mp.end());

  {
    const data::frame* f = mpark::get_if<data::frame>(&*i);
    ASSERT_TRUE(f);
    ASSERT_TRUE(bool(f->time_taken()));
    ASSERT_EQ(10, int(*f->time_taken()));
    ASSERT_TRUE(bool(f->time_taken_ratio()));
    ASSERT_EQ(50, int(*f->time_taken_ratio() * 100));
  }
}
