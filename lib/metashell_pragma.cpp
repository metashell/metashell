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

  bool argument_token(const token& t_)
  {
    const token_category c = t_.category();
    return c != token_category::whitespace && c != token_category::comment;
  }
}

boost::optional<command::iterator> metashell::parse_pragma(const command& cmd_)
{
  command::iterator i = skip_whitespace(cmd_.begin(), cmd_.end());

  if (
    i != cmd_.end()
    && (
      i->type() == token_type::p_pragma
      || i->type() == token_type::operator_pound
    )
  )
  {
    i = skip_whitespace(skip(i), cmd_.end());

    if (
      i != cmd_.end()
      && i->type() == token_type::identifier
      && (i->value() == "metashell" || i->value() == "msh")
    )
    {
      i = skip_whitespace(skip(i), cmd_.end());
      if (i == cmd_.end() || i->value().empty())
      {
        throw exception("The name of the metashell pragma is missing.");
      }
      else if (i->type() == token_type::identifier)
      {
        return i;
      }
      else
      {
        std::ostringstream s;
        s << "Invalid pragma name " << i->value();
        throw exception(s.str());
      }
    }
  }

  return boost::optional<command::iterator>();
}

command::iterator metashell::end_of_pragma_argument_list(
  command::iterator begin_,
  const command::iterator& end_
)
{
  command::iterator result = find_last_if(begin_, end_, argument_token);

  if (result == end_)
  {
    return begin_;
  }
  else
  {
    ++result;
    return result;
  }
}

