// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/comment.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

using pattern::_;

TEST(engine_switch, unused_arguments)
{
  metashell_instance mi{{"--",
                         "-g",
                         "-g0",
                         "-g1",
                         "-g2",
                         "-g3",
                         "-ggdb",
                         "-ggdb0",
                         "-ggdb1",
                         "-ggdb2",
                         "-ggdb3",
                         "-gdwarf",
                         "-gdwarf-2",
                         "-gdwarf-3",
                         "-gdwarf-4",
                         "-gdwarf-5",
                         "-gsplit-dwarf",
                         "-gpubnames",
                         "-ggnu-pubnames",
                         "-grecord-gcc-switches",
                         "-gno-record-gcc-switches",
                         "-gstrict-dwarf",
                         "-gno-strict-dwarf",
                         "-gcolumn-info",
                         "-gno-column-info",
                         "-gz",
                         "-gz=none",
                         "-gz=zlib",
                         "-gz=zlib-gnu",
                         "-gused",
                         "-gfull",
#ifndef _WIN32
                         "-fPIC",
                         "-fPIE",
#endif
                         "-pedantic",
                         "-fvisibility-inlines-hidden",
                         "-ffunction-sections",
                         "-fdata-sections",
                         "-fexceptions",
                         "-fno-exceptions",
                         "-frtti",
                         "-fno-rtti",
                         "-O",
                         "-O0",
                         "-O1",
                         "-O2",
                         "-O3",
                         "-Os",
                         "-Ofast",
                         "-Og",
                         "-m32",
                         "-fbuiltin",
                         "-fno-builtin",
                         "-fno-builtin-function",
                         "-fomit-frame-pointer",
                         "-fno-omit-frame-pointer",
                         "-funwind-tables",
                         "-fstack-protector",
                         "-fstack-protector-all",
                         "-fstack-protector-strong",
                         "-fno-stack-protector",
                         "-fvisibility=hidden",
                         "-flto",
                         "-fno-lto",
                         "-ftls-model=local-exec",
                         "-ffreestanding",
                         "-mmmx",
                         "-msse",
                         "-msse2",
                         "-msse3",
                         "-mssse3",
                         "-msse4",
                         "-msse4a",
                         "-msse4.1",
                         "-msse4.2",
                         "-mavx",
                         "-mavx2",
                         "-mavx512f",
                         "-mavx512pf",
                         "-mavx512er",
                         "-mavx512cd",
                         "-mavx512vl",
                         "-mavx512bw",
                         "-mavx512dq",
                         "-mavx512ifma",
                         "-mavx512vbmi",
                         "-msha",
                         "-maes",
                         "-mpclmul",
                         "-mclflushopt",
                         "-mfsgsbase",
                         "-mrdrnd",
                         "-mf16c",
                         "-mfma",
                         "-mfma4",
                         "-mprefetchwt1",
                         "-mxop",
                         "-mlwp",
                         "-m3dnow",
                         "-m3dnowa",
                         "-mpopcnt",
                         "-mbmi",
                         "-mbmi2",
                         "-mlzcnt",
                         "-mfxsr",
                         "-mxsave",
                         "-mxsaveopt",
                         "-mxsavec",
                         "-mxsaves",
                         "-mrtm",
                         "-mtbm",
                         "-mmpx",
                         "-mmwaitx",
                         "-mclzero",
                         "-mpku",
                         "-pthread"},
                        {},
                        false};

  ASSERT_EQ(comment(_), mi.command("#msh engine switch null").front());
}
