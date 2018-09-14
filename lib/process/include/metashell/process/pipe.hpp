#ifndef METASHELL_PROCESS_PIPE_HPP
#define METASHELL_PROCESS_PIPE_HPP

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

#include <metashell/process/fd_t.hpp>
#include <metashell/process/input_file.hpp>
#include <metashell/process/output_file.hpp>

namespace metashell
{
  namespace process
  {
    class pipe
    {
    public:
      class fds
      {
      public:
        fds();

        fd_t read_fd() const;
        fd_t write_fd() const;

      private:
        fd_t _fd[2];
      };

      input_file input;
      output_file output;

      pipe(fds fds_ = fds());

      // Adding move operations for implementing ownership transfer
      pipe(pipe&& p_);
      pipe& operator=(pipe&& p_);
    };
  }
}

#endif
