#ifndef METASHELL_HEADER_FILE_ENVIRONMENT_HPP
#define METASHELL_HEADER_FILE_ENVIRONMENT_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/in_memory_environment.hpp>
#include <metashell/headers.hpp>

#include <just/temp.hpp>

namespace metashell
{
  class header_file_environment : public environment
  {
  public:
    header_file_environment();

    virtual void append(const std::string& s_);
    virtual std::string get() const;
    virtual std::string get_appended(const std::string& s_) const;

    virtual std::string internal_dir() const;

    virtual const std::vector<std::string>& extra_clang_arguments() const;

    virtual const headers& get_headers() const;
  private:
    just::temp::directory _dir;
    std::vector<std::string> _extra_clang_args;
    in_memory_environment _buffer;

    void save();
  };
}

#endif


