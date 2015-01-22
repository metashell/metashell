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
#include <metashell/data/headers.hpp>

#include <just/temp.hpp>

namespace metashell
{
  class header_file_environment : public iface::environment
  {
  public:
    header_file_environment(const config& config_, logger* logger_);

    virtual void append(const std::string& s_) override;
    virtual std::string get() const override;
    virtual std::string get_appended(const std::string& s_) const override;

    virtual std::string internal_dir() const override;

    virtual std::vector<std::string>& clang_arguments() override;
    virtual const std::vector<std::string>& clang_arguments() const override;

    virtual const data::headers& get_headers() const override;

    virtual std::string get_all() const override;
  private:
    just::temp::directory _dir;
    in_memory_environment _buffer;
    std::vector<std::string> _clang_args;
    data::headers _empty_headers;

    bool _use_precompiled_headers;
    std::string _clang_path;

    void save();
    std::string env_filename() const;
  };
}

#endif


