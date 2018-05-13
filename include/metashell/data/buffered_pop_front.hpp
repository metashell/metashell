#ifndef METASHELL_DATA_BUFFERED_POP_FRONT_HPP
#define METASHELL_DATA_BUFFERED_POP_FRONT_HPP

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

namespace metashell
{
  namespace data
  {
    template <class T>
    class buffered_pop_front
    {
    public:
      explicit buffered_pop_front(T& container_)
        : _container(&container_), _count(0)
      {
      }

      void buffer_pop_front() { ++_count; }

      void flush()
      {
        for (int i = 0; i < _count; ++i)
        {
          _container->pop_front();
        }
        _count = 0;
      }

    private:
      T* _container;
      int _count;
    };
  }
}

#endif
