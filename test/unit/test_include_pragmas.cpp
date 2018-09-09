// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/engine_constant.hpp>
#include <metashell/core/in_memory_displayer.hpp>
#include <metashell/core/shell.hpp>

#include "test_config.hpp"

#include <gtest/gtest.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  const std::vector<std::vector<boost::filesystem::path>> foo_bar{
      {"foo", "bar"}};

  template <data::include_type Type>
  std::vector<std::vector<boost::filesystem::path>> pragma_includes_displays()
  {
    core::in_memory_displayer d;
    core::shell sh(metashell::test_config(), "", "", "",
                   core::create_engine_with_include_path(Type, {"foo", "bar"}));
    sh.line_available("#msh " + to_string(Type) + "includes", d);

    return d.filename_lists();
  }
}

TEST(include_pragmas, pragma_sysincludes_displays_sysincludes)
{
  ASSERT_EQ(foo_bar, pragma_includes_displays<data::include_type::sys>());
}

TEST(include_pargmas, pragma_quoteincludes_displays_quoteincludes)
{
  ASSERT_EQ(foo_bar, pragma_includes_displays<data::include_type::quote>());
}
