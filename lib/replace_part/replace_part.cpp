// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/replace_part/replace_part.hpp>

#include <just/lines.hpp>

#include <iostream>
#include <sstream>

namespace
{
  template <class StringRange>
  void replace_impl(StringRange& lines_,
                    const std::string& marker_,
                    const std::string& replace_with_,
                    std::ostream& out_)
  {
    bool in_text_to_replace = false;

    for (const std::string& line : lines_)
    {
      const bool marked_line = line.find(marker_) != std::string::npos;

      if (!in_text_to_replace || marked_line)
      {
        out_ << line;
      }

      if (marked_line)
      {
        if (!in_text_to_replace)
        {
          out_ << replace_with_ << '\n';
        }

        in_text_to_replace = !in_text_to_replace;
      }
    }
  }
}

namespace replace_part
{
  void replace(const boost::filesystem::path& input_,
               const std::string& marker_,
               const std::string& replace_with_,
               std::ostream& out_)
  {
    just::lines::basic_file_view<true> lines(input_.string());
    replace_impl(lines, marker_, replace_with_, out_);
  }

  void replace(const std::string& input_,
               const std::string& marker_,
               const std::string& replace_with_,
               std::ostream& out_)
  {
    auto lines = just::lines::view_of<true>(input_);
    replace_impl(lines, marker_, replace_with_, out_);
  }

  std::string replace(const boost::filesystem::path& input_,
                      const std::string& marker_,
                      const std::string& replace_with_)
  {
    std::ostringstream out;
    replace(input_, marker_, replace_with_, out);
    return out.str();
  }

  std::string replace(const std::string& input_,
                      const std::string& marker_,
                      const std::string& replace_with_)
  {
    std::ostringstream out;
    replace(input_, marker_, replace_with_, out);
    return out.str();
  }
}
