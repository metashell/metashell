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

#include <metashell/command.hpp>

using namespace metashell;

command::command(const std::string& cmd_) :
  _cmd(cmd_)
{}

command::iterator command::begin() const
{
  return
    iterator(
      _cmd.begin(),
      _cmd.end(),
      iterator::value_type::position_type("<input>"),
      boost::wave::language_support(
        boost::wave::support_cpp
        | boost::wave::support_option_long_long
      )
    );
}

command::iterator command::end() const
{
  return iterator();
}

command::iterator metashell::skip(command::iterator i_)
{
  ++i_;
  return i_;
}

command::iterator metashell::skip_whitespace(command::iterator i_)
{
  return IS_CATEGORY(*i_, boost::wave::WhiteSpaceTokenType) ? skip(i_) : i_;
}

std::string metashell::tokens_to_string(
  command::iterator begin_,
  const command::iterator& end_
)
{
  std::ostringstream s;
  for (; begin_ != end_; ++begin_)
  {
    s << begin_->get_value();
  }
  return s.str();
}

