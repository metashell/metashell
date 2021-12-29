#ifndef METASHELL_SYSTEM_TEST_ANY_OF_HPP
#define METASHELL_SYSTEM_TEST_ANY_OF_HPP

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

#include <algorithm>
#include <vector>

#include <iostream>

namespace metashell
{
  namespace system_test
  {
    template <class T>
    class any_of
    {
    public:
      template <class... Choices>
      any_of(Choices&&... choices_)
        : _choices{T{std::forward<Choices>(choices_)}...}
      {
      }

      const std::vector<T>& choices() const { return _choices; }

    private:
      std::vector<T> _choices;
    };

    template <class T>
    std::ostream& operator<<(std::ostream& out_, const any_of<T>& rhs_)
    {
      out_ << "any_of(";
      bool first = true;
      for (const T& choice : rhs_.choices())
      {
        if (first)
        {
          first = false;
        }
        else
        {
          out_ << ", ";
        }
        out_ << choice;
      }
      return out_ << ")";
    }

    template <class T>
    std::ostream& operator<<(std::ostream& out_,
                             const any_of<std::vector<T>>& rhs_)
    {
      out_ << "any_of(";
      bool first = true;
      for (const std::vector<T>& choice : rhs_.choices())
      {
        if (first)
        {
          first = false;
        }
        else
        {
          out_ << ", ";
        }
        out_ << "[";
        bool inner_first = true;
        for (const T& inner_choice : choice)
        {
          if (inner_first)
          {
            inner_first = false;
          }
          else
          {
            out_ << ", ";
          }
          out_ << inner_choice;
        }
        out_ << "]";
      }
      return out_ << ")";
    }

    template <class T, class U>
    bool operator==(const any_of<T>& lhs_, const U& rhs_)
    {
      return std::any_of(lhs_.choices().begin(), lhs_.choices().end(),
                         [&rhs_](const T& choice_) { return choice_ == rhs_; });
    }

    template <class T, class U>
    bool operator==(const U& lhs_, const any_of<T>& rhs_)
    {
      // Defining again instead of calling the previous overload to avoid
      // swapping the left and right hand side of the underlying == operator
      return std::any_of(rhs_.choices().begin(), rhs_.choices().end(),
                         [&lhs_](const T& choice_) { return lhs_ == choice_; });
    }

    template <class T, class U>
    bool operator!=(const any_of<T>& lhs_, const U& rhs_)
    {
      return !(lhs_ == rhs_);
    }

    template <class T, class U>
    bool operator!=(const U& lhs_, const any_of<T>& rhs_)
    {
      return !(lhs_ == rhs_);
    }
  } // namespace system_test
} // namespace metashell

#endif
