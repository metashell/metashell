// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "editline_shell.hpp"

#include <metashell/config.hpp>

#include <vector>
#include <string>

int main(int argc_, char* argv_[])
{
  metashell::config cfg;

  editline_shell shell(cfg);

  shell.display_splash();

  shell.run();
}

