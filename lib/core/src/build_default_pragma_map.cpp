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

#include <metashell/core/build_default_pragma_map.hpp>
#include <metashell/core/metashell_pragma.hpp>
#include <metashell/core/pragma_config.hpp>
#include <metashell/core/pragma_config_load.hpp>
#include <metashell/core/pragma_config_show.hpp>
#include <metashell/core/pragma_echo.hpp>
#include <metashell/core/pragma_environment.hpp>
#include <metashell/core/pragma_environment_add.hpp>
#include <metashell/core/pragma_environment_pop.hpp>
#include <metashell/core/pragma_environment_push.hpp>
#include <metashell/core/pragma_environment_reload.hpp>
#include <metashell/core/pragma_environment_reset.hpp>
#include <metashell/core/pragma_environment_save.hpp>
#include <metashell/core/pragma_environment_stack.hpp>
#include <metashell/core/pragma_evaluate.hpp>
#include <metashell/core/pragma_help.hpp>
#include <metashell/core/pragma_included_headers.hpp>
#include <metashell/core/pragma_includes.hpp>
#include <metashell/core/pragma_ls.hpp>
#include <metashell/core/pragma_macro.hpp>
#include <metashell/core/pragma_macro_names.hpp>
#include <metashell/core/pragma_macros.hpp>
#include <metashell/core/pragma_mdb.hpp>
#include <metashell/core/pragma_pp.hpp>
#include <metashell/core/pragma_quit.hpp>
#include <metashell/core/pragma_switch.hpp>
#include <metashell/core/pragma_which.hpp>
#include <metashell/core/shell.hpp>

#include <metashell/data/exception.hpp>

namespace metashell
{
  namespace core
  {
    namespace
    {
      std::string on_off(bool v_) { return v_ ? "on" : "off"; }

      std::unique_ptr<pragma_macro> shell_mode(const std::string& name_,
                                               const std::string& comment_,
                                               bool preprocessing_mode_,
                                               iface::command_processor& shell_)
      {
        std::vector<std::string> cmds{
            "#msh preprocessed echo " + on_off(preprocessing_mode_),
            "#msh show cpp_errors " + on_off(!preprocessing_mode_),
            "#msh metaprogram evaluation " + on_off(!preprocessing_mode_)};

        if (!comment_.empty())
        {
          cmds.push_back("#msh echo " + comment_);
        }

        return std::make_unique<pragma_macro>(
            "Set Metashell to " + name_ + " mode", move(cmds), shell_);
      }
    }

    std::map<std::vector<std::string>, std::unique_ptr<iface::pragma_handler>>
    build_default_pragma_map(shell& shell_,
                             command_processor_queue* cpq_,
                             const boost::filesystem::path& mdb_temp_dir_,
                             logger* logger_)
    {
      typedef std::vector<std::string> sv;

      std::map<sv, std::unique_ptr<iface::pragma_handler>> result;

      result.emplace(
          sv{"help"}, std::make_unique<pragma_help>(shell_.pragma_handlers()));

      result.emplace(
          sv{"verbose"},
          std::make_unique<pragma_switch>(
              "verbose mode", [&shell_]() { return shell_.verbose(); },
              [&shell_](bool v_) { shell_.verbose(v_); }));

      result.emplace(
          sv{"precompiled_headers"},
          std::make_unique<pragma_switch>(
              "precompiled header usage",
              [&shell_]() { return shell_.using_precompiled_headers(); },
              [&shell_](bool v_) { shell_.using_precompiled_headers(v_); }));

      result.emplace(sv{"environment"},
                     std::make_unique<pragma_environment>(shell_.env()));

      result.emplace(sv{"environment", "push"},
                     std::make_unique<pragma_environment_push>(shell_));

      result.emplace(sv{"environment", "pop"},
                     std::make_unique<pragma_environment_pop>(shell_));

      result.emplace(sv{"environment", "stack"},
                     std::make_unique<pragma_environment_stack>(shell_));

      result.emplace(sv{"environment", "add"},
                     std::make_unique<pragma_environment_add>(shell_));

      result.emplace(sv{"environment", "reset"},
                     std::make_unique<pragma_environment_reset>(shell_));

      result.emplace(sv{"environment", "reload"},
                     std::make_unique<pragma_environment_reload>(shell_));

      result.emplace(
          sv{"environment", "save"}, std::make_unique<pragma_environment_save>(
                                         shell_.get_config(), shell_.env()));

      result.emplace(sv{"preprocessed", "echo"},
                     std::make_unique<pragma_switch>(
                         "display preprocessed commands",
                         [&shell_]() { return shell_.echo(); },
                         [&shell_](bool v_) { shell_.echo(v_); }));

      result.emplace(
          sv{"mdb"}, std::make_unique<pragma_mdb>(
                         shell_, cpq_, mdb_temp_dir_, false, logger_));

      result.emplace(sv{"evaluate"}, std::make_unique<pragma_evaluate>(shell_));

      result.emplace(
          sv{"pdb"}, std::make_unique<pragma_mdb>(
                         shell_, cpq_, mdb_temp_dir_, true, logger_));

      result.emplace(sv{"pp"}, std::make_unique<pragma_pp>(shell_));

      result.emplace(sv{"show", "cpp_errors"},
                     std::make_unique<pragma_switch>(
                         "display C++ errors",
                         [&shell_]() { return shell_.show_cpp_errors(); },
                         [&shell_](bool v_) { shell_.show_cpp_errors(v_); }));

      result.emplace(
          sv{"metaprogram", "evaluation"},
          std::make_unique<pragma_switch>(
              "evaluation of metaprograms",
              [&shell_]() { return shell_.evaluate_metaprograms(); },
              [&shell_](bool v_) { shell_.evaluate_metaprograms(v_); }));

      result.emplace(sv{"preprocessor", "mode"},
                     shell_mode("preprocessor",
                                "To switch back to the default mode, run #msh "
                                "metaprogram mode",
                                true, shell_));

      result.emplace(sv{"metaprogram", "mode"},
                     shell_mode("metaprogram", "", false, shell_));

      result.emplace(sv{"echo"}, std::make_unique<pragma_echo>());

      result.emplace(sv{"macros"}, std::make_unique<pragma_macros>(shell_));

      result.emplace(
          sv{"macro", "names"}, std::make_unique<pragma_macro_names>(shell_));

      result.emplace(
          sv{"sysincludes"},
          std::make_unique<pragma_includes<data::include_type::sys>>(shell_));

      result.emplace(
          sv{"quoteincludes"},
          std::make_unique<pragma_includes<data::include_type::quote>>(shell_));

      result.emplace(sv{"which"}, std::make_unique<pragma_which>(shell_));

      result.emplace(sv{"included", "headers"},
                     std::make_unique<pragma_included_headers>(shell_));

      result.emplace(sv{"ls"}, std::make_unique<pragma_ls>(shell_));

      result.emplace(sv{"config"}, std::make_unique<pragma_config>(shell_));

      result.emplace(
          sv{"config", "show"}, std::make_unique<pragma_config_show>(shell_));

      result.emplace(
          sv{"config", "load"}, std::make_unique<pragma_config_load>(shell_));

      result.emplace(sv{"quit"}, std::make_unique<pragma_quit>(shell_));

      return result;
    }
  }
}
