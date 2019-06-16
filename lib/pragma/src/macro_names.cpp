// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/pragma/macro_names.hpp>

#include <metashell/core/wave_tokeniser.hpp>

#include <boost/algorithm/string/join.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace pragma
  {
    namespace
    {
      enum class state
      {
        ignore,
        start_line,
        was_define
      };

      data::cpp_code extract_macro_names(const data::cpp_code& definitions_)
      {
        std::vector<std::string> names;
        state st = state::start_line;
        for (auto tokeniser = core::create_wave_tokeniser(definitions_);
             tokeniser->has_further_tokens(); tokeniser->move_to_next_token())
        {
          const data::token token = tokeniser->current_token();
          if (type_of(token) == data::token_type::new_line)
          {
            st = state::start_line;
          }
          else if (category(token) != data::token_category::whitespace)
          {
            switch (st)
            {
            case state::ignore:
              break;
            case state::start_line:
              st = type_of(token) == data::token_type::p_define ?
                       state::was_define :
                       state::ignore;
              break;
            case state::was_define:
              if (type_of(token) == data::token_type::identifier)
              {
                names.push_back(value(token).value());
              }
              st = state::ignore;
              break;
            }
          }
        }

        return data::cpp_code(boost::algorithm::join(names, "\n"));
      }
    }

    std::string macro_names::description() const
    {
      return "Displays the names of the defined macros";
    }

    void macro_names::run(iface::shell& shell_,
                          iface::displayer& displayer_) const
    {
      displayer_.show_cpp_code(extract_macro_names(
          shell_.engine().macro_discovery().macros(shell_.env())));
    }
  }
}
