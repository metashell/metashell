#ifndef METASHELL_TOKEN_HPP
#define METASHELL_TOKEN_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/token_.hpp>
#include <metashell/data/token_category.hpp>
#include <metashell/data/token_type.hpp>

#include <variant.hpp>

#include <cassert>
#include <iosfwd>
#include <sstream>
#include <string>

namespace metashell
{
  namespace data
  {
    using token = mpark::variant<token_<token_category::unknown>,

                                 token_<token_category::character_literal>,
                                 token_<token_category::floating_literal>,
                                 token_<token_category::identifier>,
                                 token_<token_category::integer_literal>,
                                 token_<token_category::string_literal>,
                                 token_<token_category::bool_literal>,
                                 token_<token_category::whitespace>,
                                 token_<token_category::comment>,
                                 token_<token_category::keyword>,
                                 token_<token_category::operator_token>,
                                 token_<token_category::preprocessor>>;

    token make_token(cpp_code, token_type);

    token_type type_of(const token&);
    const cpp_code& value(const token&);

    token_category category(const token&);

    bool can_be_part_of_name(const token&);

    std::string format_token(const token& t_);

    std::ostream& operator<<(std::ostream& out_, const token& t_);

    std::string string_literal_value(const token& token_);

    template <class TokenIt>
    cpp_code tokens_to_string(TokenIt begin_, const TokenIt& end_)
    {
      std::ostringstream s;
      while (begin_ != end_)
      {
        s << begin_->value();
        ++begin_;
      }
      return cpp_code(s.str());
    }
  }
}

namespace mindent
{
  template <class TokenType>
  class token_traits;

  template <>
  class token_traits<metashell::data::token>
  {
  public:
    typedef metashell::data::token token_type;
    typedef std::string string_type;

    static bool is_double_colon(const token_type& t_)
    {
      return category(t_) == metashell::data::token_category::operator_token &&
             metashell::data::value(t_) == "::";
    }

    static bool is_c_comment(const token_type& t_)
    {
      return category(t_) == metashell::data::token_category::comment &&
             metashell::data::value(t_).size() >= 4 &&
             metashell::data::value(t_)[0] == '/' &&
             metashell::data::value(t_)[1] == '*';
    }

    static bool is_less(const token_type& t_)
    {
      return category(t_) == metashell::data::token_category::operator_token &&
             metashell::data::value(t_) == "<";
    }

    static bool is_greater(const token_type& t_)
    {
      return category(t_) == metashell::data::token_category::operator_token &&
             metashell::data::value(t_) == ">";
    }

    static bool is_comma(const token_type& t_)
    {
      return category(t_) == metashell::data::token_category::operator_token &&
             metashell::data::value(t_) == ",";
    }

    static bool is_space(const token_type& t_)
    {
      return category(t_) == metashell::data::token_category::unknown ||
             category(t_) == metashell::data::token_category::whitespace;
    }

    static token_type empty_token()
    {
      return metashell::data::make_token<metashell::data::token_type::unknown>(
          metashell::data::cpp_code());
    }

    static token_type space_token(int len_)
    {
      assert(len_ > 0);
      return metashell::data::make_token<
          metashell::data::token_type::whitespace>(
          metashell::data::cpp_code(std::string(len_, ' ')));
    }

    static token_type new_line_token()
    {
      return metashell::data::make_token<metashell::data::token_type::new_line>(
          metashell::data::cpp_code("\n"));
    }

    static token_type change_value(string_type value_, token_type t_)
    {
      mpark::visit(
          [&value_](auto& token_) {
            token_.value() = metashell::data::cpp_code(std::move(value_));
          },
          t_);
      return t_;
    }

    static string_type value(const token_type& t_)
    {
      return metashell::data::value(t_).value();
    }
  };
}

#endif
