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

#include <metashell/engine/constant/builder.hpp>

#include <metashell/engine/constant/code_completer.hpp>
#include <metashell/engine/constant/cpp_validator.hpp>
#include <metashell/engine/constant/header_discoverer.hpp>
#include <metashell/engine/constant/macro_discovery.hpp>
#include <metashell/engine/constant/metaprogram_tracer.hpp>
#include <metashell/engine/constant/preprocessor_shell.hpp>
#include <metashell/engine/constant/preprocessor_tracer.hpp>
#include <metashell/engine/constant/type_shell.hpp>

#include <metashell/core/engine.hpp>

namespace metashell
{
  namespace engine
  {
    namespace constant
    {
      namespace
      {
        std::vector<data::feature> supported_features()
        {
          return {data::feature::type_shell(),
                  data::feature::preprocessor_shell(),
                  data::feature::code_completer(),
                  data::feature::header_discoverer(),
                  data::feature::metaprogram_tracer(),
                  data::feature::cpp_validator(),
                  data::feature::macro_discovery(),
                  data::feature::preprocessor_tracer()};
        }

        header_discoverer create_header_discoverer_with_include_path(
            data::include_type type_,
            std::vector<boost::filesystem::path> path_)
        {
          const std::vector<boost::filesystem::path> empty;

          switch (type_)
          {
          case data::include_type::sys:
            return header_discoverer(move(path_), empty);
          case data::include_type::quote:
            return header_discoverer(empty, move(path_));
          }
          // avoid control reaches end of non-void function warnings on
          // some compilers
          return header_discoverer(empty, empty);
        }
      }

      std::function<std::unique_ptr<iface::engine>(const data::config&)>
      create_failing()
      {
        return [](const data::config&) {
          const data::result result(false, "", "Using failing engine", "");
          const std::vector<boost::filesystem::path> empty;

          return core::make_engine(
              data::engine_name("failing"), type_shell(result),
              preprocessor_shell(result), code_completer(),
              header_discoverer(empty, empty), metaprogram_tracer(),
              cpp_validator(result), macro_discovery(), preprocessor_tracer(),
              supported_features());
        };
      }

      std::function<std::unique_ptr<iface::engine>(const data::config&)>
      create_returning_type(const std::string& type_)
      {
        return [type_](const data::config&) {
          const data::result result(true, type_, "", "");
          const std::vector<boost::filesystem::path> empty;

          return core::make_engine(
              data::engine_name("type_returning"), type_shell(result),
              preprocessor_shell(result), code_completer(),
              header_discoverer(empty, empty), metaprogram_tracer(),
              cpp_validator(result), macro_discovery(), preprocessor_tracer(),
              supported_features());
        };
      }

      std::function<std::unique_ptr<iface::engine>(const data::config&)>
      create_with_include_path(data::include_type type_,
                               std::vector<boost::filesystem::path> path_)
      {
        return [type_, path_](const data::config&) {
          const data::result result(true, "int", "", "");

          return core::make_engine(
              data::engine_name("engine_with_include_path"), type_shell(result),
              preprocessor_shell(result), code_completer(),
              create_header_discoverer_with_include_path(type_, path_),
              metaprogram_tracer(), cpp_validator(result), macro_discovery(),
              preprocessor_tracer(), supported_features());
        };
      }
    }
  }
}
