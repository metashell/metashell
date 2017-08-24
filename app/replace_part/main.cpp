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

#include <boost/filesystem/path.hpp>

#include <just/file.hpp>
#include <just/lines.hpp>

#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>

namespace
{
  void do_replacement(const boost::filesystem::path& input_,
                      std::ostream& out_,
                      const std::string& marker_,
                      const std::string& replace_with_)
  {
    bool in_text_to_replace = false;

    for (const std::string& line :
         just::lines::basic_file_view<true>(input_.string()))
    {
      const bool marked_line = line.find(marker_) != std::string::npos;

      if (!in_text_to_replace || marked_line)
      {
        out_ << line;
      }

      if (marked_line)
      {
        if (!in_text_to_replace)
        {
          out_ << replace_with_ << '\n';
        }

        in_text_to_replace = !in_text_to_replace;
      }
    }
  }

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
      std::ostringstream out;
      do_replacement(args->input, out, args->marker, load(args->replace_with));
      just::file::write(args->output.string(), out.str());
    }
    return 0;
  }
  catch (const std::exception& err)
  {
    std::cerr << "Error: " << err.what() << std::endl;
    return 1;
  }
}
