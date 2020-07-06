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

#include <metashell/data/code_completion.hpp>

namespace metashell
{
  namespace data
  {
    namespace
    {
      // Workaround until we can upgrade the compiler on OS X.
      template <class Set>
      decltype(Set{}.merge(Set{})) merge_maybe_missing_workaround(Set& this_,
                                                                  Set&& set_)
      {
        this_.merge(std::move(set_));
      }

      template <class Set>
      void merge_maybe_missing_workaround(Set& this_, const Set& set_)
      {
        this_.insert(set_.begin(), set_.end());
      }
    }

    code_completion::code_completion(std::initializer_list<user_input> vals_)
      : _completions{vals_}
    {
    }

    void code_completion::insert(user_input completion_)
    {
      _completions.insert(std::move(completion_));
    }

    void code_completion::insert(code_completion&& comp_)
    {
      merge_maybe_missing_workaround(
          _completions, std::move(comp_._completions));
    }

    std::optional<user_input> code_completion::pop()
    {
      std::optional<user_input> result;

      if (const auto i = _completions.begin(); i != _completions.end())
      {
        result = std::move(*i);
        _completions.erase(i);
      }

      return result;
    }

    code_completion::size_type code_completion::size() const
    {
      return _completions.size();
    }

    bool code_completion::empty() const { return _completions.empty(); }
  }
}
