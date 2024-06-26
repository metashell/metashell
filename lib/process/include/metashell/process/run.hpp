#ifndef METASHELL_PROCESS_RUN_HPP
#define METASHELL_PROCESS_RUN_HPP

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

#include <metashell/data/command_line.hpp>
#include <metashell/data/process_output.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace process
  {
    data::process_output
    run(const data::command_line&,
        const std::string& input_,
        const boost::filesystem::path& cwd_ = boost::filesystem::path());
  }
} // namespace metashell

#endif
