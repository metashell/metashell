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

#include <metashell/data/process_output.hpp>

#include <boost/algorithm/string/replace.hpp>

namespace
{
  std::string dos2unix(std::string s_)
  {
    boost::algorithm::replace_all(s_, "\r\n", "\n");
    return s_;
  }
}

namespace metashell
{
  namespace data
  {
    process_output dos2unix(process_output o_)
    {
      return {o_.status, ::dos2unix(move(o_.standard_output)),
              ::dos2unix(move(o_.standard_error))};
    }

    bool exit_success(const process_output& o_)
    {
      return exit_success(o_.status);
    }
  }
}
