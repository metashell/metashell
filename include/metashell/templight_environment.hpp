
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

#include <metashell/logger.hpp>
#include <metashell/header_file_environment.hpp>

namespace metashell {

class templight_environment : public header_file_environment {
public:
  explicit templight_environment(
    const config& config,
    logger* logger_ = nullptr
  );

  // This should be called before the first evaluation
  // with this environment
  void set_output_location(const std::string& output_location);

private:
  // Indexes into clang_arguments()
  std::size_t output_path_index;
};

}
