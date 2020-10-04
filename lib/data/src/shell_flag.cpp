// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/shell_flag.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    std::string to_string(shell_flag flag_)
    {
      switch (flag_)
      {
      case shell_flag::echo:
        return "display preprocessed commands";
      case shell_flag::evaluate_metaprograms:
        return "evaluation of metaprograms";
      case shell_flag::show_cpp_errors:
        return "display C++ errors";
      case shell_flag::use_precompiled_headers:
        return "precompiled header usage";
      case shell_flag::verbose:
        return "verbose mode";
      }
      assert(!"Invalid shell_flag");
      return "";
    }

    std::ostream& operator<<(std::ostream& out_, shell_flag flag_)
    {
      return out_ << to_string(flag_);
    }
  } // namespace data
} // namespace metashell
