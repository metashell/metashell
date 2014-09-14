#ifndef METASHELL_METASHELL_HPP
#define METASHELL_METASHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/command.hpp>

#include "result.hpp"

#include <set>
#include <string>
#include <vector>

namespace metashell
{
  result eval_tmp_unformatted(
    const environment& env_,
    const std::string& tmp_exp_,
    const config& config_,
    const std::string& input_filename_
  );

  result eval_tmp_formatted(
    const environment& env_,
    const std::string& tmp_exp_,
    const config& config_,
    const std::string& input_filename_
  );

  result validate_code(
    const std::string& s_,
    const config& config_,
    const environment& env_,
    const std::string& intput_filename_
  );

  void code_complete(
    const environment& env_,
    const std::string& src_,
    const std::string& input_filename_,
    std::set<std::string>& out_
  );

  bool is_environment_setup_command(
    command::iterator begin_,
    const command::iterator& end_
  );

  inline bool is_environment_setup_command(const command& cmd_)
  {
    return is_environment_setup_command(cmd_.begin(), cmd_.end());
  }
}

#endif

