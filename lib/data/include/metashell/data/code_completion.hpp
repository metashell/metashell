#ifndef METASHELL_DATA_CODE_COMPLETION_HPP
#define METASHELL_DATA_CODE_COMPLETION_HPP

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

#include <metashell/data/user_input.hpp>

#include <optional>
#include <set>

namespace metashell
{
  namespace data
  {
    class code_completion
    {
    public:
      using container = std::set<user_input>;
      using size_type = container::size_type;

      code_completion() = default;
      code_completion(std::initializer_list<user_input>);

      void insert(user_input);
      void insert(code_completion&&);

      // The operation is expected to throw due to running out of memory in
      // which case it is acceptable to loose a code completion result.
      std::optional<user_input> pop();

      size_type size() const;
      bool empty() const;

    private:
      container _completions;
    };
  }
}

#endif
