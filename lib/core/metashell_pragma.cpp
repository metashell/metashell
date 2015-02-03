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

#include <metashell/exception.hpp>

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

  bool argument_token(const data::token& t_)
  {
    const data::token_category c = t_.category();
    return
      c != data::token_category::whitespace
      && c != data::token_category::comment;
  }
}

boost::optional<data::command::iterator> metashell::parse_pragma(
  const data::command& cmd_
)
{
  data::command::iterator i = skip_whitespace(cmd_.begin(), cmd_.end());

  if (
    i != cmd_.end()
    && (
      i->type() == data::token_type::p_pragma
      || i->type() == data::token_type::operator_pound
    )
  )
  {
    i = skip_whitespace(skip(i), cmd_.end());

    if (
      i != cmd_.end()
      && i->type() == data::token_type::identifier
      && (i->value() == "metashell" || i->value() == "msh")
    )
    {
      i = skip_whitespace(skip(i), cmd_.end());
      if (i == cmd_.end() || i->value().empty())
      {
        throw exception("The name of the metashell pragma is missing.");
      }
      else if (i->type() == data::token_type::identifier)
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

  return boost::none;
}

data::command::iterator metashell::end_of_pragma_argument_list(
  data::command::iterator begin_,
  const data::command::iterator& end_
)
{
  data::command::iterator result = find_last_if(begin_, end_, argument_token);

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

