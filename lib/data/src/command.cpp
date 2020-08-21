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

#include <algorithm>

namespace metashell
{
  namespace data
  {
    command::command(std::vector<token> tokens_) : _tokens(std::move(tokens_))
    {
    }

    command::iterator command::begin() const { return _tokens.begin(); }

    command::iterator command::end() const { return _tokens.end(); }

    command::iterator skip(command::iterator i_)
    {
      ++i_;
      return i_;
    }

    bool command::empty() const { return _tokens.empty(); }

    int command::size() const { return _tokens.size(); }

    command::iterator skip_whitespace(command::iterator begin_,
                                      const command::iterator& end_)
    {
      return (begin_ != end_ && whitespace_or_comment(category(*begin_))) ?
                 skip(begin_) :
                 begin_;
    }

    command::iterator skip_all_whitespace(const command::iterator& begin_,
                                          const command::iterator& end_)
    {
      return std::find_if(begin_, end_, [](const token& token_) {
        return !whitespace_or_comment(category(token_));
      });
    }

    cpp_code tokens_to_string(command::iterator begin_,
                              const command::iterator& end_)
    {
      std::ostringstream s;
      for (; begin_ != end_; ++begin_)
      {
        s << value(*begin_);
      }
      return cpp_code(s.str());
    }
  }
}
