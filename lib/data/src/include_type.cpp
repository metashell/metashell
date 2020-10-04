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

#include <metashell/data/include_type.hpp>

namespace metashell
{
  namespace data
  {
    std::string to_string(include_type type_)
    {
      switch (type_)
      {
      case include_type::sys:
        return "sys";
      case include_type::quote:
        return "quote";
      }
      return ""; // to avoid warnings about the missing return on some
      // compilers.
    }

    std::string include_code(include_type type_,
                             const boost::filesystem::path& path_)
    {
      switch (type_)
      {
      case include_type::sys:
        return "<" + path_.string() + ">";
      case include_type::quote:
        return "\"" + path_.string() + "\"";
      }
      return ""; // to avoid warnings about the missing return on some
      // compilers.
    }
  } // namespace data
} // namespace metashell
