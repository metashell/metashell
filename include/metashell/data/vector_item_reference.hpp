#ifndef METASHELL_VECTOR_ITEM_REFERENCE_HPP
#define METASHELL_VECTOR_ITEM_REFERENCE_HPP

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

#include <cassert>
#include <vector>

namespace metashell
{
  namespace data
  {
    // It does not get invalidated, when the vector allocates a new buffer.
    template <class T, class Allocator = std::allocator<T>>
    class vector_item_reference
    {
    public:
      typedef std::vector<T, Allocator> vector_type;
      typedef typename vector_type::size_type size_type;
      typedef typename vector_type::value_type value_type;
      typedef typename vector_type::reference reference;
      typedef typename vector_type::pointer pointer;

      vector_item_reference(vector_type& vector_, size_type pos_)
        : _vector(&vector_), _pos(pos_)
      {
        assert(invariant());
      }

      size_type index() const { return _pos; }

      reference operator*() const
      {
        assert(invariant());
        return (*_vector)[_pos];
      }

      pointer operator->() const { return &operator*(); }
    private:
      vector_type* _vector;
      size_type _pos;

      bool invariant() const { return _pos < _vector->size(); }
    };

    template <class T, class Allocator>
    vector_item_reference<T, Allocator> back_of(std::vector<T, Allocator>& vec_)
    {
      return {vec_, vec_.size() - 1};
    }
  }
}

#endif
