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

#include <metashell/data/token_type.hpp>
#include <metashell/data/token_category.hpp>

#include <string>
#include <sstream>
#include <cassert>

namespace metashell
{
  namespace data
  {
    class token
    {
    public:
      token();
      token(std::string value_, token_type type_);

      token_category category() const;
      const std::string& value() const;
      token_type type() const;
    private:
      token_type _type;
      std::string _value;
    };

    template <class TokenIt>
    std::string tokens_to_string(TokenIt begin_, const TokenIt& end_)
    {
      std::ostringstream s;
      while (begin_ != end_)
      {
        s << begin_->value();
        ++begin_;
      }
      return s.str();
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
      return
        t_.category() == metashell::data::token_category::operator_token
        && t_.value() == "::";
    }

    static bool is_c_comment(const token_type& t_)
    {
      return
        t_.category() == metashell::data::token_category::comment
        && t_.value().size() >= 4
        && t_.value()[0] == '/'
        && t_.value()[1] == '*';
    }

    static bool is_less(const token_type& t_)
    {
      return
        t_.category() == metashell::data::token_category::operator_token
        && t_.value() == "<";
    }

    static bool is_greater(const token_type& t_)
    {
      return
        t_.category() == metashell::data::token_category::operator_token
        && t_.value() == ">";
    }

    static bool is_comma(const token_type& t_)
    {
      return
        t_.category() == metashell::data::token_category::operator_token
        && t_.value() == ",";
    }

    static bool is_space(const token_type& t_)
    {
      return
        t_.category() == metashell::data::token_category::unknown ||
        t_.category() == metashell::data::token_category::whitespace;
    }

    static token_type empty_token()
    {
      return
        token_type("", metashell::data::token_type::unknown);
    }

    static token_type space_token(int len_)
    {
      using std::string;

      assert(len_ > 0);
      return
        token_type(string(len_, ' '), metashell::data::token_type::whitespace);
    }

    static token_type new_line_token()
    {
      return token_type("\n", metashell::data::token_type::new_line);
    }

    static token_type change_value(
      const string_type& value_,
      const token_type& t_
    )
    {
      return token_type(value_, t_.type());
    }

    static string_type value(const token_type& t_)
    {
      return t_.value();
    }
  };
}

#endif

