#ifndef METASHELL_SYSTEM_TEST_RAW_TEXT_HPP
#define METASHELL_SYSTEM_TEST_RAW_TEXT_HPP

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
#include <pattern/placeholder.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <string>

namespace metashell
{
  namespace system_test
  {
    class raw_text : boost::equality_comparable<raw_text, json_string>
    {
    public:
      explicit raw_text(const std::string& text_);
      explicit raw_text(pattern::placeholder);
      explicit raw_text(const json_string& s_);

      bool text_specified() const;
      const std::string& text() const;

    private:
      std::optional<std::string> _text;
    };

    std::ostream& operator<<(std::ostream& out_, const raw_text& raw_text_);

    json_string to_json_string(const raw_text& t_);

    bool operator==(const raw_text& text_, const json_string& s_);
  }
}

#endif
