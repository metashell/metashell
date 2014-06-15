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
  template <class It, class P>
  It find_last_if(It begin_, It end_, P pred_)
  {
    It result = end_;
    for (; begin_ != end_; ++begin_)
    {
      if (pred_(*begin_))
      {
        result = begin_;
      }
    }
    return result;
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
}

boost::optional<token_iterator> metashell::parse_pragma(const std::string& s_)
{
  const token_iterator end;
  token_iterator i = skip_whitespace(begin_tokens(s_, "<string>"));

  if (i != end && *i == boost::wave::T_PP_PRAGMA)
  {
    i = skip_whitespace(skip(i));

    if (
      i != end
      && *i == boost::wave::T_IDENTIFIER
      && i->get_value() == "metashell"
    )
    {
      i = skip_whitespace(skip(i));
      if (i == end || i->get_value().empty())
      {
        throw exception("The name of the metashell pragma is missing.");
      }
      else if (*i == boost::wave::T_IDENTIFIER)
      {
        return i;
      }
      else
      {
        std::ostringstream s;
        s << "Invalid pragma name " << i->get_value();
        throw exception(s.str());
      }
    }
  }

  return boost::optional<token_iterator>();
}

token_iterator metashell::end_of_pragma_argument_list(token_iterator begin_)
{
  const token_iterator end;

  token_iterator result = find_last_if(begin_, end, argument_token);

  if (result == end)
  {
    return begin_;
  }
  else
  {
    ++result;
    return result;
  }
}

