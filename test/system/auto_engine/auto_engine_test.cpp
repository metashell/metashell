// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include "auto_engine_test.hpp"

#include <metashell/process/run.hpp>

#include <boost/algorithm/string/join.hpp>

#include <regex>
#include <stdexcept>

namespace
{
  std::optional<std::string>
  engine_selected(const metashell::data::command_line& cmd_)
  {
    const std::string out = metashell::process::run(cmd_, "").standard_output;

    std::regex find_engine("Log: auto engine selected ([^\\r\\n ]*)");
    std::smatch sm;
    if (regex_search(out, sm, find_engine))
    {
      return sm[1].str();
    }
    else
    {
      return std::nullopt;
    }
  }
}

auto_engine_test::auto_engine_test(metashell::data::executable_path metashell_)
  : _metashell(std::move(metashell_))
{
}

void auto_engine_test::test_engine_selection(
    const metashell::data::command_line_argument_list& args_,
    const std::optional<std::string>& expected_engine_)
{
  using boost::algorithm::join;

  const metashell::data::command_line cmd(
      _metashell,
      metashell::data::command_line_argument_list{
          "--engine", "auto", "--log", "-", "--console", "plain", "--"} +
          args_);

  const std::optional<std::string> actual_engine = engine_selected(cmd);

  if (expected_engine_ && !actual_engine)
  {
    throw std::runtime_error("Command \"" + to_string(cmd) +
                             "\" expected to use engine " + *expected_engine_ +
                             " but is not using any.");
  }
  else if (!expected_engine_ && actual_engine)
  {
    throw std::runtime_error(
        "Command \"" + to_string(cmd) +
        "\" is not expected to use any engine but is using " + *actual_engine +
        ".");
  }
  else if (expected_engine_ != actual_engine)
  {
    throw std::runtime_error("Command \"" + to_string(cmd) +
                             "\" expected to use engine " + *expected_engine_ +
                             " but is using " + *actual_engine + ".");
  }
}
