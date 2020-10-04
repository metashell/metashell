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

#include <metashell/pragma/includes.hpp>

#include <metashell/engine/constant/builder.hpp>

#include <metashell/core/in_memory_displayer.hpp>

#include <metashell/mock/main_shell.hpp>

#include <gtest/gtest.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>

using namespace metashell;

using ::testing::NiceMock;
using ::testing::ReturnRef;

namespace
{
  const std::vector<std::vector<boost::filesystem::path>> foo_bar{
      {"foo", "bar"}};

  template <data::include_type Type>
  std::vector<std::vector<boost::filesystem::path>> pragma_includes_displays()
  {
    std::unique_ptr<iface::engine> engine =
        engine::constant::create_with_include_path(
            Type, {"foo", "bar"})(data::shell_config{
            data::shell_config_name("default"), data::shell_config_data()});
    NiceMock<mock::main_shell> sh;
    ON_CALL(sh, engine()).WillByDefault(ReturnRef(*engine));

    core::in_memory_displayer d;
    pragma::includes<Type>().run(sh, d);

    return d.filename_lists();
  }
} // namespace

TEST(include_pragmas, pragma_sysincludes_displays_sysincludes)
{
  ASSERT_EQ(foo_bar, pragma_includes_displays<data::include_type::sys>());
}

TEST(include_pargmas, pragma_quoteincludes_displays_quoteincludes)
{
  ASSERT_EQ(foo_bar, pragma_includes_displays<data::include_type::quote>());
}
