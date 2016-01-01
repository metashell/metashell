#ifndef METASHELL_DISABLE_PRECOMPILED_HEADER_GUARD_HPP
#define METASHELL_DISABLE_PRECOMPILED_HEADER_GUARD_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/shell.hpp>
#include <metashell/iface/destroyable.hpp>

#include <memory>

namespace metashell
{
  class disable_precompiled_header_guard : public iface::destroyable
  {
  public:
    explicit disable_precompiled_header_guard(shell& shell_);
    ~disable_precompiled_header_guard();

    static std::unique_ptr<disable_precompiled_header_guard>
    create(shell& shell_);

  private:
    shell& _shell;
    bool _was_using;
  };
}

#endif
