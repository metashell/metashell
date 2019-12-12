#ifndef METASHELL_SYSTEM_TEST_CODE_COMPLETION_RESULT_HPP
#define METASHELL_SYSTEM_TEST_CODE_COMPLETION_RESULT_HPP

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

#include <metashell/system_test/json_string.hpp>

#include <iosfwd>
#include <set>
#include <string>

namespace metashell
{
  namespace system_test
  {
    class code_completion_result
    {
    public:
      explicit code_completion_result(
          const std::initializer_list<std::string>& results_);

      typedef std::set<std::string>::const_iterator iterator;
      typedef iterator const_iterator;

      iterator begin() const;
      iterator end() const;

      bool contains(const std::string& result_) const;

      int size() const;

      code_completion_result
      with(const std::initializer_list<std::string>& members_) const;

    private:
      std::set<std::string> _results;
    };

    std::ostream& operator<<(std::ostream& out_,
                             const code_completion_result& r_);

    json_string to_json_string(const code_completion_result& r_);

    bool operator==(const code_completion_result& r_, const json_string& s_);
    bool operator!=(const code_completion_result& r_, const json_string& s_);
  }
}

#endif
