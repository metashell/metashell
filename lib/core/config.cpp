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
#include <metashell/default_environment_detector.hpp>
#include <metashell/null_displayer.hpp>

#include <metashell/metashell.hpp>

using namespace metashell;

data::config metashell::detect_config(
  const data::user_config& ucfg_,
  iface::environment_detector& env_detector_,
  iface::displayer& displayer_,
  logger* logger_
)
{
  METASHELL_LOG(logger_, "Detecting config");

  data::config cfg;

  cfg.verbose = ucfg_.verbose;
  cfg.extra_clang_args = ucfg_.extra_clang_args;

  cfg.clang_path = ucfg_.clang_path;

  cfg.saving_enabled = ucfg_.saving_enabled;

  cfg.use_precompiled_headers = ucfg_.use_precompiled_headers;

  cfg.splash_enabled = ucfg_.splash_enabled;

  cfg.engine = ucfg_.engine;

  METASHELL_LOG(logger_, "Config detection completed");

  return cfg;
}

data::config metashell::empty_config(const std::string& argv0_)
{
  default_environment_detector ed(argv0_);
  null_displayer d;
  return detect_config(data::user_config(), ed, d, nullptr);
}

