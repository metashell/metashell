// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "interrupt_handler_override.hpp"

#include <cassert>

#ifndef _MSC_VER
  #include <signal.h>
#endif

namespace
{
  boost::function<void ()> handler_function;

  void call_handler()
  {
    assert(handler_function);
    handler_function();
  }

#ifdef _MSC_VER
  void install_callback()
  {
    // MinGW EditLine exits when Ctrl-C is pressed
  }

  void remove_callback()
  {
  }
#else
  sighandler_t old_handler;

  void sigint_handler(int)
  {
    call_handler();
  }

  void install_callback()
  {
    old_handler = signal(SIGINT, sigint_handler);
  }

  void remove_callback()
  {
    signal(SIGINT, old_handler);
  }
#endif
}

interrupt_handler_override::interrupt_handler_override(
  const boost::function<void ()>& handler_
)
{
  assert(!handler_function);
  handler_function = handler_;
  install_callback();
}

interrupt_handler_override::~interrupt_handler_override()
{
  remove_callback();
}

