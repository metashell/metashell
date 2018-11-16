// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/engine/clang/get_type_from_ast_string.hpp>

#include <metashell/data/exception.hpp>

#include <boost/algorithm/string/trim.hpp>

#include <regex>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      std::string repair_type_string(const std::string& type)
      {
        const std::regex bool_regex("(^|[^A-Za-z0-9_])_Bool([^A-Za-z0-9_]|$)");
        const std::regex type_regex(
            "(^|[^A-Za-z0-9_])(?:class |struct |union |enum )");

        return std::regex_replace(
            std::regex_replace(type, bool_regex, "$01bool$02"), type_regex,
            "$01");
      }

      std::string get_type_from_ast_string(const std::string& ast)
      {
        // This algorithm is very ugly, but it basically iterates on the
        // lines of the ast dump from the end until it finds the interesting
        // line.

        std::size_t end_index = std::string::npos;
        std::size_t start_index = ast.find_last_of('\n');

        const std::regex reg(
            ".*':'(?:struct )?metashell::impl::wrap<?(.*)>' "
            "'void \\((?:void)?\\)"
            "(?: __attribute__\\(\\(thiscall\\)\\)|(?:))(?: noexcept|(?:))'.*");

        std::string line;
        while (true)
        {
          end_index = start_index;
          start_index = ast.find_last_of('\n', end_index - 1);

          if (start_index == std::string::npos ||
              end_index == std::string::npos)
          {
            throw data::exception("No suitable ast line in dump");
          }

          line = ast.substr(start_index + 1, end_index - start_index - 1);

          std::smatch match;
          if (std::regex_match(line, match, reg))
          {
            return repair_type_string(boost::trim_copy(std::string(match[1])));
          }
        }
      }
    }
  }
}
