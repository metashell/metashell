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

#include "cmd_t.hpp"

#include <metashell/replace_part/replace_part.hpp>

#include <sstream>

cmd_t::cmd_t(boost::filesystem::path path_, std::string marker_, callback func_)
  : _path(std::move(path_)),
    _marker(std::move(marker_)),
    _func(std::move(func_))
{
}

const boost::filesystem::path& cmd_t::path() const { return _path; }

std::string cmd_t::generate(const std::string& original_content_) const
{
  std::ostringstream replace_with;
  _func(replace_with);
  return replace_part::replace(original_content_, _marker, replace_with.str());
}
