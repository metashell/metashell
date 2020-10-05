#ifndef REPLACE_PART_REPLACE_PART_HPP
#define REPLACE_PART_REPLACE_PART_HPP

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

#include <iosfwd>
#include <string>

namespace replace_part
{
  void replace(const std::string& input_,
               const std::string& marker_,
               const std::string& replace_with_,
               std::ostream& out_);

  void replace(const boost::filesystem::path& input_,
               const std::string& marker_,
               const std::string& replace_with_,
               std::ostream& out_);

  std::string replace(const std::string& input_,
                      const std::string& marker_,
                      const std::string& replace_with_);

  std::string replace(const boost::filesystem::path& input_,
                      const std::string& marker_,
                      const std::string& replace_with_);

  inline void replace(const char* input_,
                      const std::string& marker_,
                      const std::string& replace_with_,
                      std::ostream& out_)
  {
    replace(std::string(input_), marker_, replace_with_, out_);
  }

  inline std::string replace(const char* input_,
                             const std::string& marker_,
                             const std::string& replace_with_)
  {
    return replace(std::string(input_), marker_, replace_with_);
  }
} // namespace replace_part

#endif
