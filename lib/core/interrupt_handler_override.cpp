// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/interrupt_handler_override.hpp>

#include <cassert>

#ifndef _MSC_VER
#include <signal.h>
#endif

using namespace metashell;

namespace
{
  std::function<void()> handler_function;

  void call_handler()
  {
    assert(handler_function);
    handler_function();
  }

#ifdef _MSC_VER
  void install_callback() {}

  void remove_callback() {}
#else
  void (*old_handler)(int);

  void sigint_handler(int) { call_handler(); }

  void install_callback() { old_handler = signal(SIGINT, sigint_handler); }

  void remove_callback() { signal(SIGINT, old_handler); }
#endif
}

interrupt_handler_override::interrupt_handler_override(
    const std::function<void()>& handler_)
{
  assert(!handler_function);
  handler_function = handler_;
  install_callback();
}

interrupt_handler_override::~interrupt_handler_override() { remove_callback(); }
