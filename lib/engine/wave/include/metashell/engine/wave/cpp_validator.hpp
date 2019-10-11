#ifndef METASHELL_ENGINE_WAVE_CPP_VALIDATOR_HPP
#define METASHELL_ENGINE_WAVE_CPP_VALIDATOR_HPP

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

#include <metashell/engine/wave/preprocessor_shell.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      class cpp_validator : public iface::cpp_validator
      {
      public:
        explicit cpp_validator(data::wave_config);

        virtual data::result
        validate_code(const data::cpp_code& src_,
                      const data::config& config_,
                      const iface::environment& env_,
                      bool use_precompiled_headers_) override;

      private:
        preprocessor_shell _preprocessor;
      };
    }
  }
}

#endif
