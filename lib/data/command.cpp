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
#include <metashell/wave_tokeniser.hpp>

using namespace metashell::data;

namespace
{
  bool whitespace_or_comment(token_category c_)
  {
    return c_ == token_category::whitespace || c_ == token_category::comment;
  }
}

command::command(const std::string& cmd_) : _cmd(cmd_), _tokens()
{
  for (auto t = create_wave_tokeniser(cmd_, "<command>");
       t->has_further_tokens(); t->move_to_next_token())
  {
    _tokens.push_back(t->current_token());
  }
}

command::iterator command::begin() const { return _tokens.begin(); }

command::iterator command::end() const { return _tokens.end(); }

command::iterator metashell::data::skip(command::iterator i_)
{
  ++i_;
  return i_;
}

command::iterator
metashell::data::skip_whitespace(command::iterator begin_,
                                 const command::iterator& end_)
{
  return (begin_ != end_ && whitespace_or_comment(begin_->category())) ?
             skip(begin_) :
             begin_;
}

std::string metashell::data::tokens_to_string(command::iterator begin_,
                                              const command::iterator& end_)
{
  std::ostringstream s;
  for (; begin_ != end_; ++begin_)
  {
    s << begin_->value();
  }
  return s.str();
}
