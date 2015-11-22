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

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_metaprogram_builder_normal_mode) {
  metaprogram_builder mb(
    metaprogram::mode_t::normal,
    "root_name",
    data::file_location("stdin.hpp", 10, 20),
    data::type("eval_result"));

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<A>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    100.0);

  mb.handle_template_end(110.0);

  metaprogram mp = mb.get_metaprogram();

  JUST_ASSERT_EQUAL(metaprogram::mode_t::normal, mp.get_mode());
  JUST_ASSERT_EQUAL(2u, mp.get_num_vertices());
  JUST_ASSERT_EQUAL(1u, mp.get_num_edges());

  JUST_ASSERT(mp.is_at_start());

  mp.step();

  auto frame = mp.get_current_frame();

  JUST_ASSERT(frame.is_full());
  JUST_ASSERT(!frame.is_profiled());
  JUST_ASSERT_EQUAL("type<A>", frame.type().name());
  JUST_ASSERT_EQUAL(
    data::instantiation_kind::template_instantiation, frame.kind());

  mp.step();

  JUST_ASSERT(mp.is_finished());
}

JUST_TEST_CASE(test_metaprogram_builder_full_mode) {
  metaprogram_builder mb(
    metaprogram::mode_t::full,
    "root_name",
    data::file_location("stdin.hpp", 10, 20),
    data::type("eval_result"));

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<A>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    100.0);

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<B>"),
    data::file_location("file", 20, 20),
    data::file_location("file_sl", 15, 25),
    110.0);

  mb.handle_template_end(120.0);

  mb.handle_template_end(130.0);

  mb.handle_template_begin(
    data::instantiation_kind::memoization,
    data::type("type<A>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    140.0);

  mb.handle_template_end(150.0);

  metaprogram mp = mb.get_metaprogram();

  JUST_ASSERT_EQUAL(metaprogram::mode_t::full, mp.get_mode());
  JUST_ASSERT_EQUAL(3u, mp.get_num_vertices());
  JUST_ASSERT_EQUAL(3u, mp.get_num_edges());

  JUST_ASSERT(mp.is_at_start());

  mp.step();

  {
    auto frame = mp.get_current_frame();

    JUST_ASSERT(!frame.is_full());
    JUST_ASSERT(!frame.is_profiled());
    JUST_ASSERT_EQUAL("type<A>", frame.type().name());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    JUST_ASSERT(!frame.is_full());
    JUST_ASSERT(!frame.is_profiled());
    JUST_ASSERT_EQUAL("type<B>", frame.type().name());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    JUST_ASSERT(!frame.is_full());
    JUST_ASSERT(!frame.is_profiled());
    JUST_ASSERT_EQUAL("type<A>", frame.type().name());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    JUST_ASSERT(!frame.is_full());
    JUST_ASSERT(!frame.is_profiled());
    JUST_ASSERT_EQUAL("type<B>", frame.type().name());
  }

  mp.step();

  JUST_ASSERT(mp.is_finished());

}

JUST_TEST_CASE(test_metaprogram_builder_profile_mode) {
  metaprogram_builder mb(
    metaprogram::mode_t::profile,
    "root_name",
    data::file_location("stdin.hpp", 10, 20),
    data::type("eval_result"));

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<A>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    100.0);

  mb.handle_template_end(110.0);

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<B>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    120.0);

  mb.handle_template_end(140.0);

  metaprogram mp = mb.get_metaprogram();
  mp.init_full_time_taken();

  JUST_ASSERT_EQUAL(metaprogram::mode_t::profile, mp.get_mode());
  JUST_ASSERT_EQUAL(3u, mp.get_num_vertices());
  JUST_ASSERT_EQUAL(2u, mp.get_num_edges());

  JUST_ASSERT(mp.is_at_start());

  mp.step();

  {
    auto frame = mp.get_current_frame();

    JUST_ASSERT(frame.is_full());
    JUST_ASSERT(frame.is_profiled());
    JUST_ASSERT_EQUAL("type<B>", frame.type().name());
    JUST_ASSERT_EQUAL(
      data::instantiation_kind::template_instantiation, frame.kind());
    JUST_ASSERT_EQUAL(20.0, frame.time_taken());
    JUST_ASSERT_EQUAL(0.5, frame.time_taken_ratio());
  }

  mp.step();

  {
    auto frame = mp.get_current_frame();

    JUST_ASSERT(frame.is_full());
    JUST_ASSERT(frame.is_profiled());
    JUST_ASSERT_EQUAL("type<A>", frame.type().name());
    JUST_ASSERT_EQUAL(
      data::instantiation_kind::template_instantiation, frame.kind());
    JUST_ASSERT_EQUAL(10.0, frame.time_taken());
    JUST_ASSERT_EQUAL(0.25, frame.time_taken_ratio());
  }

  mp.step();

  JUST_ASSERT(mp.is_finished());
}

JUST_TEST_CASE(test_metaprogram_builder_too_much_end_events_1) {
  metaprogram_builder mb(
    metaprogram::mode_t::normal,
    "root_name",
    data::file_location("stdin.hpp", 40, 50),
    data::type("eval_result"));

  JUST_ASSERT_THROWS<std::exception>([&] {
    mb.handle_template_end(100.0);
  }).check_exception(JUST_WHAT_RETURNS(
    "Mismatched Templight TemplateBegin and TemplateEnd events"));
}

JUST_TEST_CASE(test_metaprogram_builder_too_much_end_events_2) {
  metaprogram_builder mb(
    metaprogram::mode_t::normal,
    "root_name",
    data::file_location("stdin.hpp", 30, 45),
    data::type("eval_result"));

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<A>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    100.0);

  mb.handle_template_end(110.0);

  JUST_ASSERT_THROWS<std::exception>([&] {
    mb.handle_template_end(120.0);
  }).check_exception(JUST_WHAT_RETURNS(
    "Mismatched Templight TemplateBegin and TemplateEnd events"));
}

JUST_TEST_CASE(test_metaprogram_builder_too_few_end_events) {
  metaprogram_builder mb(
    metaprogram::mode_t::normal,
    "root_name",
    data::file_location("stdin.hpp", 30, 31),
    data::type("eval_result"));

  mb.handle_template_begin(
    data::instantiation_kind::template_instantiation,
    data::type("type<A>"),
    data::file_location("file", 10, 20),
    data::file_location("file_sl", 15, 25),
    100.0);

  JUST_ASSERT_THROWS<std::exception>([&] {
    mb.get_metaprogram();
  }).check_exception(JUST_WHAT_RETURNS(
    "Some Templight TemplateEnd events are missing"));
}
