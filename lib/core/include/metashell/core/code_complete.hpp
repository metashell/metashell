#ifndef METASHELL_CORE_CODE_COMPLETE_HPP
#define METASHELL_CORE_CODE_COMPLETE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/header_discoverer.hpp>
#include <metashell/iface/pragma_handler.hpp>

#include <metashell/data/command.hpp>
#include <metashell/data/pragma_name.hpp>
#include <metashell/data/user_input.hpp>

#include <boost/filesystem/path.hpp>

#include <functional>
#include <map>
#include <vector>

namespace metashell
{
  namespace core
  {
    namespace code_complete
    {
      namespace impl
      {
        template <class T>
        data::user_input to_user_input(const T& value_)
        {
          return data::user_input{to_string(value_)};
        }

        data::user_input to_user_input(std::string);
      }

      data::code_completion
      files(const data::user_input& to_complete_,
            const data::user_input& prefix_,
            const std::function<bool(const boost::filesystem::path&)>&);

      data::code_completion include(data::command::const_iterator,
                                    data::command::const_iterator,
                                    iface::header_discoverer&);

      std::pair<data::code_completion,
                std::vector<std::pair<iface::pragma_handler*,
                                      data::command::const_iterator>>>
      pragma_metashell(data::command::const_iterator,
                       data::command::const_iterator,
                       const std::map<data::pragma_name,
                                      std::unique_ptr<iface::pragma_handler>>&);

      template <class InputIt>
      data::code_completion fixed_values(data::command::const_iterator begin_,
                                         data::command::const_iterator end_,
                                         InputIt values_begin_,
                                         InputIt values_end_)
      {
        data::code_completion result;

        if (begin_ == end_)
        {
          for (; values_begin_ != values_end_; ++values_begin_)
          {
            result.insert(data::user_input{" "} +
                          impl::to_user_input(*values_begin_));
          }
        }
        else
        {
          const data::user_input prefix{
              data::join_tokens(data::skip_whitespace(begin_, end_), end_)};
          for (; values_begin_ != values_end_; ++values_begin_)
          {
            const data::user_input value = impl::to_user_input(*values_begin_);
            if (starts_with(value, prefix))
            {
              result.insert(value.substr(prefix.size()));
            }
          }
        }

        return result;
      }

      template <class Container>
      data::code_completion fixed_values(data::command::const_iterator begin_,
                                         data::command::const_iterator end_,
                                         const Container& values_)
      {
        using std::begin;
        using std::end;
        return fixed_values(begin_, end_, begin(values_), end(values_));
      }
    }
  }
}

#endif
