#ifndef METASHELL_CONFIG_HPP
#define METASHELL_CONFIG_HPP

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

#include <metashell/standard.hpp>
#include <metashell/console_type.hpp>
#include <metashell/iface/environment_detector.hpp>

#include <string>
#include <vector>
#include <iosfwd>

namespace metashell
{
  struct user_config;

  class config
  {
  public:
    std::vector<std::string> include_path;
    bool verbose;
    bool syntax_highlight;
    bool indent;
    standard::type standard_to_use;
    std::vector<std::string> macros;
    bool warnings_enabled;
    std::vector<std::string> extra_clang_args;
    bool use_precompiled_headers;
    std::string clang_path;
    int max_template_depth;
    unsigned templight_trace_capacity;
    bool saving_enabled;
    console_type con_type;

    config();
  };

  config detect_config(
    const user_config& ucfg_,
    iface::environment_detector& env_detector_,
    std::ostream& stderr_
  );

  config empty_config(const std::string& argv0_);
}

#endif

