// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2020, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/file_completion_entry.hpp>

#include <boost/filesystem.hpp>

namespace metashell
{
  namespace data
  {
    file_completion_entry::file_completion_entry(
        boost::filesystem::path base_path_,
        const boost::filesystem::path& full_path_,
        data::user_input completion_no_prefix_)
      : _base_path{std::move(base_path_)},
        _is_directory{boost::filesystem::is_directory(full_path_)},
        _completion_no_prefix{std::move(completion_no_prefix_)}
    {
    }

    bool file_completion_entry::is_directory() const { return _is_directory; }

    const boost::filesystem::path& file_completion_entry::base_path() const
    {
      return _base_path;
    }

    const data::user_input& file_completion_entry::completion_no_prefix() const
    {
      return _completion_no_prefix;
    }
  }
}
