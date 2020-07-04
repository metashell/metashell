#ifndef METASHELL_ENGINE_CLANG_CODE_COMPLETER_HPP
#define METASHELL_ENGINE_CLANG_CODE_COMPLETER_HPP

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

#include <metashell/iface/code_completer.hpp>

#include <metashell/engine/clang/binary.hpp>

#include <metashell/core/logger.hpp>

#include <boost/filesystem/path.hpp>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      class code_completer : public iface::code_completer
      {
      public:
        code_completer(const boost::filesystem::path& internal_dir_,
                       const boost::filesystem::path& temp_dir_,
                       const boost::filesystem::path& env_filename_,
                       binary binary_,
                       core::logger* logger_);

        virtual data::code_completion
        code_complete(const iface::environment& env_,
                      const data::user_input& src_,
                      bool use_precompiled_headers_) override;

      private:
        binary _binary;
        boost::filesystem::path _temp_dir;
        boost::filesystem::path _env_path;
        core::logger* _logger;
      };
    }
  }
}

#endif
