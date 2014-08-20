#ifndef METASHELL_PRAGMA_ENVIRONMENT_POP_HPP
#define METASHELL_PRAGMA_ENVIRONMENT_POP_HPP

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

#include <metashell/pragma_handler_interface.hpp>

#include <string>

namespace metashell
{
  class shell;

  class pragma_environment_pop : public pragma_handler_interface
  {
  public:
    explicit pragma_environment_pop(shell& shell_);

    virtual pragma_handler_interface* clone() const;

    virtual std::string arguments() const;
    virtual std::string description() const;

    virtual void run(
      const command::iterator& args_begin_,
      const command::iterator& args_end_
    ) const;
  private:
    shell& _shell;
  };
}

#endif

