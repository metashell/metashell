#ifndef METASHELL_EVENT_NAME_HPP
#define METASHELL_EVENT_NAME_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/metaprogram_node.hpp>
#include <metashell/data/none.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/type.hpp>

#include <boost/filesystem/path.hpp>

#include <variant.hpp>

namespace metashell
{
  namespace data
  {
    typedef mpark::
        variant<none_t, type, cpp_code, token, boost::filesystem::path>
            event_name;

    metaprogram_node to_metaprogram_node(const event_name& name_);
  } // namespace data
} // namespace metashell

#endif
