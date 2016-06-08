// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/code_completer.hpp>
#include <metashell_system_test/code_completion_result.hpp>
#include <metashell_system_test/prompt.hpp>

#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/run_metashell.hpp>

#include <just/test.hpp>

using namespace metashell_system_test;

code_completer::code_completer(const std::string& init_code_)
  : _init_code(init_code_)
{
}

json_string code_completer::operator()(const std::string& code_) const
{
  const auto r = run_metashell({command(_init_code), code_completion(code_)});

  auto i = r.begin();

  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;
  JUST_ASSERT_EQUAL(prompt(">"), *i);
  ++i;

  JUST_ASSERT(i != r.end());
  const json_string result = *i;
  ++i;

  JUST_ASSERT(i == r.end());

  return result;
}
