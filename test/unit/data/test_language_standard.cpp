// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/language_standard.hpp>
#include <metashell/data/unknown_standard.hpp>

#include <gtest/gtest.h>

using namespace metashell::data;

TEST(language_standard, parse_invalid_standard)
{
  ASSERT_THROW(
      parse_standard(real_engine_name::clang, "foo"), unknown_standard);
  ASSERT_THROW(parse_standard(real_engine_name::gcc, "foo"), unknown_standard);
  ASSERT_THROW(
      parse_standard(real_engine_name::internal, "foo"), unknown_standard);
  ASSERT_THROW(parse_standard(real_engine_name::msvc, "foo"), unknown_standard);
  ASSERT_THROW(parse_standard(real_engine_name::null, "foo"), unknown_standard);
  ASSERT_THROW(
      parse_standard(real_engine_name::pure_wave, "foo"), unknown_standard);
  ASSERT_THROW(
      parse_standard(real_engine_name::templight, "foo"), unknown_standard);
  ASSERT_THROW(parse_standard(real_engine_name::wave, "foo"), unknown_standard);
}

TEST(language_standard, wave_standard)
{
  ASSERT_EQ(wave_name(language_standard::c89), command_line_argument("--c99"));
  ASSERT_EQ(
      wave_name(language_standard::gnu89), command_line_argument("--c99"));
  ASSERT_EQ(wave_name(language_standard::c90), command_line_argument("--c99"));
  ASSERT_EQ(wave_name(language_standard::iso9899_199409),
            command_line_argument("--c99"));
  ASSERT_EQ(
      wave_name(language_standard::gnu90), command_line_argument("--c99"));
  ASSERT_EQ(wave_name(language_standard::c9x), command_line_argument("--c99"));
  ASSERT_EQ(
      wave_name(language_standard::gnu9x), command_line_argument("--c99"));
  ASSERT_EQ(wave_name(language_standard::c99), command_line_argument("--c99"));
  ASSERT_EQ(
      wave_name(language_standard::gnu99), command_line_argument("--c99"));
  ASSERT_EQ(wave_name(language_standard::c1x), command_line_argument("--c99"));
  ASSERT_EQ(
      wave_name(language_standard::gnu1x), command_line_argument("--c99"));
  ASSERT_EQ(wave_name(language_standard::c11), command_line_argument("--c99"));
  ASSERT_EQ(
      wave_name(language_standard::gnu11), command_line_argument("--c99"));

  ASSERT_EQ(
      wave_name(language_standard::cpp98), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnu98), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp03), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnu03), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp0x), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp0x), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp11), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp11), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp1y), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp1y), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp14), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp14), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp1z), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp1z), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp17), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp17), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp2a), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp2a), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::cpp20), command_line_argument("--c++11"));
  ASSERT_EQ(
      wave_name(language_standard::gnupp20), command_line_argument("--c++11"));
}

