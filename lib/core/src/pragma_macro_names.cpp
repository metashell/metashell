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

#include <metashell/core/pragma_macro_names.hpp>
#include <metashell/core/wave_tokeniser.hpp>

#include <boost/algorithm/string/join.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace core
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
        for (auto tokeniser = create_wave_tokeniser(definitions_);
             tokeniser->has_further_tokens(); tokeniser->move_to_next_token())
        {
          const data::token token = tokeniser->current_token();
          if (token.type() == data::token_type::new_line)
          {
            st = state::start_line;
          }
          else if (token.category() != data::token_category::whitespace)
          {
            switch (st)
            {
            case state::ignore:
              break;
            case state::start_line:
              st = token.type() == data::token_type::p_define ?
                       state::was_define :
                       state::ignore;
              break;
            case state::was_define:
              if (token.type() == data::token_type::identifier)
              {
                names.push_back(token.value().value());
              }
              st = state::ignore;
              break;
            }
          }
        }

        return data::cpp_code(boost::algorithm::join(names, "\n"));
      }
    }

    pragma_macro_names::pragma_macro_names(shell& shell_) : _shell(shell_) {}

    std::string pragma_macro_names::description() const
    {
      return "Displays the names of the defined macros";
    }

    void pragma_macro_names::run(iface::displayer& displayer_) const
    {
      displayer_.show_cpp_code(extract_macro_names(
          _shell.engine().macro_discovery().macros(_shell.env())));
    }
  }
}
