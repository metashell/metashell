// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/json_generator.hpp>
#include <metashell/system_test/process_execution.hpp>

#include <metashell/process/run.hpp>

#include <iostream>

using namespace metashell::system_test;

process_execution::process_execution(std::vector<std::string> cmd_,
                                     std::string stdin_,
                                     metashell::data::process_output result_)
  : _cmd(move(cmd_)), _stdin(move(stdin_)), _result(std::move(result_))
{
}

const std::vector<std::string>& process_execution::cmd() const { return _cmd; }

const std::string& process_execution::standard_input() const { return _stdin; }

const std::string& process_execution::standard_output() const
{
  return _result.standard_output();
}

const std::string& process_execution::standard_error() const
{
  return _result.standard_error();
}

metashell::data::exit_code_t process_execution::exit_code() const
{
  return _result.exit_code();
}

process_execution metashell::system_test::run(std::vector<std::string> cmd_,
                                              std::string input_)
{
  return process_execution(
      move(cmd_), move(input_), metashell::process::run(cmd_, input_));
}

process_execution
metashell::system_test::run(std::vector<std::string> cmd_,
                            const boost::filesystem::path& cwd_,
                            std::string input_)
{
  return process_execution(
      move(cmd_), move(input_),
      metashell::process::run(cmd_, input_, cwd_.string()));
}

std::ostream& metashell::system_test::operator<<(std::ostream& out_,
                                                 const process_execution& e_)
{
  return out_ << "Command: " << to_json(e_.cmd()) << std::endl
              << std::endl
              << "Standard input: " << to_json(e_.standard_input()) << std::endl
              << std::endl
              << "Standard output: " << to_json(e_.standard_output())
              << std::endl
              << std::endl
              << "Standard error: " << to_json(e_.standard_error()) << std::endl
              << std::endl
              << "Exit code: " << e_.exit_code() << std::endl;
}
