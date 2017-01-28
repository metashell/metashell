#ifndef METASHELL_IFACE_CPP_VALIDATOR_HPP
#define METASHELL_IFACE_CPP_VALIDATOR_HPP

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

#include <metashell/data/config.hpp>
#include <metashell/data/cpp_code.hpp>
#include <metashell/data/result.hpp>
#include <metashell/iface/environment.hpp>

#include <string>

namespace metashell
{
  namespace iface
  {
    class cpp_validator
    {
    public:
      virtual ~cpp_validator() {}

      virtual data::result validate_code(const data::cpp_code& s_,
                                         const data::config& config_,
                                         const environment& env_,
                                         bool use_precompiled_headers_) = 0;

      static std::string name_of_feature() { return "cpp_validator"; }
    };
  }
}

#endif
