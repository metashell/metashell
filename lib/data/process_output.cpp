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

#include <metashell/data/process_output.hpp>

#include <string>

using namespace metashell::data;

process_output::process_output(
  exit_code_t exit_code_,
  const std::string& stdout_,
  const std::string& stderr_
) :
  _exit_code(exit_code_),
  _out(stdout_),
  _err(stderr_)
{}

exit_code_t process_output::exit_code() const
{
  return _exit_code;
}

const std::string& process_output::standard_output() const
{
  return _out;
}

const std::string& process_output::standard_error() const
{
  return _err;
}

