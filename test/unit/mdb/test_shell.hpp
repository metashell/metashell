#ifndef METASHELL_MDB_TEST_SHELL_HPP
#define METASHELL_MDB_TEST_SHELL_HPP

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

#include <memory>
#include <vector>

#include <metashell/mdb/metaprogram.hpp>
#include <metashell/mdb/shell.hpp>

class test_shell : public metashell::mdb::shell
{
public:
  test_shell(const std::string& line = "");

  bool has_metaprogram() const;
  const metashell::mdb::metaprogram& get_metaprogram() const;

  const metashell::mdb::breakpoints_t& get_breakpoints() const;
};

#endif
