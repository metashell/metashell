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
#include <metashell/engine_clang.hpp>
#include <metashell/has_prefix.hpp>

#include <metashell/version.hpp>
#include <metashell/wave_tokeniser.hpp>
#include <metashell/readline/version.hpp>

#include <just/temp.hpp>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

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
        {"Boost.Wave", metashell::wave_version()},
        {readline_name, metashell::readline::version()}
      };
  }

  std::string set_max_template_depth(int v_)
  {
    return "-ftemplate-depth=" + std::to_string(v_);
  }

  template <class Cont>
  void add_with_prefix(
    const std::string& prefix_,
    const Cont& cont_,
    std::vector<std::string>& v_
  )
  {
    std::transform(
      cont_.begin(),
      cont_.end(),
      std::back_insert_iterator<std::vector<std::string> >(v_),
      [&prefix_] (const std::string& s_) { return prefix_ + s_; }
    );
  }

  bool cpp_standard_set(const std::vector<std::string>& args_)
  {
    return metashell::has_prefix(args_, {"--std", "-std"});
  }

  bool max_template_depth_set(const std::vector<std::string>& args_)
  {
    return metashell::has_prefix(args_, {"-ftemplate-depth"});
  }
}

int main(int argc_, const char* argv_[])
{
  const std::string env_filename = "metashell_environment.hpp";

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
    case metashell::data::logging_mode::none:
      // do nothing
      break;
    case metashell::data::logging_mode::console:
      logger.log_to_console();
      break;
    case metashell::data::logging_mode::file:
      logger.log_into_file(r.cfg.log_file);
      break;
    }

    METASHELL_LOG(&logger, "Start logging");

    if (r.should_run_shell())
    {
      metashell::default_environment_detector det(argv_[0]);
      const metashell::data::config
        cfg = detect_config(r.cfg, det, ccfg.displayer(), &logger);

      METASHELL_LOG(&logger, "Running shell");

      just::temp::directory dir;

      std::vector<std::string> clang_args;

      add_with_prefix("-I", cfg.include_path, clang_args);
      add_with_prefix("-D", cfg.macros, clang_args);
      if (!cpp_standard_set(cfg.extra_clang_args))
      {
        clang_args.push_back("-std=c++0x");
      }

      if (!max_template_depth_set(cfg.extra_clang_args))
      {
        clang_args.push_back(set_max_template_depth(256));
      }

      if (!cfg.warnings_enabled)
      {
        clang_args.push_back("-w");
      }

      clang_args.insert(
        clang_args.end(),
        cfg.extra_clang_args.begin(),
        cfg.extra_clang_args.end()
      );

      std::unique_ptr<metashell::shell>
        shell(
          new metashell::shell(
            cfg,
            ccfg.processor_queue(),
            dir.path(),
            env_filename,
            metashell::create_clang_engine(
              cfg.clang_path,
              dir.path(),
              env_filename,
              clang_args,
              det,
              &logger
            ),
            &logger
          )
        );

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

