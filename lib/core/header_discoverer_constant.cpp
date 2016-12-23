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

#include <metashell/header_discoverer_constant.hpp>

namespace metashell
{
  header_discoverer_constant::header_discoverer_constant(
      std::vector<boost::filesystem::path> sysincludes_,
      std::vector<boost::filesystem::path> quoteincludes_)
    : _sysincludes(move(sysincludes_)), _quoteincludes(move(quoteincludes_))
  {
  }

  std::vector<boost::filesystem::path>
  header_discoverer_constant::include_path(data::include_type type_)
  {
    switch (type_)
    {
    case data::include_type::sys:
      return _sysincludes;
    case data::include_type::quote:
      return _quoteincludes;
    }
    // avoid control reaches end of non-void function warnings on
    // some compilers
    return {};
  }

  std::set<boost::filesystem::path>
  header_discoverer_constant::files_included_by(const std::string&)
  {
    return std::set<boost::filesystem::path>();
  }
}
