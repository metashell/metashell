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

#include "arguments.hpp"

#include <metashell/replace_part/replace_part.hpp>

#include <just/file.hpp>

#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

namespace
{
  std::string load(const boost::filesystem::path& path_)
  {
    if (path_ == "-")
    {
      std::cin >> std::noskipws;
      return std::string(
          std::istream_iterator<char>(std::cin), std::istream_iterator<char>());
    }
    else
    {
      return just::file::read<std::string>(path_.string());
    }
  }
}

int main(int argc_, const char* argv_[])
{
  try
  {
    if (const boost::optional<arguments> args = parse_arguments(argc_, argv_))
    {
      just::file::write(args->output.string(),
                        replace_part::replace(args->input, args->marker,
                                              load(args->replace_with)));
    }
    return 0;
  }
  catch (const std::exception& err)
  {
    std::cerr << "Error: " << err.what() << std::endl;
    return 1;
  }
}
