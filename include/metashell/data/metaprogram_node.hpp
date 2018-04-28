#ifndef METASHELL_DATA_METAPROGRAM_NODE_HPP
#define METASHELL_DATA_METAPROGRAM_NODE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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
#include <metashell/data/token.hpp>
#include <metashell/data/type.hpp>

#include <metashell/unique.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/variant.hpp>

namespace metashell
{
  namespace data
  {
    typedef boost::variant<type,
                           unique<type>,
                           unique<token>,
                           unique<cpp_code>,
                           unique<boost::filesystem::path>>
        metaprogram_node;
  }
}

#endif
