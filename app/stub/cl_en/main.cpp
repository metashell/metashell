// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/compiler_stub/cl_stub.hpp>

using namespace metashell;

int main(int argc_, const char* argv_[])
{
  compiler_stub::cl_stub compiler(
      "Microsoft (R) C/C++ Optimizing Compiler Version 19.16.27025.1 for x86\n"
      "Copyright (C) Microsoft Corporation.  All rights reserved.\n"
      "\n",

      "usage: cl [ option... ] filename... [ /link linkoption... ]\n",

      [](const std::string& option_) {
        return "cl : Command line warning D9002 : ignoring unknown option '" +
               option_ +
               "'\n"
               "cl : Command line error D8003 : missing source filename\n";
      });

  return compiler.run(argc_, argv_).exit_status().value();
}
