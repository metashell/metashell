#ifndef METASHELL_EVENT_CACHE_HPP
#define METASHELL_EVENT_CACHE_HPP

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
#include <metashell/data/list.hpp>
#include <metashell/data/type.hpp>

#include <deque>
#include <map>
#include <type_traits>
#include <vector>

namespace metashell
{
  namespace core
  {
    class event_cache
    {
    public:
      void record(const data::event_data& event_);
      data::list<data::event_data> replay(data::event_data event_);
      void erase_related(const data::event_data& event_);

    private:
      std::map<data::type, std::vector<data::event_data>> _recorded;
      std::vector<std::pair<data::type, int>> _recording_to;
      std::vector<data::event_data> _empty;

      static constexpr bool recordable(data::event_kind kind_)
      {
        return kind_ == data::event_kind::template_instantiation ||
               kind_ == data::event_kind::memoization;
      }

      template <data::event_kind Kind>
      void record(const data::event_details<Kind>& event_)
      {
        if (!_recording_to.empty())
        {
          switch (relative_depth_of(Kind))
          {
          case data::relative_depth::open:
            ++_recording_to.back().second;
            break;
          case data::relative_depth::flat:
            break;
          case data::relative_depth::close:
          case data::relative_depth::end:
            --_recording_to.back().second;
            break;
          }

          _recorded[_recording_to.back().first].emplace_back(event_);
        }
      }

      void record(const data::event_details<
                  data::event_kind::template_instantiation>& event_);

      void
      record(const data::event_details<data::event_kind::template_end>& event_);

      template <data::event_kind Kind>
      typename std::enable_if<!recordable(Kind),
                              data::list<data::event_data>>::type
      replay(data::event_details<Kind> event_)
      {
        return {data::event_data(event_), _empty};
      }

      template <data::event_kind Kind>
      typename std::enable_if<recordable(Kind),
                              data::list<data::event_data>>::type
      replay(data::event_details<Kind> event_)
      {
        const auto i = _recorded.find(event_.what.full_name);
        if (i != _recorded.end() && !i->second.empty())
        {
          // The template_end event closing the memoization will become the
          // template_end event closing the simulated template_instantiation
          return {
              data::event_details<data::event_kind::template_instantiation>{
                  data::timeless_event_details<
                      data::event_kind::template_instantiation>(event_.what),
                  event_.timestamp},
              i->second};
        }
        else
        {
          return {data::event_data(event_), _empty};
        }
      }

      template <data::event_kind Kind>
      typename std::enable_if<!recordable(Kind)>::type
      erase_related(const data::event_details<Kind>&)
      {
      }

      template <data::event_kind Kind>
      typename std::enable_if<recordable(Kind)>::type
      erase_related(const data::event_details<Kind>& event_)
      {
        _recorded.erase(event_.what.full_name);
      }
    };
  }
}

#endif
