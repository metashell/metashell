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

#include <metashell/main_shell/metashell_pragma.hpp>

#include <metashell/core/find_last_if.hpp>

#include <metashell/data/exception.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>

namespace metashell
{
  namespace main_shell
  {
    namespace
    {
      bool argument_token(const data::token& t_)
      {
        const data::token_category c = category(t_);
        return c != data::token_category::whitespace &&
               c != data::token_category::comment;
      }
    }

    std::optional<data::command::iterator>
    parse_pragma(const data::command& cmd_)
    {
      data::command::iterator i = skip_whitespace(cmd_.begin(), cmd_.end());

      if (i != cmd_.end() && (type_of(*i) == data::token_type::p_pragma ||
                              type_of(*i) == data::token_type::operator_pound))
      {
        i = skip_whitespace(skip(i), cmd_.end());

        if (i != cmd_.end() && type_of(*i) == data::token_type::identifier &&
            (value(*i) == "metashell" || value(*i) == "msh"))
        {
          i = skip_whitespace(skip(i), cmd_.end());
          if (i == cmd_.end() || value(*i).empty())
          {
            throw data::exception(
                "The name of the metashell pragma is missing.");
          }
          else if (type_of(*i) == data::token_type::identifier)
          {
            return i;
          }
          else
          {
            std::ostringstream s;
            s << "Invalid pragma name " << *i;
            throw data::exception(s.str());
          }
        }
      }

      return std::nullopt;
    }

    data::command::iterator
    end_of_pragma_argument_list(data::command::iterator begin_,
                                const data::command::iterator& end_)
    {
      data::command::iterator result =
          core::find_last_if(begin_, end_, main_shell::argument_token);

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
  }
}
