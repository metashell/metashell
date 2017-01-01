#ifndef METASHELL_CPP_VALIDATOR_VC_HPP
#define METASHELL_CPP_VALIDATOR_VC_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/cpp_validator.hpp>

#include <metashell/logger.hpp>
#include <metashell/vc_binary.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  class cpp_validator_vc : public iface::cpp_validator
  {
  public:
    cpp_validator_vc(const boost::filesystem::path& internal_dir_,
                     const boost::filesystem::path& env_filename_,
                     vc_binary vc_binary_,
                     logger* logger_);

    virtual data::result validate_code(const std::string& src_,
                                       const data::config& config_,
                                       const iface::environment& env_,
                                       bool use_precompiled_headers_) override;

  private:
    vc_binary _vc_binary;
    boost::filesystem::path _env_path;
    logger* _logger;
  };
}

#endif
