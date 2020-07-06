#ifndef METASHELL_SYSTEM_TEST_AUTO_ENGINE_AUTO_ENGINE_TEST_HPP
#define METASHELL_SYSTEM_TEST_AUTO_ENGINE_AUTO_ENGINE_TEST_HPP

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

#include <metashell/data/command_line_argument_list.hpp>
#include <metashell/data/executable_path.hpp>

#include <optional>
#include <vector>

class auto_engine_test
{
public:
  explicit auto_engine_test(metashell::data::executable_path metashell_);

  void test_engine_selection(
      const metashell::data::command_line_argument_list& args_,
      const std::optional<std::string>& expected_engine_);

private:
  metashell::data::executable_path _metashell;
};

#endif
