#ifndef METASHELL_DATA_DEFAULT_CLANG_SEARCH_PATH_HPP
#define METASHELL_DATA_DEFAULT_CLANG_SEARCH_PATH_HPP

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

#include <boost/filesystem/path.hpp>

#include <boost/operators.hpp>

#include <optional>

namespace metashell
{
  namespace data
  {
    class default_clang_search_path
    {
    public:
      typedef boost::filesystem::path value_type;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
      typedef const value_type& const_reference;
      typedef const_reference reference;
      typedef const value_type* const_pointer;
      typedef const_pointer pointer;

      class const_iterator : boost::random_access_iterator_helper<
                                 const_iterator,
                                 default_clang_search_path::difference_type,
                                 default_clang_search_path::value_type,
                                 default_clang_search_path::pointer,
                                 default_clang_search_path::reference>
      {
      public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef default_clang_search_path::value_type value_type;
        typedef default_clang_search_path::difference_type difference_type;
        typedef default_clang_search_path::reference reference;
        typedef default_clang_search_path::pointer pointer;

        explicit const_iterator(const char**);

        reference operator*() const;

        const_iterator& operator++();
        const_iterator& operator+=(difference_type);

        const_iterator& operator--();
        const_iterator& operator-=(difference_type);

        const char** at() const;

      private:
        const char** _pos;
        mutable std::optional<boost::filesystem::path> _value;
      };

      typedef const_iterator iterator;

      const_iterator begin() const;
      const_iterator end() const;
    };

    default_clang_search_path::const_iterator::difference_type
    operator-(default_clang_search_path::const_iterator,
              default_clang_search_path::const_iterator);

    bool operator==(default_clang_search_path::const_iterator,
                    default_clang_search_path::const_iterator);
    bool operator<(default_clang_search_path::const_iterator,
                   default_clang_search_path::const_iterator);

    std::string join(default_clang_search_path, const std::string&);
  }
}

#endif
