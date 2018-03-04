#ifndef METASHELL_EVENT_DATA_SEQUENCE_HPP
#define METASHELL_EVENT_DATA_SEQUENCE_HPP

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

#include <metashell/data/event_data.hpp>

#include <boost/operators.hpp>
#include <boost/optional.hpp>

#include <cassert>

namespace metashell
{
  namespace data
  {
    template <class Derived>
    class event_data_sequence
    {
    public:
      class iterator : boost::input_iteratable<iterator, const event_data*>
      {
      public:
        iterator() : _container(nullptr), _current(boost::none) {}

        explicit iterator(Derived& container_)
          : _container(&container_), _current(container_.next())
        {
        }

        bool at_end() const { return !_current; }

        iterator& operator++()
        {
          assert(_container);
          assert(bool(_current));

          _current = _container->next();
          return *this;
        }

        const event_data& operator*() const
        {
          assert(_container);
          assert(bool(_current));

          return *_current;
        }

        friend bool operator==(const iterator& a_, const iterator& b_)
        {
          return a_.at_end() == b_.at_end();
        }

      private:
        Derived* _container;
        boost::optional<event_data> _current;
      };

      typedef iterator const_iterator;

      iterator begin() { return iterator(*static_cast<Derived*>(this)); }

      iterator end() const { return iterator(); }
    };
  }
}

#endif
