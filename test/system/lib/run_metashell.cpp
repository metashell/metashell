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
#include <metashell_system_test/process_execution.hpp>

#include <cassert>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace metashell_system_test;

namespace
{
  void run_metashell_assert(const std::string& cond_,
                            bool cond_value_,
                            const process_execution& execution_,
                            const std::string& filename_,
                            int line_)
  {
    if (!cond_value_)
    {
      const char sep[] = "----------------------------------";
      std::cerr << sep << std::endl
                << "Assertion failed: " << cond_ << " at " << filename_ << ":"
                << line_ << std::endl
                << "Related child process execution:" << std::endl
                << execution_ << sep << std::endl;
      std::exit(1);
    }
  }

#ifdef rm_assert
#error rm_assert already defined
#endif
#define rm_assert(cond, execution)                                             \
  run_metashell_assert(#cond, (cond), execution, __FILE__, __LINE__)

  void pop_item(const std::string& item_,
                std::vector<std::string>& v_,
                const process_execution& execution_)
  {
    rm_assert(!v_.empty(), execution_);
    rm_assert(v_.back() == item_, execution_);
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

  process_execution execute(const std::vector<json_string>& commands_,
                            const std::vector<std::string>& extra_args_)
  {
    std::vector<std::string> cmd{
        system_test_config::metashell_binary(), "--console=json", "--nosplash"};
    cmd.insert(cmd.end(), system_test_config::metashell_args().begin(),
               system_test_config::metashell_args().end());
    if (!extra_args_.empty())
    {
      if (std::find(cmd.begin(), cmd.end(), "--") == cmd.end() ||
          extra_args_.front() != "--")
      {
        cmd.insert(cmd.end(), extra_args_.begin(), extra_args_.end());
      }
      else
      {
        cmd.insert(cmd.end(), extra_args_.begin() + 1, extra_args_.end());
      }
    }

    return run(move(cmd), join(commands_));
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

  process_execution
  run_metashell_impl(const std::vector<json_string>& commands_,
                     const std::vector<std::string>& extra_args_,
                     std::vector<json_string>& result_)
  {
    process_execution me = execute(commands_, extra_args_);

    std::vector<std::string> rsp;
    split_at_new_lines(me.standard_output(), rsp);

    // The result of the new line at the end of the last response
    pop_item("", rsp, me);

    // The result of the end of the input
    pop_item(to_json_string(raw_text("")).get(), rsp, me);
    pop_item(to_json_string(prompt(">")).get(), rsp, me);

    result_.reserve(result_.size() + rsp.size());
    for (const std::string& s : rsp)
    {
      result_.push_back(json_string(s));
    }

    return me;
  }
}

std::vector<json_string> metashell_system_test::run_metashell(
    const std::vector<json_string>& commands_,
    const std::vector<std::string>& extra_args_)
{
  std::vector<json_string> jv;
  run_metashell_impl(commands_, extra_args_, jv);
  return jv;
}

json_string
metashell_system_test::run_metashell_command(const std::string& command_)
{
  std::vector<json_string> jv;
  const auto execution = run_metashell_impl({command(command_)}, {}, jv);

  rm_assert(jv.size() == 2, execution);
  rm_assert(jv.front() == to_json_string(prompt(">")), execution);

  return jv.back();
}
