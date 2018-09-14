#ifndef METASHELL_PREPROCESSOR_SHELL_WAVE_HPP
#define METASHELL_PREPROCESSOR_SHELL_WAVE_HPP

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

#include <metashell/iface/preprocessor_shell.hpp>

#include <metashell/data/wave_config.hpp>

namespace metashell
{
  namespace core
  {
    class preprocessor_shell_wave : public iface::preprocessor_shell
    {
    public:
      explicit preprocessor_shell_wave(data::wave_config config_);

      virtual data::result precompile(const data::cpp_code& exp_) override;

    private:
      data::wave_config _config;
    };
  }
}

#endif
