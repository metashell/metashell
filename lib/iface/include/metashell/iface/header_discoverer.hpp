#ifndef METASHELL_IFACE_HEADER_DISCOVERER_HPP
#define METASHELL_IFACE_HEADER_DISCOVERER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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
#include <metashell/data/feature.hpp>
#include <metashell/data/include_type.hpp>
#include <metashell/data/standard_headers_allowed.hpp>

#include <boost/filesystem/path.hpp>

#include <set>
#include <string>
#include <vector>

namespace metashell
{
  namespace iface
  {
    class header_discoverer
    {
    public:
      virtual ~header_discoverer() {}

      virtual std::vector<boost::filesystem::path>
          include_path(data::include_type, data::standard_headers_allowed) = 0;

      virtual std::set<boost::filesystem::path>
      files_included_by(const data::cpp_code& exp_) = 0;

      static data::feature name_of_feature()
      {
        return data::feature::header_discoverer();
      }
    };
  } // namespace iface
} // namespace metashell

#endif
