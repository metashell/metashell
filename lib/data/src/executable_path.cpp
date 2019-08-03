// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/exception.hpp>
#include <metashell/data/executable_path.hpp>

#include <boost/filesystem.hpp>

#include <iostream>

namespace metashell
{
  namespace data
  {
    executable_path::executable_path(std::string path_)
      : _value(std::move(path_))
    {
      if (_value.empty())
      {
        throw data::exception("The path of an executable should not be empty.");
      }
    }

    executable_path::executable_path(const char* path_)
      : executable_path(std::string(path_))
    {
    }

    executable_path::executable_path(const boost::filesystem::path& path_)
      : executable_path(path_.string())
    {
    }

    executable_path::executable_path(const command_line_argument& value_)
      : executable_path(value_.value())
    {
    }

    const char* executable_path::c_str() const { return _value.c_str(); }

    boost::filesystem::path executable_path::value() const { return _value; }

    const std::string& executable_path::as_string() const { return _value; }

    bool operator==(const executable_path& lhs_, const executable_path& rhs_)
    {
      return lhs_.value() == rhs_.value();
    }

    std::ostream& operator<<(std::ostream& out_, const executable_path& exe_)
    {
      return out_ << to_string(exe_);
    }

    std::string to_string(const executable_path& exe_)
    {
      return exe_.as_string();
    }

    executable_path absolute(const executable_path& exe_)
    {
      return executable_path(absolute(exe_.value()));
    }

    std::string operator+(const executable_path& lhs_, const std::string& rhs_)
    {
      return to_string(lhs_) + rhs_;
    }

    std::string operator+(const std::string& lhs_, const executable_path& rhs_)
    {
      return lhs_ + to_string(rhs_);
    }
  }
}
