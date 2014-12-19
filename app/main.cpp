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

#include <metashell/parse_config.hpp>
#include <metashell/config.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/console_displayer.hpp>
#include <metashell/stdout_console.hpp>
#include <metashell/readline_input_loop.hpp>
#include <metashell/readline_history.hpp>
#include <metashell/shell.hpp>

#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#  include <windows.h>
#endif

int main(int argc_, const char* argv_[])
{
  try
  {
    using metashell::parse_config;
    using metashell::parse_config_result;

    const parse_config_result
      r = parse_config(argc_, argv_, &std::cout, &std::cerr);

    metashell::default_environment_detector det(argv_[0]);
    const metashell::config cfg = detect_config(r.cfg, det, std::cerr);

    if (r.should_run_shell())
    {
      metashell::stdout_console c;
      metashell::console_displayer
        displayer(c, cfg.indent, cfg.syntax_highlight);
      metashell::readline_history history;
      metashell::command_processor_queue processor_queue(history);

      std::unique_ptr<metashell::shell>
        shell(new metashell::shell(cfg, processor_queue));

      shell->display_splash(displayer);

      processor_queue.push(move(shell));

      metashell::readline_input_loop(processor_queue, displayer);
      std::cout << std::endl;
    }
    return r.should_error_at_exit() ? 1 : 0;
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << std::endl;
  }
}

