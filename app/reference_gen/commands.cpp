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

#include "commands.hpp"

#include <metashell/data/markdown_string.hpp>

#include <metashell/defaults/available_engines.hpp>
#include <metashell/defaults/pragma_map.hpp>

#include <metashell/mdb/command_handler_map.hpp>
#include <metashell/mdb/shell.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <cassert>
#include <iostream>

namespace
{
  void show_markdown(const metashell::data::pragma_name& name_,
                     const metashell::iface::pragma_handler& h_,
                     std::ostream& out_)
  {
    const std::string args = h_.arguments();

    out_ << "* __`#msh " << name_ << (args.empty() ? "" : " ") << args
         << "`__ <br />\n"
         << h_.description() << "\n\n";
  }

  void show_pragma_help(std::ostream& out_)
  {
    metashell::data::config cfg;

    metashell::core::command_processor_queue cpq;
    const std::string internal_dir;
    const boost::filesystem::path mdb_temp_dir;
    for (const auto& p :
         metashell::defaults::pragma_map(&cpq, mdb_temp_dir, nullptr))
    {
      assert(p.second);

      show_markdown(p.first, *p.second, out_);
    }
  }

  template <bool Preprocessor>
  void show_mdb_help(std::ostream& out_)
  {
    using boost::algorithm::join;
    using boost::algorithm::replace_all_copy;

    metashell::mdb::command_handler_map::commands_t commands =
        metashell::mdb::shell::build_command_handler(Preprocessor)
            .get_commands();

    for (const metashell::mdb::command& cmd : commands)
    {
      out_ << "* __`"
           << join(cmd.get_keys() |
                       boost::adaptors::transformed([](
                           const metashell::data::mdb_command::name_type& n_) {
                         return to_string(n_);
                       }),
                   "|")
           << " " << cmd.get_usage() << "`__ <br />\n"
           << cmd.get_short_description();
      if (!cmd.get_long_description().empty())
      {
        out_ << " <br />" << '\n'
             << replace_all_copy(cmd.get_long_description(), "\n", "\n  ");
      }
      out_ << "\n\n";
    }
  }

  metashell::data::executable_path metashell_binary()
  {
    return metashell::data::executable_path("metashell");
  }

  void show_engine_help(std::ostream& out_)
  {
    const auto engines =
        metashell::defaults::available_engines(metashell_binary());

    out_ << "* " << metashell::data::self_id("auto") << "<br />\n"
         << "Usage: `metashell --engine auto`<br />\n<br />\n"
         << metashell::data::auto_engine_description() << "<br />\n<br />\n";

    for (const auto& engine : engines)
    {
      out_ << "* " << metashell::data::self_id(to_string(engine.first))
           << "<br />\n"
           << "Usage: `metashell --engine " << engine.first;
      const auto args = engine.second.args();
      if (!args.empty())
      {
        out_ << " -- " << engine.second.args();
      }
      out_ << "`<br />\n<br />\n"
           << engine.second.description() << "<br /><br />Supported features: "
           << list_features_in_markdown(engine.second) << "<br />\n<br />\n";
    }
  }

  void show_engine_features(std::ostream& out_)
  {
    const auto engines =
        metashell::defaults::available_engines(metashell_binary());
    const auto features = metashell::data::feature::all();

    std::vector<metashell::data::markdown_string> header{
        metashell::data::markdown_string("Feature")};
    for (const auto& engine : engines)
    {
      header.push_back(
          metashell::data::self_reference(to_string(engine.first)));
    }

    std::vector<std::vector<metashell::data::markdown_string>> table;
    for (metashell::data::feature f : features)
    {
      table.push_back({metashell::data::self_reference(to_string(f))});

      for (const auto& engine : engines)
      {
        const auto& engine_features = engine.second.features();

        table.back().push_back(metashell::data::markdown_string(
            std::find(engine_features.begin(), engine_features.end(), f) ==
                    engine_features.end() ?
                "<img src=\"../../img/no.png\" width=\"20px\" />" :
                "<img src=\"../../img/yes.png\" width=\"20px\" />"));
      }
    }

    metashell::data::format_table(header, table, out_);
  }

  void show_feature_help(std::ostream& out_)
  {
    for (metashell::data::feature f : metashell::data::feature::all())
    {
      out_ << "* " << metashell::data::self_id(to_string(f)) << "<br />\n"
           << f.description() << "<br />\n<br />\n";
    }
  }
}

std::vector<cmd_t> get_commands(const boost::filesystem::path& docs_dir_)
{
  return {cmd_t{docs_dir_ / "reference/pragmas.md", "<!-- pragma_info -->",
                &show_pragma_help},
          cmd_t{docs_dir_ / "reference/mdb_commands.md", "<!-- mdb_info -->",
                &show_mdb_help<false>},
          cmd_t{docs_dir_ / "reference/pdb_commands.md", "<!-- pdb_info -->",
                &show_mdb_help<true>},
          cmd_t{docs_dir_ / "reference/engines.md", "<!-- engine_info -->",
                &show_engine_help},
          cmd_t{docs_dir_ / "reference/engines.md", "<!-- feature_info -->",
                &show_feature_help},
          cmd_t{docs_dir_ / "reference/engines.md", "<!-- feature_matrix -->",
                &show_engine_features}};
}
