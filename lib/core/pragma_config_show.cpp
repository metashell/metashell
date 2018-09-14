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
#include <metashell/core/pragma_config_show.hpp>
#include <metashell/core/shell.hpp>

#include <metashell/data/exception.hpp>

#include <algorithm>

namespace metashell
{
  namespace core
  {
    pragma_config_show::pragma_config_show(shell& shell_) : _shell(shell_) {}

    iface::pragma_handler* pragma_config_show::clone() const
    {
      return new pragma_config_show(_shell);
    }

    std::string pragma_config_show::arguments() const { return "<name>"; }

    std::string pragma_config_show::description() const
    {
      return "Shows the details of a config.";
    }

    void pragma_config_show::run(const data::command::iterator&,
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
        comment_json_writer w(displayer_);
        display(w, *cfg);
      }
    }
  }
}
