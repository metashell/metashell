// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/system_test_config.hpp>

using namespace metashell_system_test;

namespace
{
  std::string metashell_binary_path;
  std::vector<std::string> metashell_args;
}

void system_test_config::metashell_binary(const std::string& path_)
{
  metashell_binary_path = path_;
}

void system_test_config::metashell_arg(const std::string& arg_)
{
  ::metashell_args.push_back(arg_);
}

std::string system_test_config::metashell_binary()
{
  return metashell_binary_path;
}

const std::vector<std::string>& system_test_config::metashell_args()
{
  return ::metashell_args;
}

