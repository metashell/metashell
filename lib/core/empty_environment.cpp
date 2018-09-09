// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/empty_environment.hpp>

namespace metashell
{
  namespace core
  {
    empty_environment::empty_environment(
        const boost::filesystem::path& internal_dir_)
      : _headers(internal_dir_)
    {
    }

    void empty_environment::append(const data::cpp_code&)
    {
      // ignore
    }

    data::cpp_code empty_environment::get() const { return data::cpp_code(); }

    data::cpp_code empty_environment::get_appended(const data::cpp_code&) const
    {
      return data::cpp_code();
    }

    const data::headers& empty_environment::get_headers() const
    {
      return _headers;
    }

    data::cpp_code empty_environment::get_all() const
    {
      return data::cpp_code();
    }
  }
}
