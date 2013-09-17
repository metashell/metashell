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

#include "result.hpp"

#include <set>
#include <string>

namespace metashell
{
  std::string append_to_buffer(
    const std::string& buffer_,
    const std::string& s_
  );

  result eval_tmp(
    const std::string& buffer_,
    const std::string& tmp_exp_,
    const config& config_
  );

  result validate_code(const std::string& s_, const config& config_);

  void code_complete(
    const std::string& buffer_,
    const std::string& src_,
    const config& config_,
    std::set<std::string>& out_
  );
}

#endif

