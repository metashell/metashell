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

namespace
{
  json_string
  unused_argument(const metashell::data::command_line_argument_list& args_)
  {
    metashell::data::command_line_argument_list args{"--"};
    args.append(args_.begin(), args_.end());
    return metashell_instance{args, {}, false}
        .command("#msh engine switch null")
        .front();
  }
}

TEST(engine_switch, unused_arguments)
{
  ASSERT_EQ(comment(_), unused_argument({"-g"}));
  ASSERT_EQ(comment(_), unused_argument({"-g0"}));
  ASSERT_EQ(comment(_), unused_argument({"-g1"}));
  ASSERT_EQ(comment(_), unused_argument({"-g2"}));
  ASSERT_EQ(comment(_), unused_argument({"-g3"}));
  ASSERT_EQ(comment(_), unused_argument({"-ggdb"}));
  ASSERT_EQ(comment(_), unused_argument({"-ggdb0"}));
  ASSERT_EQ(comment(_), unused_argument({"-ggdb1"}));
  ASSERT_EQ(comment(_), unused_argument({"-ggdb2"}));
  ASSERT_EQ(comment(_), unused_argument({"-ggdb3"}));
  ASSERT_EQ(comment(_), unused_argument({"-gdwarf"}));
  ASSERT_EQ(comment(_), unused_argument({"-gdwarf-2"}));
  ASSERT_EQ(comment(_), unused_argument({"-gdwarf-3"}));
  ASSERT_EQ(comment(_), unused_argument({"-gdwarf-4"}));
  ASSERT_EQ(comment(_), unused_argument({"-gdwarf-5"}));
  ASSERT_EQ(comment(_), unused_argument({"-gsplit-dwarf"}));
  ASSERT_EQ(comment(_), unused_argument({"-gpubnames"}));
  ASSERT_EQ(comment(_), unused_argument({"-ggnu-pubnames"}));
  ASSERT_EQ(comment(_), unused_argument({"-grecord-gcc-switches"}));
  ASSERT_EQ(comment(_), unused_argument({"-gno-record-gcc-switches"}));
  ASSERT_EQ(comment(_), unused_argument({"-gstrict-dwarf"}));
  ASSERT_EQ(comment(_), unused_argument({"-gno-strict-dwarf"}));
  ASSERT_EQ(comment(_), unused_argument({"-gcolumn-info"}));
  ASSERT_EQ(comment(_), unused_argument({"-gno-column-info"}));
  ASSERT_EQ(comment(_), unused_argument({"-gz"}));
  ASSERT_EQ(comment(_), unused_argument({"-gz=none"}));
  ASSERT_EQ(comment(_), unused_argument({"-gz=zlib"}));
  ASSERT_EQ(comment(_), unused_argument({"-gz=zlib-gnu"}));
  ASSERT_EQ(comment(_), unused_argument({"-gused"}));
  ASSERT_EQ(comment(_), unused_argument({"-gfull"}));
#ifndef _WIN32
  ASSERT_EQ(comment(_), unused_argument({"-fPIC"}));
  ASSERT_EQ(comment(_), unused_argument({"-fPIE"}));
#endif
  ASSERT_EQ(comment(_), unused_argument({"-pedantic"}));
  ASSERT_EQ(comment(_), unused_argument({"-pedantic-errors"}));
  ASSERT_EQ(comment(_), unused_argument({"-fvisibility-inlines-hidden"}));
  ASSERT_EQ(comment(_), unused_argument({"-ffunction-sections"}));
  ASSERT_EQ(comment(_), unused_argument({"-fdata-sections"}));
  ASSERT_EQ(comment(_), unused_argument({"-fexceptions"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-exceptions"}));
  ASSERT_EQ(comment(_), unused_argument({"-frtti"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-rtti"}));
  ASSERT_EQ(comment(_), unused_argument({"-O"}));
  ASSERT_EQ(comment(_), unused_argument({"-O0"}));
  ASSERT_EQ(comment(_), unused_argument({"-O1"}));
  ASSERT_EQ(comment(_), unused_argument({"-O2"}));
  ASSERT_EQ(comment(_), unused_argument({"-O3"}));
  ASSERT_EQ(comment(_), unused_argument({"-Os"}));
  ASSERT_EQ(comment(_), unused_argument({"-Ofast"}));
  ASSERT_EQ(comment(_), unused_argument({"-Og"}));
  ASSERT_EQ(comment(_), unused_argument({"-Oz"}));
  ASSERT_EQ(comment(_), unused_argument({"-m32"}));
  ASSERT_EQ(comment(_), unused_argument({"-fbuiltin"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-builtin"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-builtin-function"}));
  ASSERT_EQ(comment(_), unused_argument({"-fomit-frame-pointer"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-omit-frame-pointer"}));
  ASSERT_EQ(comment(_), unused_argument({"-funwind-tables"}));
  ASSERT_EQ(comment(_), unused_argument({"-fstack-protector"}));
  ASSERT_EQ(comment(_), unused_argument({"-fstack-protector-all"}));
  ASSERT_EQ(comment(_), unused_argument({"-fstack-protector-strong"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-stack-protector"}));
  ASSERT_EQ(comment(_), unused_argument({"-fvisibility=hidden"}));
  ASSERT_EQ(comment(_), unused_argument({"-flto"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-lto"}));
  ASSERT_EQ(comment(_), unused_argument({"-ftls-model=local-exec"}));
  ASSERT_EQ(comment(_), unused_argument({"-ffreestanding"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-sanitize=all"}));
  ASSERT_EQ(comment(_), unused_argument({"-mmmx"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse2"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse3"}));
  ASSERT_EQ(comment(_), unused_argument({"-mssse3"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse4"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse4a"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse4.1"}));
  ASSERT_EQ(comment(_), unused_argument({"-msse4.2"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx2"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512f"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512pf"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512er"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512cd"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512vl"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512bw"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512dq"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512ifma"}));
  ASSERT_EQ(comment(_), unused_argument({"-mavx512vbmi"}));
  ASSERT_EQ(comment(_), unused_argument({"-msha"}));
  ASSERT_EQ(comment(_), unused_argument({"-maes"}));
  ASSERT_EQ(comment(_), unused_argument({"-mpclmul"}));
  ASSERT_EQ(comment(_), unused_argument({"-mclflushopt"}));
  ASSERT_EQ(comment(_), unused_argument({"-mfsgsbase"}));
  ASSERT_EQ(comment(_), unused_argument({"-mrdrnd"}));
  ASSERT_EQ(comment(_), unused_argument({"-mf16c"}));
  ASSERT_EQ(comment(_), unused_argument({"-mfma"}));
  ASSERT_EQ(comment(_), unused_argument({"-mfma4"}));
  ASSERT_EQ(comment(_), unused_argument({"-mprefetchwt1"}));
  ASSERT_EQ(comment(_), unused_argument({"-mxop"}));
  ASSERT_EQ(comment(_), unused_argument({"-mlwp"}));
  ASSERT_EQ(comment(_), unused_argument({"-m3dnow"}));
  ASSERT_EQ(comment(_), unused_argument({"-m3dnowa"}));
  ASSERT_EQ(comment(_), unused_argument({"-mpopcnt"}));
  ASSERT_EQ(comment(_), unused_argument({"-mbmi"}));
  ASSERT_EQ(comment(_), unused_argument({"-mbmi2"}));
  ASSERT_EQ(comment(_), unused_argument({"-mlzcnt"}));
  ASSERT_EQ(comment(_), unused_argument({"-mfxsr"}));
  ASSERT_EQ(comment(_), unused_argument({"-mxsave"}));
  ASSERT_EQ(comment(_), unused_argument({"-mxsaveopt"}));
  ASSERT_EQ(comment(_), unused_argument({"-mxsavec"}));
  ASSERT_EQ(comment(_), unused_argument({"-mxsaves"}));
  ASSERT_EQ(comment(_), unused_argument({"-mrtm"}));
  ASSERT_EQ(comment(_), unused_argument({"-mtbm"}));
  ASSERT_EQ(comment(_), unused_argument({"-mmpx"}));
  ASSERT_EQ(comment(_), unused_argument({"-mmwaitx"}));
  ASSERT_EQ(comment(_), unused_argument({"-mclzero"}));
  ASSERT_EQ(comment(_), unused_argument({"-mpku"}));
  ASSERT_EQ(comment(_), unused_argument({"-pthread"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-common"}));
  ASSERT_EQ(comment(_), unused_argument({"-fstrict-aliasing"}));
  ASSERT_EQ(comment(_), unused_argument({"-fno-strict-aliasing"}));
  ASSERT_EQ(comment(_), unused_argument({"-stdlib=libc++"}));
  ASSERT_EQ(comment(_), unused_argument({"-miphoneos-version-min=8.0"}));
  ASSERT_EQ(comment(_), unused_argument({"-mmacosx-version-min=10.9"}));
  ASSERT_EQ(comment(_), unused_argument({"-static"}));
  ASSERT_EQ(comment(_), unused_argument({"-mfloat-abi=hard"}));
  ASSERT_EQ(comment(_), unused_argument({"-mkernel"}));
  ASSERT_EQ(comment(_), unused_argument({"-arch", "armv7", "-target",
                                         "thumbv7-apple-darwin-eabi"}));
  ASSERT_EQ(comment(_), unused_argument({"-fobjc-arc"}));
}
