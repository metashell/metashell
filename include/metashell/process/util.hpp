#ifndef METASHELL_PROCESS_UTIL_HPP
#define METASHELL_PROCESS_UTIL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <algorithm>
#include <string>

namespace metashell
{
  namespace process
  {
    std::string quote_argument(std::string arg_);

    template <class InputIt, class OutputIt>
    void quote_arguments(InputIt begin_, InputIt end_, OutputIt obegin_)
    {
      std::transform(begin_, end_, obegin_, quote_argument);
    }

    template <class ForwardIt>
    void quote_arguments(ForwardIt begin_, ForwardIt end_)
    {
      for (; begin_ != end_; ++begin_)
      {
        *begin_ = quote_argument(std::move(*begin_));
      }
    }

    template <class Container>
    void quote_arguments(Container& c_)
    {
      quote_arguments(c_.begin(), c_.end());
    }
  }
}

#endif
