// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/config.hpp>

using namespace metashell;

namespace
{
  const char* default_includes[] =
    {
      ""
      #include "default_include.hpp"
    };

  const char* default_sysincludes[] =
    {
      ""
      #include "default_sysinclude.hpp"
    };
}

config::config() :
  include_path(
    default_includes + 1,
    default_includes + sizeof(default_includes) / sizeof(const char*)
  ),
  sysinclude_path(
    default_sysincludes + 1,
    default_sysincludes + sizeof(default_sysincludes) / sizeof(const char*)
  )
{}


