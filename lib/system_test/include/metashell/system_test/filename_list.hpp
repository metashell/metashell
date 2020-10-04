#ifndef METASHELL_SYSTEM_TEST_FILENAME_LIST_HPP
#define METASHELL_SYSTEM_TEST_FILENAME_LIST_HPP

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
#include <boost/operators.hpp>

#include <iosfwd>
#include <vector>

namespace metashell
{
  namespace system_test
  {
    class filename_list
      : boost::equality_comparable<filename_list, json_string>,
        boost::equality_comparable<filename_list>
    {
    public:
      typedef boost::filesystem::path value_type;
      typedef std::vector<value_type>::const_iterator const_iterator;
      typedef const_iterator iterator;

      explicit filename_list();
      explicit filename_list(const json_string& s_);

      template <class Container>
      filename_list(const Container& c_) : _paths(c_.begin(), c_.end())
      {
      }

      filename_list(std::initializer_list<value_type> paths_);

      const_iterator begin() const;
      const_iterator end() const;

      bool empty() const;

    private:
      std::vector<value_type> _paths;
    };

    std::ostream& operator<<(std::ostream& out_,
                             const filename_list& filenames_);

    json_string to_json_string(const filename_list& filenames_);

    bool operator==(const filename_list& filenames_, const json_string& s_);
    bool operator==(const filename_list& a_, const filename_list& b_);
  } // namespace system_test
} // namespace metashell

#endif
