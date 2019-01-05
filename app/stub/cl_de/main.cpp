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
      "Microsoft (R) C/C++-Optimierungscompiler Version 19.16.27025.1 "
      "f\xc3\xbcr x86\n"
      "Copyright (C) Microsoft Corporation. Alle Rechte vorbehalten.\n"
      "\n",

      "Syntax: cl [ Option... ] Dateiname... [ /link Linkeroption... ]\n\n",

      [](const std::string& option_) {
        return "cl : Befehlszeile warning D9002 : Unbekannte Option \"" +
               option_ +
               "\" wird ignoriert.\n"
               "cl : Befehlszeile error D8003 : Name der Quelldatei fehlt.\n";
      });

  return compiler.run(argc_, argv_).value();
}
