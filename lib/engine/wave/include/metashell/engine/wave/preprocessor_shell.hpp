#ifndef METASHELL_ENGINE_WAVE_PREPROCESSOR_SHELL_HPP
#define METASHELL_ENGINE_WAVE_PREPROCESSOR_SHELL_HPP

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
  namespace engine
  {
    namespace wave
    {
      class preprocessor_shell : public iface::preprocessor_shell
      {
      public:
        preprocessor_shell(
            data::wave_config,
            std::vector<boost::filesystem::path> system_includes_);

        virtual data::result precompile(const data::cpp_code& exp_) override;

      private:
        data::wave_config _config;
        std::vector<boost::filesystem::path> _system_includes;
      };
    } // namespace wave
  } // namespace engine
} // namespace metashell

#endif
