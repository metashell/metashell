#ifndef METASHELL_SYSTEM_TEST_FILENAME_SET_HPP
#define METASHELL_SYSTEM_TEST_FILENAME_SET_HPP

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
#include <set>

namespace metashell
{
  namespace system_test
  {
    class filename_set : boost::equality_comparable<filename_set, json_string>,
                         boost::equality_comparable<filename_set>
    {
    public:
      typedef std::set<boost::filesystem::path>::const_iterator const_iterator;
      typedef const_iterator iterator;

      explicit filename_set();
      explicit filename_set(const json_string& s_);

      template <class Container>
      filename_set(const Container& c_) : _paths(c_.begin(), c_.end())
      {
      }

      filename_set(std::initializer_list<boost::filesystem::path> paths_);

      const_iterator begin() const;
      const_iterator end() const;

      bool operator==(const filename_set& fs_) const;

    private:
      std::set<boost::filesystem::path> _paths;
    };

    std::ostream& operator<<(std::ostream& out_,
                             const filename_set& filenames_);

    json_string to_json_string(const filename_set& filenames_);

    bool operator==(const filename_set& filenames_, const json_string& s_);
  } // namespace system_test
} // namespace metashell

#endif
