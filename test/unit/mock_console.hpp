#ifndef METASHELL_MOCK_CONSOLE_HPP
#define METASHELL_MOCK_CONSOLE_HPP

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

#include <limits>

class mock_console : public metashell::iface::console
{
public:
  explicit mock_console(int width_ = 80,
                        int height_ = std::numeric_limits<int>::max());

  virtual void show(const metashell::data::colored_string& s_) override;
  virtual void new_line() override;

  virtual user_answer ask_for_continuation() override;

  virtual int width() const override;
  virtual int height() const override;

  void clear();
  void set_continiation_answer(user_answer answer);
  void set_width(int width_);
  void set_height(int width_);

  const metashell::data::colored_string& content() const;
  int ask_for_continuation_count() const;

private:
  metashell::data::colored_string _content;
  int _ask_for_continuation_count = 0;

  user_answer _continuation_answer = user_answer::show_all;
  int _width;
  int _height;
};

#endif
