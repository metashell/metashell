#ifndef INTERRUPT_HANDLER_OVERRIDE_HPP
#define INTERRUPT_HANDLER_OVERRIDE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/utility.hpp>
#include <boost/function.hpp>

class interrupt_handler_override : boost::noncopyable
{
public:
  interrupt_handler_override(const boost::function<void ()>& handler_);
  ~interrupt_handler_override();
};

#endif

