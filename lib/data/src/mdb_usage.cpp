// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/mdb_usage.hpp>

namespace metashell
{
  namespace data
  {
    std::string mdb_usage(bool preprocessor_)
    {
      return std::string(preprocessor_ ? "[-profile]" : "[-full|-profile]") +
             " [-nocache] [" + (preprocessor_ ? "<expression>" : "<type>") +
             "|-]";
    }
  } // namespace data
} // namespace metashell
