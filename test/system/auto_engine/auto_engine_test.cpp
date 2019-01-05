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
  boost::optional<std::string>
  engine_selected(const boost::filesystem::path& metashell_,
                  const std::vector<std::string>& args_)
  {
    const std::string out =
        metashell::process::run(metashell_, args_, "").standard_output;

    std::regex find_engine("Log: auto engine selected ([^\\r\\n ]*)");
    std::smatch sm;
    if (regex_search(out, sm, find_engine))
    {
      return sm[1].str();
    }
    else
    {
      return boost::none;
    }
  }
}

auto_engine_test::auto_engine_test(boost::filesystem::path metashell_)
  : _metashell(std::move(metashell_))
{
}

void auto_engine_test::test_engine_selection(
    const std::vector<std::string>& args_,
    const boost::optional<std::string>& expected_engine_)
{
  using boost::algorithm::join;

  std::vector<std::string> args{
      "--engine", "auto", "--log", "-", "--console", "plain", "--"};
  args.insert(args.end(), args_.begin(), args_.end());

  const boost::optional<std::string> actual_engine =
      engine_selected(_metashell, args);

  if (expected_engine_ && !actual_engine)
  {
    throw std::runtime_error("Command \"" + _metashell.string() + " " +
                             join(args, " ") + "\" expected to use engine " +
                             *expected_engine_ + " but is not using any.");
  }
  else if (!expected_engine_ && actual_engine)
  {
    throw std::runtime_error(
        "Command \"" + _metashell.string() + " " + join(args, " ") +
        "\" is not expected to use any engine but is using " + *actual_engine +
        ".");
  }
  else if (expected_engine_ != actual_engine)
  {
    throw std::runtime_error("Command \"" + _metashell.string() + " " +
                             join(args, " ") + "\" expected to use engine " +
                             *expected_engine_ + " but is using " +
                             *actual_engine + ".");
  }
}
