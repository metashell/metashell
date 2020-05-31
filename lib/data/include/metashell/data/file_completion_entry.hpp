#ifndef METASHELL_DATA_FILE_COMPLETION_ENTRY_HPP
#define METASHELL_DATA_FILE_COMPLETION_ENTRY_HPP

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

#include <metashell/data/user_input.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  namespace data
  {
    class file_completion_entry
    {
    public:
      file_completion_entry(boost::filesystem::path base_path_,
                            const boost::filesystem::path& full_path_,
                            data::user_input completion_no_prefix_);

      bool is_directory() const;
      const boost::filesystem::path& base_path() const;
      const data::user_input& completion_no_prefix() const;

    private:
      boost::filesystem::path _base_path;
      bool _is_directory;
      data::user_input _completion_no_prefix;
    };
  }
}

#endif
