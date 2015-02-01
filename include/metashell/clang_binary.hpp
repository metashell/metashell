#ifndef METASHELL_CLANG_BINARY_HPP
#define METASHELL_CLANG_BINARY_HPP

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

#include <metashell/logger.hpp>

#include <just/process.hpp>

#include <string>
#include <vector>

namespace metashell
{
  class clang_binary
  {
  public:
    clang_binary(const std::string& path_, logger* logger_);

    just::process::output run(
        const std::vector<std::string>& args_,
        const std::string& stdin_ = "") const;
  private:
    std::string _path;
    logger* _logger;
  };

  std::vector<std::string> default_sysinclude(
    const clang_binary& clang_,
    logger* logger_
  );
}

#endif

