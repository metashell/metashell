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

namespace metashell
{
  namespace data
  {
    event_data template_begin(event_kind kind,
                              const type& type,
                              const file_location& point_of_event,
                              const file_location& source_location,
                              double timestamp)
    {
      switch (kind)
      {
      case event_kind::template_instantiation:
        return event_details<event_kind::template_instantiation>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::default_template_argument_instantiation:
        return event_details<
            event_kind::default_template_argument_instantiation>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::default_function_argument_instantiation:
        return event_details<
            event_kind::default_function_argument_instantiation>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::explicit_template_argument_substitution:
        return event_details<
            event_kind::explicit_template_argument_substitution>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::deduced_template_argument_substitution:
        return event_details<
            event_kind::deduced_template_argument_substitution>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::prior_template_argument_substitution:
        return event_details<event_kind::prior_template_argument_substitution>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::default_template_argument_checking:
        return event_details<event_kind::default_template_argument_checking>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::exception_spec_instantiation:
        return event_details<event_kind::exception_spec_instantiation>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::declaring_special_member:
        return event_details<event_kind::declaring_special_member>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::defining_synthesized_function:
        return event_details<event_kind::defining_synthesized_function>{
            type, point_of_event, source_location, timestamp};
        break;
      case event_kind::memoization:
        return event_details<event_kind::memoization>{
            type, point_of_event, source_location, timestamp};
        break;
      default:
        assert(!"Invalid event_kind");
        break;
      }
    }
  }
}
