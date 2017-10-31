#ifndef METASHELL_SYSTEM_TEST_METASHELL_TERMINATED_HPP
#define METASHELL_SYSTEM_TEST_METASHELL_TERMINATED_HPP

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

#include <stdexcept>
#include <string>

namespace metashell
{
  namespace system_test
  {
    class metashell_terminated : public std::runtime_error
    {
    public:
      explicit metashell_terminated(std::string stderr_);

      const std::string& standard_error() const;

    private:
      std::string _stderr;
    };
  }
}

#endif
