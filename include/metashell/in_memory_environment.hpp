#ifndef METASHELL_IN_MEMORY_ENVIRONMENT_HPP
#define METASHELL_IN_MEMORY_ENVIRONMENT_HPP

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

#include <metashell/environment.hpp>
#include <metashell/headers.hpp>

#include <string>

namespace metashell
{
  class config;

  class in_memory_environment : public environment
  {
  public:
    in_memory_environment(
      const std::string& internal_dir_,
      const config& config_,
      const std::string& clang_extra_arg_ = ""
    );

    virtual void append(const std::string& s_);
    virtual std::string get() const;
    virtual std::string get_appended(const std::string& s_) const;

    virtual std::string internal_dir() const;

    virtual std::vector<std::string>& clang_arguments();
    virtual const std::vector<std::string>& clang_arguments() const;

    virtual const headers& get_headers() const;

    void add_clang_arg(const std::string& arg_);

    virtual std::string get_all() const;
  private:
    std::string _buffer;
    headers _headers;
    std::vector<std::string> _clang_args;
  };
}

#endif


