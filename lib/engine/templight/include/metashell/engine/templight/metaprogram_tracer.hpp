#ifndef METASHELL_ENGINE_TEMPLIGHT_METAPROGRAM_TRACER_HPP
#define METASHELL_ENGINE_TEMPLIGHT_METAPROGRAM_TRACER_HPP

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

#include <metashell/iface/metaprogram_tracer.hpp>

#include <metashell/engine/clang/binary.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  namespace engine
  {
    namespace templight
    {
      class metaprogram_tracer : public iface::metaprogram_tracer
      {
      public:
        explicit metaprogram_tracer(clang::binary templight_binary_);

        virtual std::unique_ptr<iface::event_data_sequence>
        eval(iface::environment& env_,
             const boost::filesystem::path& temp_dir_,
             const std::optional<data::cpp_code>& expression_,
             data::metaprogram_mode mode_,
             iface::displayer& displayer_) override;

      private:
        clang::binary _templight_binary;
      };
    }
  }
}

#endif
