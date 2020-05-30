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

#include <metashell/data/default_clang_search_path.hpp>

#include <boost/algorithm/string/join.hpp>

#include <boost/range/adaptor/transformed.hpp>

namespace metashell
{
  namespace data
  {
    namespace
    {
      const char* search_path[] = {""
#include "default_clang_search_path.hpp"
      };
    }

    default_clang_search_path::const_iterator::const_iterator(const char** pos_)
      : _pos(pos_), _value()
    {
    }

    default_clang_search_path::const_iterator::reference
        default_clang_search_path::const_iterator::operator*() const
    {
      if (!_value)
      {
        _value = *_pos;
      }
      return *_value;
    }

    default_clang_search_path::const_iterator&
        default_clang_search_path::const_iterator::operator++()
    {
      ++_pos;
      _value = std::nullopt;
      return *this;
    }

    default_clang_search_path::const_iterator&
    default_clang_search_path::const_iterator::operator+=(difference_type n_)
    {
      _pos += n_;
      _value = std::nullopt;
      return *this;
    }

    default_clang_search_path::const_iterator&
        default_clang_search_path::const_iterator::operator--()
    {
      --_pos;
      _value = std::nullopt;
      return *this;
    }

    default_clang_search_path::const_iterator&
    default_clang_search_path::const_iterator::operator-=(difference_type n_)
    {
      _pos -= n_;
      _value = std::nullopt;
      return *this;
    }

    const char** default_clang_search_path::const_iterator::at() const
    {
      return _pos;
    }

    default_clang_search_path::const_iterator
    default_clang_search_path::begin() const
    {
      return const_iterator(search_path + 1);
    }

    default_clang_search_path::const_iterator
    default_clang_search_path::end() const
    {
      return const_iterator(search_path +
                            sizeof(search_path) / sizeof(search_path[0]));
    }

    bool operator==(default_clang_search_path::const_iterator a_,
                    default_clang_search_path::const_iterator b_)
    {
      return a_.at() == b_.at();
    }

    bool operator<(default_clang_search_path::const_iterator a_,
                   default_clang_search_path::const_iterator b_)
    {
      return a_.at() < b_.at();
    }

    std::string join(default_clang_search_path p_, const std::string& sep_)
    {
      return boost::algorithm::join(
          p_ | boost::adaptors::transformed(
                   [](const boost::filesystem::path& path_) -> std::string {
                     return path_.string();
                   }),
          sep_);
    }

    default_clang_search_path::const_iterator::difference_type
    operator-(default_clang_search_path::const_iterator a_,
              default_clang_search_path::const_iterator b_)
    {
      return a_.at() - b_.at();
    }
  }
}
