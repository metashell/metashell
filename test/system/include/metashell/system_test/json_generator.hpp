#ifndef METASHELL_SYSTEM_TEST_JSON_GENERATOR_HPP
#define METASHELL_SYSTEM_TEST_JSON_GENERATOR_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/json_string.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    json_string command(const std::string& cmd_);
    json_string code_completion(const std::string& code_);

    json_string to_json(const std::string& s_);
    json_string to_json(const std::vector<std::string>& strings_);
  }
}

#endif
