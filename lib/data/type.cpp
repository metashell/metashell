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

#include <metashell/data/type.hpp>

#include <boost/regex.hpp>

#include <ostream>
#include <sstream>

namespace metashell {
namespace data {

type::type() {}

type::type(const std::string& name_) :
  _name(name_)
{}

const std::string& type::name() const
{
  return _name;
}

bool type::is_integral_constant(
  const type& type_,
  const std::string& value_
) const
{
  using boost::regex;
  using boost::regex_match;

  std::ostringstream s;
  s << "std::(.*::|)integral_constant<" << type_ << ", " << value_ << ">";

  return regex_match(name(), regex(s.str()));
}

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

}} // namespace metashell:data
