// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/proc_exit.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    proc_exit::proc_exit(data::exit_status exit_status_)
      : _exit_status(exit_status_)
    {
    }

    data::exit_status proc_exit::exit_status() const { return _exit_status; }

    proc_exit exit_success() { return proc_exit{exit_status{EXIT_SUCCESS}}; }

    proc_exit exit_failure() { return proc_exit{exit_status{EXIT_FAILURE}}; }

    bool exit_success(const proc_exit& e_)
    {
      return e_.exit_status() == exit_status{EXIT_SUCCESS};
    }

    bool operator==(const proc_exit& lhs_, const proc_exit& rhs_)
    {
      return lhs_.exit_status() == rhs_.exit_status();
    }

    std::string to_string(const proc_exit& e_)
    {
      return "Process terminated with exit status " +
             to_string(e_.exit_status());
    }

    std::ostream& operator<<(std::ostream& out_, const proc_exit& e_)
    {
      return out_ << to_string(e_);
    }
  } // namespace data
} // namespace metashell
