// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/templight/entry.hpp>

#include <metashell/mock/environment.hpp>
#include <metashell/mock/environment_detector.hpp>

#include <metashell/core/null_displayer.hpp>

#include <gtest/gtest.h>

using namespace metashell;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

TEST(evaluation, throwing_environment_not_breaking_validate)
{
  data::config cfg;
  cfg.push_back(data::shell_config(
      data::shell_config_name("test"), data::shell_config_data()));

  NiceMock<mock::environment_detector> det;
  NiceMock<mock::environment> e;
  core::null_displayer d;

  ON_CALL(e, append(_)).WillByDefault(Throw(std::runtime_error("some error")));

  ON_CALL(det, on_windows()).WillByDefault(Return(false));
  ON_CALL(det, on_osx()).WillByDefault(Return(false));
  ON_CALL(det, directory_of_executable()).WillByDefault(Return(""));
  ON_CALL(det, file_exists(_)).WillByDefault(Return(true));

  const data::result r =
      engine::templight::entry(true, data::executable_path("metashell"))
          .build(
              cfg.active_shell_config(), "", "", "env.hpp", {}, det, d, nullptr)
          ->cpp_validator()
          .validate_code(data::cpp_code("typedef int foo;"), cfg, e, false);

  ASSERT_FALSE(r.successful);
  ASSERT_FALSE(r.error.empty());
}