TEST(language_standard, gcc_standard)
{
  const real_engine_name engines[] = {
      real_engine_name::clang, real_engine_name::gcc,
      real_engine_name::internal, real_engine_name::templight};

  for (const real_engine_name engine : engines)
  {
    ASSERT_EQ(language_standard::c89, parse_standard(engine, "c89"));
    ASSERT_EQ(language_standard::gnu89, parse_standard(engine, "gnu89"));
    ASSERT_EQ(language_standard::c90, parse_standard(engine, "c90"));
    ASSERT_EQ(language_standard::c90, parse_standard(engine, "iso9899:1990"));
    ASSERT_EQ(language_standard::iso9899_199409,
              parse_standard(engine, "iso9899:199409"));
    ASSERT_EQ(language_standard::gnu90, parse_standard(engine, "gnu90"));
    ASSERT_EQ(language_standard::c9x, parse_standard(engine, "c9x"));
    ASSERT_EQ(language_standard::c9x, parse_standard(engine, "iso9899:199x"));
    ASSERT_EQ(language_standard::gnu9x, parse_standard(engine, "gnu9x"));
    ASSERT_EQ(language_standard::c99, parse_standard(engine, "c99"));
    ASSERT_EQ(language_standard::c99, parse_standard(engine, "iso9899:1999"));
    ASSERT_EQ(language_standard::gnu99, parse_standard(engine, "gnu99"));
    ASSERT_EQ(language_standard::c1x, parse_standard(engine, "c1x"));
    ASSERT_EQ(language_standard::gnu1x, parse_standard(engine, "gnu1x"));
    ASSERT_EQ(language_standard::c11, parse_standard(engine, "c11"));
    ASSERT_EQ(language_standard::c11, parse_standard(engine, "iso9899:2011"));
    ASSERT_EQ(language_standard::gnu11, parse_standard(engine, "gnu11"));

    ASSERT_EQ(language_standard::cpp98, parse_standard(engine, "c++98"));
    ASSERT_EQ(language_standard::gnu98, parse_standard(engine, "gnu98"));
    ASSERT_EQ(language_standard::cpp03, parse_standard(engine, "c++03"));
    ASSERT_EQ(language_standard::gnu03, parse_standard(engine, "gnu03"));
    ASSERT_EQ(language_standard::cpp0x, parse_standard(engine, "c++0x"));
    ASSERT_EQ(language_standard::gnupp0x, parse_standard(engine, "gnu++0x"));
    ASSERT_EQ(language_standard::cpp11, parse_standard(engine, "c++11"));
    ASSERT_EQ(language_standard::gnupp11, parse_standard(engine, "gnu++11"));
    ASSERT_EQ(language_standard::cpp1y, parse_standard(engine, "c++1y"));
    ASSERT_EQ(language_standard::gnupp1y, parse_standard(engine, "gnu++1y"));
    ASSERT_EQ(language_standard::cpp14, parse_standard(engine, "c++14"));
    ASSERT_EQ(language_standard::gnupp14, parse_standard(engine, "gnu++14"));
    ASSERT_EQ(language_standard::cpp1z, parse_standard(engine, "c++1z"));
    ASSERT_EQ(language_standard::gnupp1z, parse_standard(engine, "gnu++1z"));
    ASSERT_EQ(language_standard::cpp17, parse_standard(engine, "c++17"));
    ASSERT_EQ(language_standard::gnupp17, parse_standard(engine, "gnu++17"));
    ASSERT_EQ(language_standard::cpp2a, parse_standard(engine, "c++2a"));
    ASSERT_EQ(language_standard::gnupp2a, parse_standard(engine, "gnu++2a"));
    ASSERT_EQ(language_standard::cpp20, parse_standard(engine, "c++20"));
    ASSERT_EQ(language_standard::gnupp20, parse_standard(engine, "gnu++20"));
  }

  ASSERT_EQ(gcc_name(language_standard::c89), command_line_argument("c89"));
  ASSERT_EQ(gcc_name(language_standard::gnu89), command_line_argument("gnu89"));
  ASSERT_EQ(gcc_name(language_standard::c90), command_line_argument("c90"));
  ASSERT_EQ(gcc_name(language_standard::iso9899_199409),
            command_line_argument("iso9899:199409"));
  ASSERT_EQ(gcc_name(language_standard::gnu90), command_line_argument("gnu90"));
  ASSERT_EQ(gcc_name(language_standard::c9x), command_line_argument("c9x"));
  ASSERT_EQ(gcc_name(language_standard::gnu9x), command_line_argument("gnu9x"));
  ASSERT_EQ(gcc_name(language_standard::c99), command_line_argument("c99"));
  ASSERT_EQ(gcc_name(language_standard::gnu99), command_line_argument("gnu99"));
  ASSERT_EQ(gcc_name(language_standard::c1x), command_line_argument("c1x"));
  ASSERT_EQ(gcc_name(language_standard::gnu1x), command_line_argument("gnu1x"));
  ASSERT_EQ(gcc_name(language_standard::c11), command_line_argument("c11"));
  ASSERT_EQ(gcc_name(language_standard::gnu11), command_line_argument("gnu11"));

  ASSERT_EQ(gcc_name(language_standard::cpp98), command_line_argument("c++98"));
  ASSERT_EQ(gcc_name(language_standard::gnu98), command_line_argument("gnu98"));
  ASSERT_EQ(gcc_name(language_standard::cpp03), command_line_argument("c++03"));
  ASSERT_EQ(gcc_name(language_standard::gnu03), command_line_argument("gnu03"));
  ASSERT_EQ(gcc_name(language_standard::cpp0x), command_line_argument("c++0x"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp0x), command_line_argument("gnu++0x"));
  ASSERT_EQ(gcc_name(language_standard::cpp11), command_line_argument("c++11"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp11), command_line_argument("gnu++11"));
  ASSERT_EQ(gcc_name(language_standard::cpp1y), command_line_argument("c++1y"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp1y), command_line_argument("gnu++1y"));
  ASSERT_EQ(gcc_name(language_standard::cpp14), command_line_argument("c++14"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp14), command_line_argument("gnu++14"));
  ASSERT_EQ(gcc_name(language_standard::cpp1z), command_line_argument("c++1z"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp1z), command_line_argument("gnu++1z"));
  ASSERT_EQ(gcc_name(language_standard::cpp17), command_line_argument("c++17"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp17), command_line_argument("gnu++17"));
  ASSERT_EQ(gcc_name(language_standard::cpp2a), command_line_argument("c++2a"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp2a), command_line_argument("gnu++2a"));
  ASSERT_EQ(gcc_name(language_standard::cpp20), command_line_argument("c++20"));
  ASSERT_EQ(
      gcc_name(language_standard::gnupp20), command_line_argument("gnu++20"));
}
