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

#include <metashell/data/headers.hpp>
#include <metashell/data/shell_config.hpp>
#include <metashell/iface/environment.hpp>
#include <metashell/iface/type_shell.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  class header_file_environment : public iface::environment
  {
  public:
    header_file_environment(iface::type_shell* type_shell,
                            const data::shell_config& config_,
                            const boost::filesystem::path& internal_dir_,
                            const boost::filesystem::path& env_filename_);

    virtual void append(const data::cpp_code& s_) override;
    virtual data::cpp_code get() const override;
    virtual data::cpp_code
    get_appended(const data::cpp_code& s_) const override;

    virtual const data::headers& get_headers() const override;

    virtual data::cpp_code get_all() const override;

  private:
    boost::filesystem::path _internal_dir;
    boost::filesystem::path _env_filename;
    data::cpp_code _buffer;
    data::headers _headers;

    bool _use_precompiled_headers;

    iface::type_shell* _type_shell;

    void save();
  };
}

#endif
