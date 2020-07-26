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

#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/command_line_argument_list.hpp>

#include <boost/filesystem/path.hpp>

#include <optional>
#include <string>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    std::optional<std::string> try_to_remove_prefix(const std::string& prefix_,
                                                    const std::string& s_);
    std::string remove_prefix(const std::string& prefix_,
                              const std::string& s_);

    std::optional<std::string>
    try_to_remove_prefix_suffix(const std::string& prefix_,
                                const std::string& s_,
                                const std::string& suffix_);

    std::optional<boost::filesystem::path>
    include_path_addition(const data::command_line_argument& arg_);

    std::string c_string_literal(const std::string& s_);

    template <class T1, class T2>
    bool matches(const std::optional<T1>& a_, const std::optional<T2>& b_)
    {
      return !a_ || !b_ || *a_ == *b_;
    }

    std::string new_line();

    void write_file(const boost::filesystem::path&, const std::string&);

    json_string test_config(const std::string& name_);

    template <size_t PrefixLen>
    std::vector<data::command_line_argument_list>
    with_and_without_prefix(const char (&prefix_)[PrefixLen],
                            const boost::filesystem::path& arg_)
    {
      return {data::command_line_argument_list{prefix_, arg_.string()},
              data::command_line_argument_list{arg_.string()}};
    }
  }
}

#endif
