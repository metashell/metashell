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

#include <metashell/core/engine_constant.hpp>
#include <metashell/core/shell.hpp>

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

  metashell::core::shell& get_shell()
  {
    static metashell::core::shell sh(get_shell_config(), temp_dir(),
                                     env_filename(),
                                     metashell::core::create_failing_engine());
    return sh;
  }
}

test_shell::test_shell(const std::string& line)
  : metashell::mdb::shell(get_shell().env(),
                          get_shell().engine(),
                          get_shell().env_path(),
                          mdb_temp_dir(),
                          false,
                          nullptr)
{
  env.append(metashell::data::cpp_code(line));
}

test_shell::test_shell(metashell::core::shell& shell, const std::string& line)
  : metashell::mdb::shell(shell.env(),
                          shell.engine(),
                          shell.env_path(),
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
