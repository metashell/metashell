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

#include <metashell/system_test/cpp_code.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/prompt.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

TEST(pp, old_standard)
{
  if (!using_msvc() && !using_wave())
  {
    for (const auto standard : {"-std=c++98", "-std=c99"})
    {
      metashell_instance mi{
          {"--preprocessor", "--", standard}, boost::filesystem::path{}};

      ASSERT_EQ(cpp_code{"works"}, mi.command("works").front());
    }
  }
}
