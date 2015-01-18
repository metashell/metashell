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

#include "console_config.hpp"

#include <metashell/parse_config.hpp>
#include <metashell/config.hpp>
#include <metashell/default_environment_detector.hpp>
#include <metashell/shell.hpp>
#include <metashell/logger.hpp>
#include <metashell/fstream_file_writer.hpp>

#include <metashell/version.hpp>
#include <metashell/wave_tokeniser.hpp>
#include <metashell/readline/version.hpp>
#include <metashell/clang/version.hpp>

#include <iostream>
#include <stdexcept>

namespace
{
  std::map<std::string, std::string> get_dependency_versions()
  {
    const std::string readline_name =
#ifdef USE_EDITLINE
      "Libedit"
#else
      "Readline"
#endif
    ;

    return
      {
        {"libclang", metashell::clang::libclang_version()},
        {"Boost.Wave", metashell::wave_version()},
        {readline_name, metashell::readline::version()}
      };
  }
}

int main(int argc_, const char* argv_[])
{
  try
  {
    using metashell::parse_config;
    using metashell::parse_config_result;

    const parse_config_result
      r = parse_config(argc_, argv_, &std::cout, &std::cerr);

    metashell::console_config
      ccfg(r.cfg.con_type, r.cfg.indent, r.cfg.syntax_highlight);

    metashell::fstream_file_writer file_writer;
    metashell::logger logger(ccfg.displayer(), file_writer);
    switch (r.cfg.log_mode)
    {
    case metashell::logging_mode::none:
      // do nothing
      break;
    case metashell::logging_mode::console:
      logger.log_to_console();
      break;
    case metashell::logging_mode::file:
      logger.log_into_file(r.cfg.log_file);
      break;
    }

    METASHELL_LOG(&logger, "Start logging");

    metashell::default_environment_detector det(argv_[0], &logger);
    const metashell::config
      cfg = detect_config(r.cfg, det, ccfg.displayer(), &logger);

    if (r.should_run_shell())
    {
      METASHELL_LOG(&logger, "Running shell");

      std::unique_ptr<metashell::shell>
        shell(new metashell::shell(cfg, ccfg.processor_queue(), &logger));

      if (cfg.splash_enabled)
      {
        shell->display_splash(ccfg.displayer(), get_dependency_versions());
      }

      ccfg.processor_queue().push(move(shell));

      METASHELL_LOG(&logger, "Starting input loop");
      
      metashell::input_loop(
        ccfg.processor_queue(),
        ccfg.displayer(),
        ccfg.reader()
      );

      METASHELL_LOG(&logger, "Input loop finished");
    }
    else
    {
      METASHELL_LOG(&logger, "Not running shell");
    }
    return r.should_error_at_exit() ? 1 : 0;
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << std::endl;
  }
}

