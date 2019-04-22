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

#include <metashell/data/cpp_code.hpp>

#include <stdexcept>

namespace
{
  template <class Str>
  std::string wrap(const Str& s_, const std::string& wrapper_)
  {
    return wrapper_ + s_ + wrapper_;
  }

  std::string marker(bool process_directives_)
  {
    return wrap("* __METASHELL_PP_MARKER *",
                std::string(process_directives_ ? "\n" : ""));
  }
}

namespace metashell
{
  namespace data
  {
    cpp_code add_markers(const cpp_code& code_, bool process_directives_)
    {
      return cpp_code(wrap(code_, marker(process_directives_)));
    }

    cpp_code remove_markers(const cpp_code& code_, bool process_directives_)
    {
      const std::string code = code_.value();
      const std::string mrk = marker(process_directives_);

      const auto p1 = code.find(mrk);
      if (p1 == std::string::npos)
      {
        throw std::runtime_error(
            "Marker (" + mrk +
            ") not found in preprocessed output."
            " Does it contain a macro that has been defined?");
      }
      else
      {
        const auto full_len = mrk.size();
        const auto from = p1 + full_len;

        const auto last_non_new_line = mrk.find_last_not_of("\n\r");
        const auto m_len = (last_non_new_line == std::string::npos) ?
                               full_len :
                               last_non_new_line + 1;
        const auto p2 = code.find(mrk, p1 + m_len);
        if (p2 == std::string::npos)
        {
          throw std::runtime_error("Marker (" + mrk +
                                   ") found only once in preprocessed output.");
        }
        else if (code.find(mrk, p2 + m_len) == std::string::npos)
        {
          return p2 < from ? cpp_code() :
                             cpp_code(code.substr(from, p2 - from));
        }
        else
        {
          throw std::runtime_error(
              "Marker (" + mrk +
              ") found more than two times in preprocessed output.");
        }
      }
    }

    int lines_in(const cpp_code& code_) { return int(count(code_, '\n')); }
  }
} // namespace metashell:data
