#ifndef METASHELL_SHELL_STUB_HPP
#define METASHELL_SHELL_STUB_HPP

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

#include <metashell/shell.hpp>

namespace metashell
{
  class shell_stub : public shell
  {
  public:
    shell_stub();

    virtual void add_history(const std::string& s_);

    virtual void display_normal(const std::string& s_) const;
    virtual void display_info(const std::string& s_) const;
    virtual void display_error(const std::string& s_) const;

    virtual unsigned int width() const;
  };
}

#endif

