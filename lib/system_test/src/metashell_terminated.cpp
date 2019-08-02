// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/metashell_terminated.hpp>

namespace metashell
{
  namespace system_test
  {
    metashell_terminated::metashell_terminated(const process_execution& pe_,
                                               std::string stderr_)
      : std::runtime_error("Metashell terminated. Command:\n" +
                           to_string(pe_.args()) + "\n\nStandard input:\n" +
                           pe_.standard_input() + "\n\nStandard output:\n" +
                           pe_.standard_output() + "\n\nStandard error:\n" +
                           stderr_),
        _stderr(std::move(stderr_))
    {
    }

    const std::string& metashell_terminated::standard_error() const
    {
      return _stderr;
    }
  }
}
