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

#include "mock_console.hpp"

using namespace metashell;

mock_console::mock_console(int width_, int height_) :
  _width(width_),
  _height(height_)
{}

void mock_console::show(const metashell::data::colored_string& s_)
{
  _content += s_;
}

void mock_console::new_line()
{
  _content += "\n";
}

int mock_console::width() const
{
  return _width;
}

int mock_console::height() const
{
  return _height;
}

iface::console::user_answer mock_console::ask_for_continuation()
{
  ++_ask_for_continuation_count;
  return _continuation_answer;
}

void mock_console::clear()
{
  _content.clear();
  _ask_for_continuation_count = 0;
}

void mock_console::set_continiation_answer(user_answer answer)
{
  _continuation_answer = answer;
}

void mock_console::set_width(int width_)
{
  _width = width_;
}

void mock_console::set_height(int height_)
{
  _height = height_;
}

const metashell::data::colored_string& mock_console::content() const
{
  return _content;
}

int mock_console::ask_for_continuation_count() const
{
  return _ask_for_continuation_count;
}

