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

#include <metashell/data/token.hpp>

#include <cassert>

namespace metashell
{
  namespace data
  {
    namespace
    {
      struct category_visitor
      {
        template <token_category Category>
        token_category operator()(const token_<Category>&) const
        {
          return Category;
        }
      };
    }

    token make_token(cpp_code value_, token_type type_)
    {
      switch (type_)
      {
#ifdef METASHELL_TOKEN_TYPE
#error METASHELL_TOKEN_TYPE already defined
#endif
#define METASHELL_TOKEN_TYPE(name, cat, format)                                \
  case token_type::name:                                                       \
    return make_token<token_type::name>(std::move(value_));

#include <metashell/data/impl/token_type_list.hpp>

#undef METASHELL_TOKEN_TYPE
      }
      assert(!"Invalid token type");
      return token();
    }

    token_type type_of(const token& t_)
    {
      return mpark::visit([](const auto& tok_) { return tok_.type(); }, t_);
    }

    const cpp_code& value(const token& t_)
    {
      return mpark::visit(
          [](const auto& tok_) -> const cpp_code& { return tok_.value(); }, t_);
    }

    token_category category(const token& t_)
    {
      return mpark::visit(category_visitor(), t_);
    }

    std::string string_literal_value(const token& token_)
    {
      assert(type_of(token_) == token_type::string_literal);

      std::string val = value(token_).value();
      if (!val.empty() && val[0] == '"')
      {
        val.erase(0, 1);
      }
      if (!val.empty() && val[val.size() - 1] == '"')
      {
        val.erase(val.size() - 1);
      }
      return val;
    }

    std::ostream& operator<<(std::ostream& out_, const token& t_)
    {
      return out_ << "token(\"" << value(t_) << "\", " << type_of(t_) << ")";
    }

    std::string format_token(const token& t_)
    {
      switch (type_of(t_))
      {
#ifdef TOKEN_VALUE
#error TOKEN_VALUE already defined
#endif
#define TOKEN_VALUE value(t_).value()

#ifdef METASHELL_TOKEN_TYPE
#error METASHELL_TOKEN_TYPE already defined
#endif
#define METASHELL_TOKEN_TYPE(name, cat, format)                                \
  case token_type::name:                                                       \
    return format;

#include <metashell/data/impl/token_type_list.hpp>

#undef METASHELL_TOKEN_TYPE
#undef TOKEN_VALUE
      }
      assert(false);
      return std::string();
    }

    bool include_quote_token(const token& token_)
    {
      return type_of(token_) == token_type::unknown && value(token_) == "\"";
    }
  }
}
