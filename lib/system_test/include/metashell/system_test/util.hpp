#ifndef METASHELL_SYSTEM_TEST_UTIL_HPP
#define METASHELL_SYSTEM_TEST_UTIL_HPP

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

#include <metashell/system_test/json_string.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>

namespace metashell
{
  namespace system_test
  {
    boost::optional<std::string>
    try_to_remove_prefix(const std::string& prefix_, const std::string& s_);
    std::string remove_prefix(const std::string& prefix_,
                              const std::string& s_);

    boost::optional<std::string>
    try_to_remove_prefix_suffix(const std::string& prefix_,
                                const std::string& s_,
                                const std::string& suffix_);

    boost::optional<boost::filesystem::path>
    include_path_addition(const std::string& arg_);

    std::string c_string_literal(const std::string& s_);

    template <class T1, class T2>
    bool matches(const boost::optional<T1>& a_, const boost::optional<T2>& b_)
    {
      return !a_ || !b_ || *a_ == *b_;
    }

    std::string new_line();

    void write_file(const boost::filesystem::path&, const std::string&);

    json_string test_config(const std::string& name_);
  }
}

#endif
