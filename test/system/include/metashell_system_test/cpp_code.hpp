#ifndef METASHELL_SYSTEM_TEST_CPP_CODE_HPP
#define METASHELL_SYSTEM_TEST_CPP_CODE_HPP

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

#include <metashell_system_test/json_string.hpp>
#include <pattern/string.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell_system_test
{
  class cpp_code : boost::equality_comparable<cpp_code, json_string>
  {
  public:
    explicit cpp_code(pattern::string code_);
    explicit cpp_code(const json_string& s_);

    const pattern::string& code() const;

  private:
    pattern::string _code;
  };

  std::ostream& operator<<(std::ostream& out_, const cpp_code& code_);

  json_string to_json_string(const cpp_code& code_);

  bool operator==(const cpp_code& code_, const json_string& s_);
}

#endif
