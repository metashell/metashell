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

#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <just/test.hpp>

using namespace metashell::system_test;

std::string metashell::system_test::remove_prefix(const std::string& prefix_,
                                                  const std::string& s_)
{
  const auto p = try_to_remove_prefix(prefix_, s_);
  JUST_ASSERT(bool(p));
  return *p;
}

boost::optional<std::string>
metashell::system_test::try_to_remove_prefix(const std::string& prefix_,
                                             const std::string& s_)
{
  if (boost::algorithm::starts_with(s_, prefix_))
  {
    return s_.substr(prefix_.size());
  }
  else
  {
    return boost::none;
  }
}
