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

#include <metashell_system_test/run_metashell.hpp>
#include <metashell_system_test/json_generator.hpp>
#include <metashell_system_test/system_test_config.hpp>
#include <metashell_system_test/prompt.hpp>
#include <metashell_system_test/raw_text.hpp>

#include <just/process.hpp>

#include <cassert>
#include <sstream>
#include <iostream>

using namespace metashell_system_test;

namespace
{
  void pop_item(const std::string& item_, std::vector<std::string>& v_)
  {
    assert(!v_.empty());
    assert(v_.back() == item_);
    v_.pop_back();
  }

  std::string join(const std::vector<json_string>& commands_)
  {
    std::ostringstream s;
    bool first = true;
    for (const json_string& js : commands_)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        s << '\n';
      }
      s << js;
    }
    return s.str();
  }

  void split_at_new_lines(const std::string& s_, std::vector<std::string>& out_)
  {
    bool was_r = false;
    auto from = s_.begin();
    for (auto i = s_.begin(), e = s_.end(); i != e; ++i)
    {
      switch (*i)
      {
      case '\r':
        if (was_r)
        {
          out_.push_back(std::string(from, i - 1));
          from = i;
          // was_r remains true
        }
        else
        {
          was_r = true;
        }
        break;
      case '\n':
        if (was_r)
        {
          out_.push_back(std::string(from, i - 1));
          was_r = false;
        }
        else
        {
          out_.push_back(std::string(from, i));
        }
        from = i + 1;
        break;
      default:
        if (was_r)
        {
          out_.push_back(std::string(from, i - 1));
          from = i;
          was_r = false;
        }
      }
    }

    assert(!was_r || from != s_.end());

    out_.push_back(std::string(from, was_r ? s_.end() - 1 : s_.end()));
  }
}

std::vector<json_string> metashell_system_test::run_metashell(
  const std::vector<json_string>& commands_,
  const std::vector<std::string>& extra_args_
)
{
  using just::process::run;

  std::vector<std::string>
    cmd{system_test_config::metashell_binary(), "--console=json", "--nosplash"};
  cmd.insert(
    cmd.end(),
    system_test_config::metashell_args().begin(),
    system_test_config::metashell_args().end()
  );
  cmd.insert(cmd.end(), extra_args_.begin(), extra_args_.end());

  std::vector<std::string> rsp;
  split_at_new_lines(run(cmd, join(commands_)).standard_output(), rsp);

  // The result of the new line at the end of the last response
  pop_item("", rsp);

  // The result of the end of the input
  pop_item(to_json_string(raw_text("")).get(), rsp);
  pop_item(to_json_string(prompt(">")).get(), rsp);

  std::vector<json_string> jv;
  jv.reserve(rsp.size());
  for (const std::string& s : rsp)
  {
    jv.push_back(json_string(s));
  }

  return jv;
}

json_string metashell_system_test::run_metashell_command(
  const std::string& command_
)
{
  const auto r = run_metashell({command(command_)});

  assert(r.size() == 2);
  assert(r.front() == to_json_string(prompt(">")));

  return r.back();
}


