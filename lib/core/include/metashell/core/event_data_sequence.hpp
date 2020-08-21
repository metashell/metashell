#ifndef METASHELL_EVENT_DATA_SEQUENCE_IMPL_HPP
#define METASHELL_EVENT_DATA_SEQUENCE_IMPL_HPP

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

#include <metashell/iface/event_data_sequence.hpp>

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/metaprogram_mode.hpp>

#include <memory>

namespace metashell
{
  namespace core
  {
    template <class Wrapped>
    class event_data_sequence : public iface::event_data_sequence
    {
    public:
      explicit event_data_sequence(Wrapped&& wrapped_)
        : _wrapped(std::move(wrapped_))
      {
      }

      virtual std::optional<data::event_data> next() override
      {
        return _wrapped.next();
      }

      virtual data::cpp_code root_name() const override
      {
        return _wrapped.root_name();
      }

      virtual data::metaprogram_mode mode() const override
      {
        return _wrapped.mode();
      }

    private:
      Wrapped _wrapped;
    };

    template <class Wrapped>
    std::unique_ptr<iface::event_data_sequence>
    make_event_data_sequence_ptr(Wrapped&& arg_)
    {
      return std::unique_ptr<event_data_sequence<Wrapped>>(
          new event_data_sequence<Wrapped>(std::move(arg_)));
    }
  }
}

#endif
