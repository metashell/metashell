// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/metaprogram_builder.hpp>

#include <gtest/gtest.h>

using namespace metashell;

namespace
{
  template <class F>
  void assert_throw(const F& f_, const std::string& err_)
  {
    try
    {
      f_();
      ASSERT_TRUE(false);
    }
    catch (const std::exception& e)
    {
      ASSERT_EQ(err_, e.what());
    }
  }
}

TEST(metaprogram_builder, normal_mode)
{
  const std::vector<data::event_data> events{
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<A>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 100.0},
      data::event_details<data::event_kind::template_end>{110.0},
      data::event_details<data::event_kind::evaluation_end>{
          data::type("eval_result")}};

  data::metaprogram mp =
      metaprogram_builder(events, data::metaprogram::mode_t::normal,
                          data::cpp_code("root_name"))
          .get_metaprogram();

  ASSERT_EQ(data::metaprogram::mode_t::normal, mp.get_mode());
  ASSERT_EQ(2u, mp.get_num_vertices());
  ASSERT_EQ(1u, mp.get_num_edges());

  ASSERT_TRUE(mp.is_at_start());

  mp.step();

  auto frame = mp.get_current_frame();

  ASSERT_TRUE(frame.is_full());
  ASSERT_FALSE(frame.is_profiled());
  ASSERT_TRUE(boost::get<data::type>(&frame.node()));
  ASSERT_EQ("type<A>", boost::get<data::type>(frame.node()).name());
  ASSERT_EQ(data::event_kind::template_instantiation, frame.kind());

  mp.step();

  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram_builder, full_mode)
{
  const std::vector<data::event_data> events{
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<A>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 100.0},
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<B>"), data::file_location("file", 20, 20),
          data::file_location("file_sl", 15, 25), 110.0},
      data::event_details<data::event_kind::template_end>{120.0},
      data::event_details<data::event_kind::template_end>{130.0},
      data::event_details<data::event_kind::memoization>{
          data::type("type<A>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 140.0},
      data::event_details<data::event_kind::template_end>{150.0},
      data::event_details<data::event_kind::evaluation_end>{
          data::type("eval_result")}};

  data::metaprogram mp =
      metaprogram_builder(
          events, data::metaprogram::mode_t::full, data::cpp_code("root_name"))
          .get_metaprogram();

  ASSERT_EQ(data::metaprogram::mode_t::full, mp.get_mode());
  ASSERT_EQ(3u, mp.get_num_vertices());
  ASSERT_EQ(3u, mp.get_num_edges());

  ASSERT_TRUE(mp.is_at_start());

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_FALSE(frame.is_full());
    ASSERT_FALSE(frame.is_profiled());
    ASSERT_TRUE(boost::get<data::type>(&frame.node()));
    ASSERT_EQ("type<A>", boost::get<data::type>(frame.node()).name());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_FALSE(frame.is_full());
    ASSERT_FALSE(frame.is_profiled());
    ASSERT_TRUE(boost::get<data::type>(&frame.node()));
    ASSERT_EQ("type<B>", boost::get<data::type>(frame.node()).name());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_FALSE(frame.is_full());
    ASSERT_FALSE(frame.is_profiled());
    ASSERT_TRUE(boost::get<data::type>(&frame.node()));
    ASSERT_EQ("type<A>", boost::get<data::type>(frame.node()).name());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_FALSE(frame.is_full());
    ASSERT_FALSE(frame.is_profiled());
    ASSERT_TRUE(boost::get<data::type>(&frame.node()));
    ASSERT_EQ("type<B>", boost::get<data::type>(frame.node()).name());
  }

  mp.step();

  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram_builder, profile_mode)
{
  const std::vector<data::event_data> events{
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<A>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 100.0},
      data::event_details<data::event_kind::template_end>{110.0},
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<B>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 120.0},
      data::event_details<data::event_kind::template_end>{140.0},
      data::event_details<data::event_kind::evaluation_end>{
          data::type("eval_result")}};

  data::metaprogram mp =
      metaprogram_builder(events, data::metaprogram::mode_t::profile,
                          data::cpp_code("root_name"))
          .get_metaprogram();

  mp.init_full_time_taken();

  ASSERT_EQ(data::metaprogram::mode_t::profile, mp.get_mode());
  ASSERT_EQ(3u, mp.get_num_vertices());
  ASSERT_EQ(2u, mp.get_num_edges());

  ASSERT_TRUE(mp.is_at_start());

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_TRUE(frame.is_full());
    ASSERT_TRUE(frame.is_profiled());
    ASSERT_TRUE(boost::get<data::type>(&frame.node()));
    ASSERT_EQ("type<B>", boost::get<data::type>(frame.node()).name());
    ASSERT_EQ(data::event_kind::template_instantiation, frame.kind());
    ASSERT_EQ(20.0, frame.time_taken());
    ASSERT_EQ(0.5, frame.time_taken_ratio());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    ASSERT_TRUE(frame.is_full());
    ASSERT_TRUE(frame.is_profiled());
    ASSERT_TRUE(boost::get<data::type>(&frame.node()));
    ASSERT_EQ("type<A>", boost::get<data::type>(frame.node()).name());
    ASSERT_EQ(data::event_kind::template_instantiation, frame.kind());
    ASSERT_EQ(10.0, frame.time_taken());
    ASSERT_EQ(0.25, frame.time_taken_ratio());
  }

  mp.step();

  ASSERT_TRUE(mp.is_finished());
}

TEST(metaprogram_builder, too_much_end_events_1)
{
  const std::vector<data::event_data> events{
      data::event_details<data::event_kind::template_end>{100.0}};

  assert_throw(
      [&events] {
        metaprogram_builder(events, data::metaprogram::mode_t::normal,
                            data::cpp_code("root_name"))
            .get_metaprogram();

      },
      "Mismatched begin and TemplateEnd events");
}

TEST(metaprogram_builder, too_much_end_events_2)
{
  const std::vector<data::event_data> events{
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<A>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 100.0},
      data::event_details<data::event_kind::template_end>{110.0},

      data::event_details<data::event_kind::template_end>{120.0}};

  assert_throw(
      [&events] {
        metaprogram_builder(events, data::metaprogram::mode_t::normal,
                            data::cpp_code("root_name"))
            .get_metaprogram();

      },
      "Mismatched begin and TemplateEnd events");
}

TEST(metaprogram_builder, too_few_end_events)
{
  const std::vector<data::event_data> events{
      data::event_details<data::event_kind::template_instantiation>{
          data::type("type<A>"), data::file_location("file", 10, 20),
          data::file_location("file_sl", 15, 25), 100.0}

  };

  assert_throw(
      [&events] {
        metaprogram_builder(events, data::metaprogram::mode_t::normal,
                            data::cpp_code("root_name"))
            .get_metaprogram();
      },
      "Some Templight TemplateEnd events are missing");
}
