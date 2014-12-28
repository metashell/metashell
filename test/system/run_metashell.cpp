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

#include "run_metashell.hpp"
#include "json_generator.hpp"
#include "system_test_config.hpp"

#include <just/process.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>

#include <cassert>

namespace
{
  void pop_item(const std::string& item_, std::vector<std::string>& v_)
  {
    assert(!v_.empty());
    assert(v_.back() == item_);
    v_.pop_back();
  }
}

std::vector<std::string> run_metashell(
  std::initializer_list<std::string> commands_
)
{
  using boost::algorithm::join;
  using boost::algorithm::is_any_of;
  using boost::algorithm::split;

  using just::process::run;

  const std::vector<std::string>
    cmd{system_test_config::metashell_binary(), "--console=json", "--nosplash"};

  std::vector<std::string> rsp;
  split(
    rsp,
    run(cmd, join(commands_, "\n")).standard_output(),
    is_any_of("\n")
  );

  // The result of the new line at the end of the last response
  pop_item("", rsp);

  // The result of the end of the input
  pop_item(raw_text(""), rsp);
  pop_item(prompt(">"), rsp);

  return rsp;
}

std::string run_metashell_command(const std::string& command_)
{
  const auto r = run_metashell({command(command_)});

  assert(r.size() == 2);
  assert(r.front() == prompt(">"));

  return r.back();
}


