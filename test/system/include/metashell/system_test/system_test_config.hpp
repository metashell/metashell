#ifndef METASHELL_SYSTEM_TEST_CONFIG_HPP
#define METASHELL_SYSTEM_TEST_CONFIG_HPP

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

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    namespace system_test_config
    {
      void metashell_binary(const boost::filesystem::path& path_);
      void metashell_arg(const std::string& arg_);

      boost::filesystem::path metashell_binary();
      const std::vector<std::string>& metashell_args();
    }
  }
}

#endif
