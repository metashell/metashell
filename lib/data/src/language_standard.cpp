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

#include <cassert>

namespace metashell
{
  namespace data
  {
    language_standard parse_standard(real_engine_name engine_,
                                     const std::string& std_)
    {
      switch (engine_)
      {
      case real_engine_name::clang:
      case real_engine_name::gcc:
      case real_engine_name::internal:
      case real_engine_name::templight:
        if (std_ == "c89")
          return language_standard::c89;
        if (std_ == "gnu89")
          return language_standard::gnu89;
        if (std_ == "c90" || std_ == "iso9899:1990")
          return language_standard::c90;
        if (std_ == "iso9899:199409")
          return language_standard::iso9899_199409;
        if (std_ == "gnu90")
          return language_standard::gnu90;
        if (std_ == "c9x" || std_ == "iso9899:199x")
          return language_standard::c9x;
        if (std_ == "gnu9x")
          return language_standard::gnu9x;
        if (std_ == "c99" || std_ == "iso9899:1999")
          return language_standard::c99;
        if (std_ == "gnu99")
          return language_standard::gnu99;
        if (std_ == "c1x")
          return language_standard::c1x;
        if (std_ == "gnu1x")
          return language_standard::gnu1x;
        if (std_ == "c11" || std_ == "iso9899:2011")
          return language_standard::c11;
        if (std_ == "gnu11")
          return language_standard::gnu11;

        if (std_ == "c++98")
          return language_standard::cpp98;
        if (std_ == "gnu98")
          return language_standard::gnu98;
        if (std_ == "c++03")
          return language_standard::cpp03;
        if (std_ == "gnu03")
          return language_standard::gnu03;
        if (std_ == "c++0x")
          return language_standard::cpp0x;
        if (std_ == "gnu++0x")
          return language_standard::gnupp0x;
        if (std_ == "c++11")
          return language_standard::cpp11;
        if (std_ == "gnu++11")
          return language_standard::gnupp11;
        if (std_ == "c++1y")
          return language_standard::cpp1y;
        if (std_ == "gnu++1y")
          return language_standard::gnupp1y;
        if (std_ == "c++14")
          return language_standard::cpp14;
        if (std_ == "gnu++14")
          return language_standard::gnupp14;
        if (std_ == "c++1z")
          return language_standard::cpp1z;
        if (std_ == "gnu++1z")
          return language_standard::gnupp1z;
        if (std_ == "c++17")
          return language_standard::cpp17;
        if (std_ == "gnu++17")
          return language_standard::gnupp17;
        if (std_ == "c++2a")
          return language_standard::cpp2a;
        if (std_ == "gnu++2a")
          return language_standard::gnupp2a;
        if (std_ == "c++20")
          return language_standard::cpp20;
        if (std_ == "gnu++20")
          return language_standard::gnupp20;
        throw unknown_standard(engine_, std_);
      case real_engine_name::msvc:
      case real_engine_name::null:
      case real_engine_name::pure_wave:
      case real_engine_name::wave:
        throw unknown_standard(engine_, std_);
      }
      assert(!"Invalid engine");
      throw unknown_standard(engine_, std_);
    }

    command_line_argument gcc_name(language_standard std_)
    {
      switch (std_)
      {
      case language_standard::c89:
        return command_line_argument("c89");
      case language_standard::gnu89:
        return command_line_argument("gnu89");
      case language_standard::c90:
        return command_line_argument("c90");
      case language_standard::iso9899_199409:
        return command_line_argument("iso9899:199409");
      case language_standard::gnu90:
        return command_line_argument("gnu90");
      case language_standard::c9x:
        return command_line_argument("c9x");
      case language_standard::gnu9x:
        return command_line_argument("gnu9x");
      case language_standard::c99:
        return command_line_argument("c99");
      case language_standard::gnu99:
        return command_line_argument("gnu99");
      case language_standard::c1x:
        return command_line_argument("c1x");
      case language_standard::gnu1x:
        return command_line_argument("gnu1x");
      case language_standard::c11:
        return command_line_argument("c11");
      case language_standard::gnu11:
        return command_line_argument("gnu11");

      case language_standard::cpp98:
        return command_line_argument("c++98");
      case language_standard::gnu98:
        return command_line_argument("gnu98");
      case language_standard::cpp03:
        return command_line_argument("c++03");
      case language_standard::gnu03:
        return command_line_argument("gnu03");
      case language_standard::cpp0x:
        return command_line_argument("c++0x");
      case language_standard::gnupp0x:
        return command_line_argument("gnu++0x");
      case language_standard::cpp11:
        return command_line_argument("c++11");
      case language_standard::gnupp11:
        return command_line_argument("gnu++11");
      case language_standard::cpp1y:
        return command_line_argument("c++1y");
      case language_standard::gnupp1y:
        return command_line_argument("gnu++1y");
      case language_standard::cpp14:
        return command_line_argument("c++14");
      case language_standard::gnupp14:
        return command_line_argument("gnu++14");
      case language_standard::cpp1z:
        return command_line_argument("c++1z");
      case language_standard::gnupp1z:
        return command_line_argument("gnu++1z");
      case language_standard::cpp17:
        return command_line_argument("c++17");
      case language_standard::gnupp17:
        return command_line_argument("gnu++17");
      case language_standard::cpp2a:
        return command_line_argument("c++2a");
      case language_standard::gnupp2a:
        return command_line_argument("gnu++2a");
      case language_standard::cpp20:
        return command_line_argument("c++20");
      case language_standard::gnupp20:
        return command_line_argument("gnu++20");
      }
      assert(!"Invalid language standard");
      return command_line_argument("");
    }

    command_line_argument wave_name(language_standard std_)
    {
      switch (std_)
      {
      case language_standard::c89:
      case language_standard::gnu89:
      case language_standard::c90:
      case language_standard::iso9899_199409:
      case language_standard::gnu90:
      case language_standard::c9x:
      case language_standard::gnu9x:
      case language_standard::c99:
      case language_standard::gnu99:
      case language_standard::c1x:
      case language_standard::gnu1x:
      case language_standard::c11:
      case language_standard::gnu11:
        return command_line_argument("--c99");
      case language_standard::cpp98:
      case language_standard::gnu98:
      case language_standard::cpp03:
      case language_standard::gnu03:
      case language_standard::cpp0x:
      case language_standard::gnupp0x:
      case language_standard::cpp11:
      case language_standard::gnupp11:
      case language_standard::cpp1y:
      case language_standard::gnupp1y:
      case language_standard::cpp14:
      case language_standard::gnupp14:
      case language_standard::cpp1z:
      case language_standard::gnupp1z:
      case language_standard::cpp17:
      case language_standard::gnupp17:
      case language_standard::cpp2a:
      case language_standard::gnupp2a:
      case language_standard::cpp20:
      case language_standard::gnupp20:
        return command_line_argument("--c++11");
      }
      assert(!"Invalid language standard");
      return command_line_argument("");
    }
  }
}
