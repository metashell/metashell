
// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/metashell.hpp>

#include <metashell/metadebugger_shell.hpp>

#include <metashell/temporary_file.hpp>

#include <cctype>
#include <functional>

#include <boost/algorithm/string/predicate.hpp>

namespace metashell {

metadebugger_shell::metadebugger_shell(
    const config& conf,
    environment& env,
    const std::string& args) :
  conf(conf),
  env(env),
  is_stopped(false)
{
  if (!args.empty()) {
    get_templight_trace_from_metaprogram(args);
  }
}

metadebugger_shell::~metadebugger_shell() {}

std::string metadebugger_shell::prompt() const {
  return "(mdb) ";
}

bool metadebugger_shell::stopped() const {
  return is_stopped;
}

void metadebugger_shell::line_available(const std::string& line) {
  bool has_non_whitespace = std::find_if(
      line.begin(), line.end(),
      [](char ch) { return !std::isspace(ch); }) != line.end();

  // TODO gdb repeats last command when empty line is given
  if (has_non_whitespace) {
    if (line != prev_line) {
      add_history(line);
      prev_line = line;
    }
    if (line == "ft" || line == "forwardtrace") {
      trace.print_full_forwardtrace(*this);
    } else if (boost::starts_with(line, "eval ")) {
      get_templight_trace_from_metaprogram(
          line.substr(5, std::string::npos));
    } else {
      display("Unknown command: \"" + line + "\"\n",
          just::console::color::red);
    }
  }
}

void metadebugger_shell::get_templight_trace_from_metaprogram(
    const std::string& str)
{
  temporary_file templight_xml_file("templight-%%%%-%%%%-%%%%-%%%%.xml");

  std::vector<std::string>& clang_args = env.clang_arguments();

  clang_args.push_back("-templight");
  clang_args.push_back("-templight-output");
  clang_args.push_back(templight_xml_file.get_path().string());
  clang_args.push_back("-templight-format");
  clang_args.push_back("xml");

  run_metaprogram(str);

  //TODO move this to a destructor. run_metaprogram might throw
  clang_args.erase(clang_args.end() - 5, clang_args.end());

  trace = templight_trace::create_from_xml(templight_xml_file.get_path().string());
}

void metadebugger_shell::run_metaprogram(const std::string& str) {
  result res = eval_tmp(env, str, conf, "<mdb-stdin>");

  if (!res.info.empty()) {
    std::cout << res.info;
  }

  for (const std::string& e : res.errors) {
    display(e + "\n", just::console::color::red);
  }
  if (!res.has_errors()) {
    std::cout << res.output << std::endl;
  }
}

}

