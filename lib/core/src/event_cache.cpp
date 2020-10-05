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

#include <metashell/core/event_cache.hpp>

namespace metashell
{
  namespace core
  {
    void event_cache::record(const data::event_data& event_)
    {
      mpark::visit([this](const auto& det) { this->record(det); }, event_);
    }

    void event_cache::record(
        const data::event_details<data::event_kind::template_instantiation>&
            event_)
    {
      if (!_recording_to.empty())
      {
        _recorded[_recording_to.back().first].emplace_back(event_);
      }
      _recording_to.push_back(std::make_pair(event_.what.full_name, 0));
    }

    void event_cache::record(
        const data::event_details<data::event_kind::template_end>& event_)
    {
      if (!_recording_to.empty())
      {
        if (_recording_to.back().second == 0)
        {
          _recording_to.pop_back();
          if (!_recording_to.empty())
          {
            _recorded[_recording_to.back().first].emplace_back(event_);
          }
        }
        else
        {
          _recorded[_recording_to.back().first].emplace_back(event_);
          --_recording_to.back().second;
        }
      }
    }

    data::list<data::event_data> event_cache::replay(data::event_data event_)
    {
      return mpark::visit(
          [this](auto& det) { return this->replay(std::move(det)); }, event_);
    }

    void event_cache::erase_related(const data::event_data& event_)
    {
      mpark::visit(
          [this](const auto& det) { this->erase_related(det); }, event_);
    }
  } // namespace core
} // namespace metashell
