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

#include <metashell/iface/environment.hpp>
#include <metashell/iface/engine.hpp>
#include <metashell/data/headers.hpp>
#include <metashell/data/config.hpp>

namespace metashell
{
  class header_file_environment : public iface::environment
  {
  public:
    header_file_environment(
      iface::engine& engine_,
      const data::config& config_,
      const std::string& internal_dir_,
      const std::string& env_filename_
    );

    virtual void append(const std::string& s_) override;
    virtual std::string get() const override;
    virtual std::string get_appended(const std::string& s_) const override;

    virtual std::string internal_dir() const override;

    virtual const data::headers& get_headers() const override;

    virtual std::string get_all() const override;

  private:
    std::string _internal_dir;
    std::string _env_filename;
    std::string _buffer;
    data::headers _headers;

    bool _use_precompiled_headers;

    iface::engine& _engine;

    void save();
  };
}

#endif


