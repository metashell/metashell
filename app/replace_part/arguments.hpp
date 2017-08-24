#ifndef REPLACE_PART_ARGUMENTS_HPP
#define REPLACE_PART_ARGUMENTS_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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
#include <boost/optional.hpp>

#include <string>

struct arguments
{
  boost::filesystem::path input;
  boost::filesystem::path output;
  std::string marker;
  boost::filesystem::path replace_with;
};

boost::optional<arguments> parse_arguments(int argc_, const char* argv_[]);

#endif
