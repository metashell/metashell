#ifndef METASHELL_STREAM_CONSOLE_HPP
#define METASHELL_STREAM_CONSOLE_HPP

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

#include <metashell/iface/console.hpp>

#include <iosfwd>

namespace metashell
{
  class stream_console : public iface::console
  {
  public:
    explicit stream_console(std::ostream& s_);

    virtual void show(const data::colored_string& s_) override;
    virtual void new_line() override;

    virtual user_answer ask_for_continuation() override;

    virtual int width() const override;
    virtual int height() const override;
  private:
    std::ostream* _s;
  };
}

#endif

