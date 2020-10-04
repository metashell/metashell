#ifndef METASHELL_ENGINE_VC_CPP_VALIDATOR_HPP
#define METASHELL_ENGINE_VC_CPP_VALIDATOR_HPP

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

#include <metashell/engine/vc/binary.hpp>

#include <metashell/core/logger.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  namespace engine
  {
    namespace vc
    {
      class cpp_validator : public iface::cpp_validator
      {
      public:
        cpp_validator(const boost::filesystem::path& internal_dir_,
                      const boost::filesystem::path& env_filename_,
                      binary binary_,
                      core::logger* logger_);

        virtual data::result
        validate_code(const data::cpp_code& src_,
                      const data::config& config_,
                      const iface::environment& env_,
                      bool use_precompiled_headers_) override;

      private:
        binary _binary;
        boost::filesystem::path _env_path;
        core::logger* _logger;
      };
    } // namespace vc
  } // namespace engine
} // namespace metashell

#endif
