#ifndef METASHELL_IFACE_PREPROCESSOR_SHELL_HPP
#define METASHELL_IFACE_PREPROCESSOR_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/result.hpp>

#include <string>

namespace metashell
{
  namespace iface
  {
    class preprocessor_shell
    {
    public:
      virtual ~preprocessor_shell() {}

      virtual data::result precompile(const data::cpp_code& exp_) = 0;

      static std::string name_of_feature() { return "preprocessor_shell"; }
    };
  }
}

#endif
