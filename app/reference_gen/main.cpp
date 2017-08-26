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

#include <metashell/available_engines.hpp>
#include <metashell/data/markdown_string.hpp>
#include <metashell/engine_constant.hpp>
#include <metashell/mdb_command_handler_map.hpp>
#include <metashell/mdb_shell.hpp>
#include <metashell/pragma_handler_map.hpp>
#include <metashell/shell.hpp>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <boost/filesystem/path.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace
{
  void show_markdown(const std::vector<std::string>& name_,
                     const metashell::pragma_handler& h_,
                     std::ostream& out_)
  {
    using boost::algorithm::join;

    const std::string args = h_.arguments();

    out_ << "* __`#msh " << join(name_, " ") << (args.empty() ? "" : " ")
         << args << "`__ <br />\n"
         << h_.description() << "\n\n";
  }

  void show_pragma_help(std::ostream& out_)
  {
    const metashell::data::config cfg{};
    metashell::command_processor_queue cpq;
    const std::string internal_dir;
    const boost::filesystem::path mdb_temp_dir;
    metashell::shell sh(cfg, cpq, internal_dir, "", mdb_temp_dir,
                        metashell::create_failing_engine());
    const metashell::pragma_handler_map m =
        metashell::pragma_handler_map::build_default(
            sh, &cpq, mdb_temp_dir, nullptr);

    for (const auto& p : m)
    {
      show_markdown(p.first, p.second, out_);
    }
  }

  template <bool Preprocessor>
  void show_mdb_help(std::ostream& out_)
  {
    using boost::algorithm::join;
    using boost::algorithm::replace_all_copy;

    metashell::mdb_command_handler_map::commands_t commands =
        metashell::mdb_shell::build_command_handler(Preprocessor)
            .get_commands();

    for (const metashell::mdb_command& cmd : commands)
    {
      out_ << "* __`" << join(cmd.get_keys(), "|") << " " << cmd.get_usage()
           << "`__ <br />\n"
           << cmd.get_short_description();
      if (!cmd.get_long_description().empty())
      {
        out_ << " <br />" << '\n'
             << replace_all_copy(cmd.get_long_description(), "\n", "\n  ");
      }
      out_ << "\n\n";
    }
  }

  void show_engine_help(std::ostream& out_)
  {
    const auto engines = metashell::available_engines();

    for (const auto& engine : engines)
    {
      out_ << "* " << metashell::data::self_id(engine.first) << "<br />\n"
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
    const auto engines = metashell::available_engines();
    const auto features = metashell::data::feature::all();

    std::vector<metashell::data::markdown_string> header{
        metashell::data::markdown_string("Feature")};
    for (const auto& engine : engines)
    {
      header.push_back(metashell::data::self_reference(engine.first));
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

int main(int argc_, const char* argv_[])
{
  typedef std::function<void(std::ostream&)> cmd_f;
  typedef std::tuple<const char*, const char*, cmd_f> cmd_t;

  boost::program_options::options_description desc("Options");

  const std::vector<cmd_t> commands{
      cmd_t{"help", "Display help",
            cmd_f([&desc](std::ostream& out_) { out_ << desc << std::endl; })},
      cmd_t{"show_pragma_help", "Display help for pragmas in MarkDown format.",
            cmd_f(&show_pragma_help)},
      cmd_t{"show_mdb_help",
            "Display help for mdb commands in MarkDown format.",
            cmd_f(&show_mdb_help<false>)},
      cmd_t{"show_pdb_help",
            "Display help for pdb commands in MarkDown format.",
            cmd_f(&show_mdb_help<true>)},
      cmd_t{"show_engine_help",
            "Display help for all engines in MarkDown format.",
            cmd_f(&show_engine_help)},
      cmd_t{"show_feature_help",
            "Display help for all features in MarkDown format.",
            cmd_f(&show_feature_help)},
      cmd_t{"show_engine_features",
            "Display a table showing which engine supports which features in "
            "MarkDown format.",
            cmd_f(&show_engine_features)}};

  for (const auto& cmd : commands)
  {
    desc.add_options()(std::get<0>(cmd), std::get<1>(cmd));
  }

  boost::filesystem::path outfile("-");
  desc.add_options()("output,o", boost::program_options::value(&outfile),
                     "Output file. Default: -, which is stdout");

  try
  {
    boost::program_options::variables_map vm;
    store(parse_command_line(argc_, argv_, desc), vm);
    notify(vm);

    std::ofstream outf;
    if (outfile != "-")
    {
      outf.open(outfile.string());
    }

    for (const auto& cmd : commands)
    {
      if (vm.count(std::get<0>(cmd)))
      {
        std::get<2>(cmd)(outfile == "-" ? std::cout : outf);
      }
    }

    return 0;
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << std::endl << desc << std::endl;
    return 1;
  }
}
