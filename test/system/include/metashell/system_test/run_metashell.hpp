#ifndef METASHELL_SYSTEM_TEST_RUN_METASHELL_HPP
#define METASHELL_SYSTEM_TEST_RUN_METASHELL_HPP

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

#include <metashell/system_test/json_string.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    std::vector<json_string>
    run_metashell(const std::vector<json_string>& commands_,
                  const std::vector<std::string>& extra_args_ = {});

    class in_directory
    {
    public:
      explicit in_directory(boost::filesystem::path cwd_);

      std::vector<json_string>
      run_metashell(const std::vector<json_string>& commands_,
                    const std::vector<std::string>& extra_args_ = {}) const;

    private:
      boost::filesystem::path _cwd;
    };

    json_string run_metashell_command(const std::string& command_);
  }
}

#endif
