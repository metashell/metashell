#ifndef METASHELL_ENGINE_CLANG_YAML_TRACE_HPP
#define METASHELL_ENGINE_CLANG_YAML_TRACE_HPP

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
#include <metashell/data/event_data.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/type_or_code_or_error.hpp>

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>

#include <optional>
#include <string>
#include <vector>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      class yaml_trace
      {
      public:
        yaml_trace(const std::string& trace_,
                   data::type_or_code_or_error evaluation_result_,
                   data::cpp_code root_name_,
                   data::metaprogram_mode mode_);

        std::optional<data::event_data> next();

        const data::cpp_code& root_name() const;

        data::metaprogram_mode mode() const;

      private:
        std::vector<YAML::Node> _nodes;
        std::vector<YAML::Node>::const_iterator _next;
        std::optional<data::event_data> _evaluation_result;
        data::cpp_code _root_name;
        data::metaprogram_mode _mode;
      };
    }
  }
}

#endif
