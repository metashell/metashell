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

#include <metashell/system_test/metashell_instance.hpp>
#include <metashell/system_test/util.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <map>

using namespace metashell::system_test;

namespace
{
  std::vector<std::pair<std::string, std::string>> include_definitions()
  {
    if (using_msvc())
    {
      return {{"/I", ""}, {"\"/I", "\""}, {"/I\"", "\""}};
    }
    else
    {
      return {{"-I", ""}};
    }
  }
}

std::string metashell::system_test::remove_prefix(const std::string& prefix_,
                                                  const std::string& s_)
{
  if (const auto p = try_to_remove_prefix(prefix_, s_))
  {
    return *p;
  }
  else
  {
    throw std::runtime_error("String \"" + s_ + "\" has no \"" + prefix_ +
                             "\" prefix");
  }
}

boost::optional<std::string>
metashell::system_test::try_to_remove_prefix(const std::string& prefix_,
                                             const std::string& s_)
{
  return try_to_remove_prefix_suffix(prefix_, s_, "");
}

boost::optional<std::string>
metashell::system_test::try_to_remove_prefix_suffix(const std::string& prefix_,
                                                    const std::string& s_,
                                                    const std::string& suffix_)
{
  const auto pre_suff_size = prefix_.size() + suffix_.size();
  if (s_.size() >= pre_suff_size &&
      boost::algorithm::starts_with(s_, prefix_) &&
      boost::algorithm::ends_with(s_, suffix_))
  {
    return s_.substr(prefix_.size(), s_.size() - pre_suff_size);
  }
  else
  {
    return boost::none;
  }
}

boost::optional<boost::filesystem::path>
metashell::system_test::include_path_addition(const std::string& arg_)
{
  for (const auto& def : include_definitions())
  {
    if (const auto path =
            try_to_remove_prefix_suffix(def.first, arg_, def.second))
    {
      return boost::filesystem::path(*path);
    }
  }
  return boost::none;
}
