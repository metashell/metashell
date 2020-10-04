// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/comment.hpp>
#include <metashell/system_test/error.hpp>
#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/system_test_config.hpp>

#include <just/lines.hpp>

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

using namespace metashell::system_test;
using namespace metashell;
using pattern::_;

namespace
{
  void parse_arguments(int argc_, const char* argv_[])
  {
    if (argc_ < 2)
    {
      throw std::runtime_error(std::string("Usage: ") + argv_[0] +
                               " <metashell binary> <metashell args>");
    }

    system_test_config::metashell_binary(data::executable_path(argv_[1]));
    for (auto i = 2; i != argc_; ++i)
    {
      system_test_config::metashell_arg(data::command_line_argument(argv_[i]));
    }
  }

  std::string parse_config_name(const std::string& line_)
  {
    return (boost::algorithm::starts_with(line_, " * ") ||
            boost::algorithm::starts_with(line_, "   ")) ?
               line_.substr(3) :
               line_;
  }

  std::vector<std::string> configs(metashell_instance& mi_)
  {
    const auto response = mi_.command("#msh all config");
    if (response.empty())
    {
      throw std::runtime_error(
          "Unexpected empty response for \"#msh all config\"");
    }

    const comment all_config{response.front()};
    if (all_config.paragraphs().size() != 1)
    {
      throw std::runtime_error("Invalid response from \"#msh all config\": " +
                               response.front().get());
    }

    std::vector<std::string> result;
    const just::lines::basic_view<std::string> lines{
        all_config.paragraphs().front().content()};
    std::transform(lines.begin(), lines.end(), std::back_inserter(result),
                   parse_config_name);

    return result;
  }

  std::optional<std::string> command_fails(const std::string& command_,
                                           metashell_instance& mi_)
  {
    for (const json_string& result : mi_.command(command_))
    {
      if (error(_) == result)
      {
        return "Error running command \"" + command_ +
               "\": " + error(result).message();
      }
    }
    return std::nullopt;
  }

  std::optional<std::string> config_fails(const std::string& config_,
                                          metashell_instance& mi_)
  {
    if (const auto failure = command_fails("#msh config load " + config_, mi_))
    {
      return failure;
    }

    if (const auto failure = command_fails("#msh engine switch wave", mi_))
    {
      return failure;
    }

    return command_fails("#msh engine switch internal", mi_);
  }
} // namespace

int main(int argc_, const char* argv_[])
{
  try
  {
    parse_arguments(argc_, argv_);

    metashell_instance mi;

    const auto cfgs = configs(mi);
    for (size_t i = 0; i != cfgs.size(); ++i)
    {
      const auto& config = cfgs[i];

      std::cout << "Checking config [" << i << "/" << cfgs.size() << "] "
                << config << ": ";
      const std::optional<std::string> failure = config_fails(config, mi);
      std::cout << (failure ? "FAIL\n" : "OK\n");
      if (failure)
      {
        std::cout << *failure << "\n";
        const std::vector<json_string> cfg =
            mi.command("#msh config show " + config);
        if (cfg.empty())
        {
          std::cout << "Error fetching the config details: no response from "
                       "Metashell\n";
        }
        else if (error(_) == cfg.front())
        {
          std::cout << "Error fetching the config details: "
                    << error(cfg.front()).message() << "\n";
        }
        else if (comment(_) == cfg.front())
        {
          std::cout << "Config:\n";
          const comment cmt(cfg.front());
          for (const paragraph& p : cmt.paragraphs())
          {
            std::cout << p.content() << "\n";
          }
        }
        else
        {
          std::cout << "Error fetching the config details: " << cfg.front()
                    << "\n";
        }
        return 1;
      }
    }

    return 0;
  }
  catch (const std::exception& err)
  {
    std::cerr << err.what() << "\n";
    return 1;
  }
}
