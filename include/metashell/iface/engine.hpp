#ifndef METASHELL_IFACE_ENGINE_HPP
#define METASHELL_IFACE_ENGINE_HPP

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

#include <metashell/data/result.hpp>
#include <metashell/data/config.hpp>
#include <metashell/iface/environment.hpp>

#include <string>
#include <set>

namespace metashell
{
  namespace iface
  {
    class engine
    {
    public:
      virtual ~engine() {}

      virtual data::result eval_tmp_formatted(
        const environment& env_,
        const std::string& tmp_exp_
      ) = 0;

      virtual data::result eval_tmp(
        const environment& env_,
        const std::string& tmp_exp_
      ) = 0;

      virtual data::result eval_environment(const environment& env_) = 0;

      virtual data::result validate_code(
        const std::string& s_,
        const data::config& config_,
        const environment& env_
      ) = 0;

      virtual void code_complete(
        const environment& env_,
        const std::string& src_,
        std::set<std::string>& out_
      ) = 0;

    };
  }
}

#endif

