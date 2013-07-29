// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "exception.hpp"

using namespace metashell;

exception::exception(const std::string& msg_) : std::runtime_error(msg_) {}


