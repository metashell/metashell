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

#include <metashell/defaults/pragma_map.hpp>

#include <metashell/pragma/config.hpp>
#include <metashell/pragma/config_load.hpp>
#include <metashell/pragma/config_show.hpp>
#include <metashell/pragma/echo.hpp>
#include <metashell/pragma/environment.hpp>
#include <metashell/pragma/environment_add.hpp>
#include <metashell/pragma/environment_pop.hpp>
#include <metashell/pragma/environment_push.hpp>
#include <metashell/pragma/environment_reload.hpp>
#include <metashell/pragma/environment_reset.hpp>
#include <metashell/pragma/environment_save.hpp>
#include <metashell/pragma/environment_stack.hpp>
#include <metashell/pragma/evaluate.hpp>
#include <metashell/pragma/help.hpp>
#include <metashell/pragma/included_headers.hpp>
#include <metashell/pragma/includes.hpp>
#include <metashell/pragma/ls.hpp>
#include <metashell/pragma/macro.hpp>
#include <metashell/pragma/macro_names.hpp>
#include <metashell/pragma/macros.hpp>
#include <metashell/pragma/mdb.hpp>
#include <metashell/pragma/pp.hpp>
#include <metashell/pragma/quit.hpp>
#include <metashell/pragma/switch_.hpp>
#include <metashell/pragma/which.hpp>

namespace metashell
{
  namespace defaults
  {
    namespace
    {
      std::string on_off(bool v_) { return v_ ? "on" : "off"; }

      std::unique_ptr<pragma::macro> shell_mode(const std::string& name_,
                                                const std::string& comment_,
                                                bool preprocessing_mode_)
      {
        std::vector<std::string> cmds{
            "#msh preprocessed echo " + on_off(preprocessing_mode_),
            "#msh show cpp_errors " + on_off(!preprocessing_mode_),
            "#msh metaprogram evaluation " + on_off(!preprocessing_mode_)};

        if (!comment_.empty())
        {
          cmds.push_back("#msh echo " + comment_);
        }

        return std::make_unique<pragma::macro>(
            "Set Metashell to " + name_ + " mode", move(cmds));
      }
    }

    std::map<data::pragma_name, std::unique_ptr<iface::pragma_handler>>
    pragma_map(core::command_processor_queue* cpq_,
               const boost::filesystem::path& mdb_temp_dir_,
               core::logger* logger_)
    {
      using sv = data::pragma_name;

      std::map<sv, std::unique_ptr<iface::pragma_handler>> result;

      result.emplace(sv{"help"}, std::make_unique<pragma::help>());

      result.emplace(sv{"verbose"}, std::make_unique<pragma::switch_>(
                                        data::shell_flag::verbose));

      result.emplace(sv{"precompiled_headers"},
                     std::make_unique<pragma::switch_>(
                         data::shell_flag::use_precompiled_headers));

      result.emplace(
          sv{"environment"}, std::make_unique<pragma::environment>());

      result.emplace(sv{"environment", "push"},
                     std::make_unique<pragma::environment_push>());

      result.emplace(sv{"environment", "pop"},
                     std::make_unique<pragma::environment_pop>());

      result.emplace(sv{"environment", "stack"},
                     std::make_unique<pragma::environment_stack>());

      result.emplace(sv{"environment", "add"},
                     std::make_unique<pragma::environment_add>());

      result.emplace(sv{"environment", "reset"},
                     std::make_unique<pragma::environment_reset>());

      result.emplace(sv{"environment", "reload"},
                     std::make_unique<pragma::environment_reload>());

      result.emplace(sv{"environment", "save"},
                     std::make_unique<pragma::environment_save>());

      result.emplace(sv{"preprocessed", "echo"},
                     std::make_unique<pragma::switch_>(data::shell_flag::echo));

      result.emplace(sv{"mdb"}, std::make_unique<pragma::mdb>(
                                    cpq_, mdb_temp_dir_, false, logger_));

      result.emplace(sv{"evaluate"}, std::make_unique<pragma::evaluate>());

      result.emplace(sv{"pdb"}, std::make_unique<pragma::mdb>(
                                    cpq_, mdb_temp_dir_, true, logger_));

      result.emplace(sv{"pp"}, std::make_unique<pragma::pp>());

      result.emplace(
          sv{"show", "cpp_errors"},
          std::make_unique<pragma::switch_>(data::shell_flag::show_cpp_errors));

      result.emplace(sv{"metaprogram", "evaluation"},
                     std::make_unique<pragma::switch_>(
                         data::shell_flag::evaluate_metaprograms));

      result.emplace(sv{"preprocessor", "mode"},
                     shell_mode("preprocessor",
                                "To switch back to the default mode, run #msh "
                                "metaprogram mode",
                                true));

      result.emplace(
          sv{"metaprogram", "mode"}, shell_mode("metaprogram", "", false));

      result.emplace(sv{"echo"}, std::make_unique<pragma::echo>());

      result.emplace(sv{"macros"}, std::make_unique<pragma::macros>());

      result.emplace(
          sv{"macro", "names"}, std::make_unique<pragma::macro_names>());

      result.emplace(
          sv{"sysincludes"},
          std::make_unique<pragma::includes<data::include_type::sys>>());

      result.emplace(
          sv{"quoteincludes"},
          std::make_unique<pragma::includes<data::include_type::quote>>());

      result.emplace(sv{"which"}, std::make_unique<pragma::which>());

      result.emplace(sv{"included", "headers"},
                     std::make_unique<pragma::included_headers>());

      result.emplace(sv{"ls"}, std::make_unique<pragma::ls>());

      result.emplace(sv{"config"}, std::make_unique<pragma::config>());

      result.emplace(
          sv{"config", "show"}, std::make_unique<pragma::config_show>());

      result.emplace(
          sv{"config", "load"}, std::make_unique<pragma::config_load>());

      result.emplace(sv{"quit"}, std::make_unique<pragma::quit>());

      return result;
    }
  }
}
