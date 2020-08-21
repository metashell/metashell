// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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
#include <metashell/main_shell/process_pragma.hpp>

#include <metashell/core/find_last_if.hpp>

#include <metashell/data/exception.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/token_type.hpp>

namespace metashell
{
  namespace main_shell
  {
    void process_pragma(
        const std::map<data::pragma_name,
                       std::unique_ptr<iface::pragma_handler>>& handlers_,
        const data::command::iterator& begin_,
        const data::command::iterator& end_,
        iface::main_shell& shell_,
        iface::displayer& displayer_)
    {
      const data::command::iterator e =
          end_of_pragma_argument_list(begin_, end_);

      auto longest_fit_begin = e;
      const iface::pragma_handler* longest_fit_handler = 0;
      int longest_fit_len = -1;

      for (const auto& p : handlers_)
      {
        assert(p.second);

        if (const std::optional<data::command::iterator> i =
                is_this_pragma(p.first, begin_, e))
        {
          const auto len = p.first.tokens().size();
          if (longest_fit_len < int(len))
          {
            longest_fit_begin = *i;
            longest_fit_handler = p.second.get();
            longest_fit_len = len;
          }
        }
      }
      if (longest_fit_handler)
      {
        longest_fit_handler->run(
            begin_, core::find_last_if(begin_, longest_fit_begin,
                                 [](const data::token& token_) {
                                   return category(token_) !=
                                          data::token_category::whitespace;
                                 }) +
                        1,
            longest_fit_begin, e, shell_, displayer_);
      }
      else
      {
        throw data::exception(
            "Pragma " + to_string(data::pragma_name(begin_, e)) + " not found");
      }
    }
  }
}
