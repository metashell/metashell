#ifndef METASHELL_PARSE_CONFIG_HPP
#define METASHELL_PARSE_CONFIG_HPP

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

#include <metashell/data/config.hpp>
#include <metashell/data/real_engine_name.hpp>

#include <metashell/core/engine_entry.hpp>

#include <metashell/iface/environment_detector.hpp>

#include <variant.hpp>

#include <iosfwd>
#include <map>

namespace metashell
{
  namespace core
  {
    struct exit
    {
      bool with_error;
    };

    using parse_config_result = mpark::variant<data::config, exit>;

    parse_config_result
    parse_config(int argc_,
                 const char* argv_[],
                 const std::map<data::real_engine_name, engine_entry>& engines_,
                 iface::environment_detector& env_detector_,
                 std::ostream* out_ = 0,
                 std::ostream* err_ = 0);
  }
}

#endif
