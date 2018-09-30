#ifndef METASHELL_CPP_VALIDATOR_WAVE_HPP
#define METASHELL_CPP_VALIDATOR_WAVE_HPP

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

#include <metashell/iface/cpp_validator.hpp>

#include <metashell/core/preprocessor_shell_wave.hpp>

namespace metashell
{
  namespace core
  {
    class cpp_validator_wave : public iface::cpp_validator
    {
    public:
      explicit cpp_validator_wave(data::wave_config config_);

      virtual data::result
      validate_code(const data::cpp_code& src_,
                    const data::config& config_,
                    const iface::environment& env_,
                    bool use_precompiled_headers_) override;

    private:
      preprocessor_shell_wave _preprocessor;
    };
  }
}

#endif
