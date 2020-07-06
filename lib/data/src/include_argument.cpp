// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/exception.hpp>
#include <metashell/data/include_argument.hpp>

#include <iostream>
#include <tuple>

namespace metashell
{
  namespace data
  {
    namespace
    {
      template <char Closing, class Pred>
      std::pair<boost::filesystem::path, command::iterator>
      parse_path_until_token(command::iterator begin_,
                             command::iterator end_,
                             Pred is_closing_token_)
      {
        const auto path_end = std::find_if(begin_, end_, is_closing_token_);
        if (path_end == end_)
        {
          throw exception(std::string("closing ") + Closing + " is missing.");
        }
        else
        {
          return {tokens_to_string(begin_, path_end).value(),
                  skip_all_whitespace(path_end + 1, end_)};
        }
      }
    }

    include_argument::include_argument(include_type type_,
                                       const boost::filesystem::path& path_)
      : type(type_), path(path_)
    {
    }

    std::pair<std::optional<include_argument>, command::iterator>
    include_argument::parse(const command::iterator& begin_,
                            const command::iterator& end_)
    {
      if (begin_ != end_)
      {
        if (type_of(*begin_) == token_type::operator_less)
        {
          const auto path = parse_path_until_token<'>'>(
              begin_ + 1, end_, [](const token& token_) {
                return type_of(token_) == token_type::operator_greater;
              });
          return {include_argument(include_type::sys, path.first), path.second};
        }
        else if (type_of(*begin_) == token_type::string_literal)
        {
          return {include_argument(
                      include_type::quote, string_literal_value(*begin_)),
                  begin_ + 1};
        }
        else if (include_quote_token(*begin_))
        {
          const auto path = parse_path_until_token<'"'>(
              begin_ + 1, end_, include_quote_token);
          return {
              include_argument(include_type::quote, path.first), path.second};
        }
      }

      return {std::nullopt, begin_};
    }

    std::ostream& operator<<(std::ostream& out_, const include_argument& arg_)
    {
      return out_ << include_code(arg_.type, arg_.path);
    }

    bool operator==(const include_argument& a_, const include_argument& b_)
    {
      return a_.type == b_.type && a_.path == b_.path;
    }

    bool operator<(const include_argument& a_, const include_argument& b_)
    {
      return std::tie(a_.type, a_.path) < std::tie(b_.type, b_.path);
    }

    std::string include_code(const include_argument& arg_)
    {
      return include_code(arg_.type, arg_.path);
    }
  }
}
