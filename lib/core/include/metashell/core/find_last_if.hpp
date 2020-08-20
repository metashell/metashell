#ifndef METASHELL_FIND_LAST_IF_HPP
#define METASHELL_FIND_LAST_IF_HPP

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

namespace metashell
{
  namespace core 
  {
    template <class ForwardIterator, class Pred>
    ForwardIterator
    find_last_if(ForwardIterator begin_, ForwardIterator end_, Pred pred_)
    {
      ForwardIterator last = end_;
      for (ForwardIterator i = begin_; i != end_; ++i)
      {
        if (pred_(*i))
        {
          last = i;
        }
      }
      return last;
    }
  }
}
#endif
