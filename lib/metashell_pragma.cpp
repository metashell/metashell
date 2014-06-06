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

#include <metashell/metashell_pragma.hpp>

#include "exception.hpp"

#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>

using namespace metashell;

namespace
{
  token_iterator skip(token_iterator i_)
  {
    ++i_;
    return i_;
  }

  token_iterator skip_whitespace(token_iterator i_)
  {
    const token_iterator end;
    assert(i_ != end);
    if (IS_CATEGORY(*i_, boost::wave::WhiteSpaceTokenType))
    {
      ++i_;
    }
    assert(i_ != end);
    return i_;
  }

  bool argument_token(const token_iterator::token_type& t_)
  {
    return 
      !(
        IS_CATEGORY(t_, boost::wave::WhiteSpaceTokenType)
        || IS_CATEGORY(t_, boost::wave::EOLTokenType)
        || IS_CATEGORY(t_, boost::wave::EOFTokenType)
      );
  }

  token_iterator end_of_argument_list(token_iterator begin_)
  {
    const token_iterator end;

    token_iterator result = begin_;
    bool found = false;
    for (; begin_ != end; ++begin_)
    {
      if (argument_token(*begin_))
      {
        result = begin_;
        found = true;
      }
    }

    if (found)
    {
      ++result;
    }
    return result;
  }
}

boost::optional<metashell_pragma> metashell_pragma::parse(const std::string& s_)
{
  const token_iterator end;
  token_iterator i = skip_whitespace(begin_tokens(s_, "<string>"));

  if (*i == boost::wave::T_PP_PRAGMA)
  {
    i = skip_whitespace(skip(i));

    if (*i == boost::wave::T_IDENTIFIER && i->get_value() == "metashell")
    {
      return metashell_pragma(s_);
    }
  }

  return boost::optional<metashell_pragma>();
}

metashell_pragma::metashell_pragma(const std::string& line_) :
  _line(line_)
{
  const token_iterator end;
  token_iterator i = skip_whitespace(begin_tokens(_line, "<string>"));

  assert(*i == boost::wave::T_PP_PRAGMA);
  i = skip_whitespace(skip(i));
  assert(*i == boost::wave::T_IDENTIFIER);
  assert(i->get_value() == "metashell");
  i = skip_whitespace(skip(i));

  if (*i == boost::wave::T_IDENTIFIER)
  {
    _name = i;
    _begin = skip_whitespace(skip(i));
    _end = end_of_argument_list(_begin);
  }
  else
  {
    const token_iterator::token_type::string_type v = i->get_value();
    if (v.empty())
    {
      throw exception("The identifier after the metashell keyword is missing.");
    }
    else
    {
      std::ostringstream s;
      s
        << "Metashell pragmas have to be identifiers. " << i->get_value()
        << " is not an identifier.";
      throw exception(s.str());
    }
  }
}

std::string metashell_pragma::name() const
{
  const token_iterator::token_type::string_type s = _name->get_value();

  return std::string(s.begin(), s.end());
}

token_iterator metashell_pragma::begin() const
{
  return _begin;
}

token_iterator metashell_pragma::end() const
{
  return _end;
}


