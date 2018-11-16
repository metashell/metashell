#ifndef METASHELL_ENGINE_CONSTANT_BUILDER_HPP
#define METASHELL_ENGINE_CONSTANT_BUILDER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/engine.hpp>

#include <functional>
#include <memory>
#include <string>

namespace metashell
{
  namespace engine
  {
    namespace constant
    {
      std::function<std::unique_ptr<iface::engine>(const data::config&)>
      create_failing();

      std::function<std::unique_ptr<iface::engine>(const data::config&)>
      create_returning_type(const std::string& type_);

      std::function<std::unique_ptr<iface::engine>(const data::config&)>
      create_with_include_path(data::include_type type_,
                               std::vector<boost::filesystem::path> path_);
    }
  }
}

#endif
