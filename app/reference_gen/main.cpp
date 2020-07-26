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

#include "cmd_t.hpp"
#include "commands.hpp"
#include "file_cache.hpp"

#include <metashell/data/arg_parser.hpp>

#include <metashell/core/stdout_console.hpp>

#include <boost/filesystem.hpp>

#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/max_element.hpp>

#include <functional>
#include <iostream>
#include <stdexcept>

int main(int argc_, const char* argv_[])
{
  boost::filesystem::path docs_dir;

  bool display_help = false;
  bool test = false;

  metashell::data::arg_parser parser{};
  // clang-format off
  parser
    .flag<false>("-h", "--help", "Display help", display_help)
    .flag<false>(
      "-t", "--test",
      "Don't update, just validate the generated content.",
      test
    )
    .with_value(
      "-d", "--docs_dir",
      "Path to the docs dir of the source code.",
      docs_dir
    )
  ;
  // clang-format on

  const auto show_help = [&parser, argv_](std::ostream& out_) {
    out_ << "Usage:\n"
         << "  " << argv_[0] << " <options>\n"
         << "\n"
         << unformat(
                parser.description(metashell::core::stdout_console{}.width()))
         << "\n";
  };

  try
  {
    parser.parse(metashell::data::command_line_argument_list{argc_, argv_});

    if (display_help)
    {
      show_help(std::cout);
      return 0;
    }

    if (docs_dir.empty())
    {
      throw std::runtime_error("Please specify the -d <docs dir> argument");
    }

    const std::vector<cmd_t> commands = get_commands(docs_dir);

    file_cache files(commands);

    for (const auto& cmd : commands)
    {
      files[cmd.path()] = cmd.generate(files[cmd.path()]);
    }

    if (files.changed())
    {
      if (test)
      {
        const std::vector<std::string> lines{
            "Automatically generated documentation is out of date.",
            "Please run the following command:",
            "\"" + canonical(boost::filesystem::path(argv_[0])).string() +
                "\" -d \"" + canonical(docs_dir).string() + "\""};

        const auto max_len = *boost::range::max_element(
            lines |
            boost::adaptors::transformed(
                std::function<std::string::size_type(const std::string&)>(
                    [](const std::string& s_) { return s_.size(); })));
        const std::string stars(max_len + 4, '*');

        std::cerr << stars << std::endl;
        std::cerr << stars << std::endl;
        for (const auto& line : lines)
        {
          std::cerr << "* " << line << std::string(max_len - line.size(), ' ')
                    << " *" << std::endl;
        }
        std::cerr << stars << std::endl;
        std::cerr << stars << std::endl;
        return 2;
      }
      else
      {
        files.write_changes();
      }
    }

    return 0;
  }
  catch (std::exception& e_)
  {
    std::cerr << "Error: " << e_.what() << "\n";
    show_help(std::cerr);
    std::cerr << "\n";
    return 1;
  }
}
