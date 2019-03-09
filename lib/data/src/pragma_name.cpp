// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/pragma_name.hpp>

#include <boost/algorithm/string/join.hpp>

namespace metashell
{
  namespace data
  {
    pragma_name::pragma_name(command::iterator begin_,
                             const command::iterator& end_)
    {
      for (; begin_ != end_ && can_be_part_of_name(*begin_);
           begin_ = skip_whitespace(skip(begin_), end_))
      {
        _tokens.push_back(begin_->value().value());
      }
    }

    const std::vector<std::string>& pragma_name::tokens() const
    {
      return _tokens;
    }

    bool operator<(const pragma_name& lhs_, const pragma_name& rhs_)
    {
      return lhs_.tokens() < rhs_.tokens();
    }

    bool operator==(const pragma_name& lhs_, const pragma_name& rhs_)
    {
      return lhs_.tokens() == rhs_.tokens();
    }

    std::string to_string(const pragma_name& n_)
    {
      return boost::algorithm::join(n_.tokens(), " ");
    }

    std::ostream& operator<<(std::ostream& out_, const pragma_name& n_)
    {
      return out_ << to_string(n_);
    }

    boost::optional<command::iterator>
    is_this_pragma(const pragma_name& name_,
                   command::iterator begin_,
                   const command::iterator& end_)
    {
      for (const auto& token : name_.tokens())
      {
        if (begin_ == end_ || !can_be_part_of_name(*begin_) ||
            token != begin_->value())
        {
          return boost::none;
        }
        else
        {
          begin_ = skip_whitespace(skip(begin_), end_);
        }
      }
      return boost::make_optional(begin_);
    }
  }
}
