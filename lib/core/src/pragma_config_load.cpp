// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/comment_json_writer.hpp>
#include <metashell/core/pragma_config_load.hpp>
#include <metashell/core/shell.hpp>

#include <metashell/data/exception.hpp>

#include <algorithm>

namespace metashell
{
  namespace core
  {
    namespace
    {
      void restore_config(const data::shell_config_name& name_, shell& shell_)
      {
        shell_.get_config().activate(name_);
        shell_.rebuild_environment();
      }
    }

    pragma_config_load::pragma_config_load(shell& shell_) : _shell(shell_) {}

    std::string pragma_config_load::arguments() const { return "<name>"; }

    std::string pragma_config_load::description() const
    {
      return "Loads the config in the shell (keeping the environment).";
    }

    void pragma_config_load::run(const data::command::iterator&,
                                 const data::command::iterator&,
                                 const data::command::iterator& args_begin_,
                                 const data::command::iterator& args_end_,
                                 iface::displayer& displayer_) const
    {
      const data::shell_config_name name = data::shell_config_name(
          tokens_to_string(args_begin_, args_end_).value());

      const auto& configs = _shell.get_config().shell_configs();

      const auto cfg = std::find_if(configs.begin(), configs.end(),
                                    [&name](const data::shell_config& cfg_) {
                                      return cfg_.name == name;
                                    });

      if (cfg == configs.end())
      {
        throw data::exception("Config " + name + " not found.");
      }
      else
      {
        const auto old_config = _shell.get_config().active_shell_config().name;
        _shell.get_config().activate(name);
        try
        {
          _shell.rebuild_environment();
          displayer_.show_comment(data::text("Switched to config " + name));
        }
        catch (const std::exception& e)
        {
          displayer_.show_error("Error loading config " + name + ": " +
                                e.what());
          restore_config(old_config, _shell);
        }
        catch (...)
        {
          displayer_.show_error("Error loading config " + name +
                                ": unknown exception");
          restore_config(old_config, _shell);
        }
      }
    }
  }
}
