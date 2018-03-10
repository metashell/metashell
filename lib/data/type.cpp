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

#include <metashell/boost/regex.hpp>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <ostream>
#include <sstream>

namespace metashell
{
  namespace data
  {

    type::type() {}

    type::type(const std::string& name_) : _name(name_) {}

    type::type(const cpp_code& name_) : _name(name_) {}

    const cpp_code& type::name() const { return _name; }

    bool type::is_integral_constant(const type& type_,
                                    const std::string& value_) const
    {
      using boost::regex;
      using boost::regex_match;

      std::ostringstream s;
      s << "std::(.*::|)integral_constant<" << type_ << ", " << value_ << ">";

      return regex_match(name().value(), regex(s.str()));
    }

    type::operator cpp_code() const { return cpp_code(_name); }

    type::const_iterator type::begin() const { return _name.begin(); }

    type::const_iterator type::end() const { return _name.end(); }

    std::ostream& operator<<(std::ostream& o_, const type& t_)
    {
      return o_ << t_.name();
    }

    bool operator==(const type& a_, const type& b_)
    {
      return a_.name() == b_.name();
    }

    bool operator<(const type& a_, const type& b_)
    {
      return a_.name() < b_.name();
    }

    boost::optional<type> trim_wrap_type(const type& type_)
    {
      const std::string wrap_prefix = "metashell::impl::wrap<";
      const std::string wrap_suffix = ">";

      // TODO this check could be made more strict,
      // since we know whats inside wrap<...> (mp->get_evaluation_result)
      if (boost::starts_with(type_, wrap_prefix) &&
          boost::ends_with(type_, wrap_suffix))
      {
        return type(boost::trim_copy(type_.name().substr(
            wrap_prefix.size(),
            type_.name().size() - wrap_prefix.size() - wrap_suffix.size())));
      }
      else
      {
        return boost::none;
      }
    }

    bool is_template_type(const type& type_)
    {
      const command cmd(type_);
      return std::find_if(cmd.begin(), cmd.end(), [](const token& t_) {
               return t_.type() == token_type::operator_greater ||
                      t_.type() == token_type::operator_less;
             }) != cmd.end();
    }
  }
} // namespace metashell:data
