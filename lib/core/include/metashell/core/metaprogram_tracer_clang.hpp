#ifndef METASHELL_METAPROGRAM_TRACER_CLANG_HPP
#define METASHELL_METAPROGRAM_TRACER_CLANG_HPP

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

#include <metashell/iface/metaprogram_tracer.hpp>

#include <metashell/core/clang_binary.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  namespace core
  {
    class metaprogram_tracer_clang : public iface::metaprogram_tracer
    {
    public:
      explicit metaprogram_tracer_clang(clang_binary clang_binary_);

      virtual std::unique_ptr<iface::event_data_sequence>
      eval(iface::environment& env_,
           const boost::filesystem::path& temp_dir_,
           const boost::optional<data::cpp_code>& expression_,
           data::metaprogram_mode mode_,
           iface::displayer& displayer_) override;

    private:
      clang_binary _clang_binary;
    };
  }
}

#endif
