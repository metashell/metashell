// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include "test_shell.hpp"

#include <metashell/engine/constant/builder.hpp>

#include <metashell/core/header_file_environment.hpp>

#include <metashell/data/config.hpp>

namespace
{
  boost::filesystem::path temp_dir() { return boost::filesystem::path(); }
  boost::filesystem::path mdb_temp_dir() { return boost::filesystem::path(); }

  boost::filesystem::path env_filename() { return boost::filesystem::path(); }

  metashell::data::config get_shell_config()
  {
    metashell::data::config cfg;
    cfg.push_back(metashell::data::shell_config());
    return cfg;
  }

  metashell::iface::environment& get_env()
  {
    static metashell::core::header_file_environment env(
        nullptr, get_shell_config().active_shell_config(), temp_dir(),
        env_filename());
    return env;
  }

  metashell::iface::engine& get_engine()
  {
    static std::unique_ptr<metashell::iface::engine> engine =
        metashell::engine::constant::create_failing()(get_shell_config());
    return *engine;
  }
}

test_shell::test_shell(const std::string& line)
  : metashell::mdb::shell(get_env(),
                          get_engine(),
                          temp_dir() / "test.hpp",
                          mdb_temp_dir(),
                          false,
                          nullptr)
{
  env.append(metashell::data::cpp_code(line));
}

bool test_shell::has_metaprogram() const { return static_cast<bool>(mp); }

const metashell::mdb::metaprogram& test_shell::get_metaprogram() const
{
  return *mp;
}

const metashell::mdb::breakpoints_t& test_shell::get_breakpoints() const
{
  return breakpoints;
}
