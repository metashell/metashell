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

#include <metashell/data/command.hpp>
#include <metashell/data/type.hpp>

#include <regex>

namespace metashell
{
  namespace data
  {
    const cpp_code& type::name() const { return value(); }

    bool type::is_integral_constant(const type& type_,
                                    const std::string& value_) const
    {
      return std::regex_match(
          name().value(), std::regex("std::(.*::|)integral_constant<" +
                                     to_string(type_) + ", " + value_ + ">"));
    }

    type::operator cpp_code() const { return name(); }

    std::optional<type> trim_wrap_type(const type& type_)
    {
      const type wrap_prefix("metashell::impl::wrap<");
      const type wrap_suffix(">");

      // TODO this check could be made more strict,
      // since we know whats inside wrap<...> (mp->get_evaluation_result)
      if (starts_with(type_, wrap_prefix) && ends_with(type_, wrap_suffix))
      {
        return type(trim_copy(type_.name().substr(
            wrap_prefix.size(),
            type_.name().size() - wrap_prefix.size() - wrap_suffix.size())));
      }
      else
      {
        return std::nullopt;
      }
    }

    bool is_remove_ptr(const type& type_)
    {
      return type_ == type("metashell::impl::remove_ptr");
    }
  }
} // namespace metashell:data
