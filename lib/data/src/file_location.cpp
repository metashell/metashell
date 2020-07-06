// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/data/file_location.hpp>

#include <cassert>
#include <cctype>
#include <optional>
#include <sstream>
#include <tuple>

namespace
{
  struct number_at_end
  {
    int result;
    std::size_t colon_at;
  };

  bool is_number(const std::string& s_, std::size_t from_, std::size_t to_)
  {
    assert(to_ != std::string::npos);

    return from_ != std::string::npos && from_ < (to_ - 1) &&
           std::all_of(s_.begin() + from_ + 1, s_.begin() + to_,
                       [](char c_) { return std::isdigit(c_); });
  }

  std::optional<number_at_end> parse_last_number(const std::string& s_,
                                                 std::size_t from_)
  {
    assert(from_ != std::string::npos);

    if (from_ == 0)
    {
      return std::nullopt;
    }
    else
    {
      const auto colon = s_.rfind(':', from_ - 1);
      return is_number(s_, colon, from_) ?
                 std::make_optional(number_at_end{
                     std::stoi(s_.substr(colon + 1, from_ - colon - 1)),
                     colon}) :
                 std::nullopt;
    }
  }
}

namespace metashell
{
  namespace data
  {
    file_location::file_location() : name(), row(-1), column(-1) {}

    file_location::file_location(const boost::filesystem::path& name,
                                 int row,
                                 int column)
      : name(name), row(row), column(column)
    {
    }

    file_location file_location::parse(const std::string& s_)
    {
      if (const auto n1 = parse_last_number(s_, s_.size()))
      {
        if (const auto n2 = parse_last_number(s_, n1->colon_at))
        {
          return file_location(
              s_.substr(0, n2->colon_at), n2->result, n1->result);
        }
        else
        {
          return file_location(s_.substr(0, n1->colon_at), n1->result, 1);
        }
      }
      else
      {
        return file_location(s_, 1, 1);
      }
    }

    std::ostream& operator<<(std::ostream& os, const file_location& location)
    {
      os << location.name.string() << ":" << location.row << ":"
         << location.column;
      return os;
    }

    bool operator<(const file_location& lhs, const file_location& rhs)
    {
      return std::tie(lhs.name, lhs.row, lhs.column) <
             std::tie(rhs.name, rhs.row, rhs.column);
    }

    bool operator==(const file_location& lhs, const file_location& rhs)
    {
      return std::tie(lhs.name, lhs.row, lhs.column) ==
             std::tie(rhs.name, rhs.row, rhs.column);
    }

    std::string to_string(const file_location& location)
    {
      std::stringstream ss;
      ss << location;
      return ss.str();
    }

    std::optional<file_location>
    determine_from_line(const cpp_code& env,
                        const std::optional<cpp_code>& expression,
                        const std::string& stdin_name)
    {
      return expression ? std::make_optional(
                              file_location{stdin_name, lines_in(env) + 1, 1}) :
                          std::nullopt;
    }
  }
}
