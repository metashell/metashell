#ifndef METASHELL_SYSTEM_TEST_METASHELL_INSTANCE_HPP
#define METASHELL_SYSTEM_TEST_METASHELL_INSTANCE_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/system_test/json_string.hpp>
#include <metashell/system_test/process_execution.hpp>

#include <metashell/process/execution.hpp>

#include <just/environment.hpp>
#include <just/lines.hpp>

#include <boost/filesystem/path.hpp>

#include <boost/noncopyable.hpp>

#include <memory>
#include <string>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    class metashell_instance : boost::noncopyable
    {
    public:
      explicit metashell_instance(
          const std::vector<std::string>& extra_args_ = {},
          const boost::filesystem::path& cwd_ = boost::filesystem::path(),
          bool allow_user_defined_args_ = true,
          bool allow_standard_headers_ = true);

      ~metashell_instance();

      std::vector<json_string> send(const json_string& cmd_);
      std::vector<json_string> command(const std::string& cmd_);
      std::vector<json_string> code_completion(const std::string& cmd_);
      void close_stdin();

      json_string next_response();
      std::vector<json_string> responses_until_prompt();

      const process_execution& get_process_execution() const;

      const std::vector<json_string>& initial_responses() const;

    private:
      std::unique_ptr<just::environment::override_guard> _include;
      process_execution _process_execution;
      process::execution _child;
      std::unique_ptr<just::lines::basic_view<process::input_file>> _lines;
      just::lines::basic_view<process::input_file>::iterator _last_line;

      std::vector<json_string> _initial_responses;
    };

    bool using_msvc();
    bool using_wave();

    std::vector<std::string>
    with_sysincludes(std::vector<std::string> args_,
                     const std::vector<boost::filesystem::path>& paths_);
    std::vector<std::string>
    with_quoteincludes(std::vector<std::string> args_,
                       const std::vector<boost::filesystem::path>& paths_);
  }
}

#endif
