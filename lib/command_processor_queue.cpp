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

#include <metashell/command_processor_queue.hpp>

#include <cassert>

using namespace metashell;

command_processor_queue::command_processor_queue(iface::history& history_) :
  _history(history_)
{}

bool command_processor_queue::empty() const
{
  return _items.empty();
}

void command_processor_queue::code_complete(
  const std::string& s_,
  std::set<std::string>& out_
) const
{
  if (!empty())
  {
    _items.back()->code_complete(s_, out_);
  }
}

void command_processor_queue::pop()
{
  assert(!empty());

  _items.pop_back();
}

void command_processor_queue::cancel_operation()
{
  if (!empty())
  {
    _items.back()->cancel_operation();
  }
}

void command_processor_queue::line_available(
  const std::string& cmd_,
  iface::displayer& displayer_
)
{
  if (!empty())
  {
    _items.back()->line_available(cmd_, displayer_, _history);
  }
}

std::string command_processor_queue::prompt() const
{
  assert(!empty());

  return _items.back()->prompt();
}

void command_processor_queue::pop_stopped_processors()
{
  while (!empty() && _items.back()->stopped())
  {
    pop();
  }
}

void command_processor_queue::push(
  std::unique_ptr<iface::command_processor> item_
)
{
  _items.push_back(move(item_));
}

