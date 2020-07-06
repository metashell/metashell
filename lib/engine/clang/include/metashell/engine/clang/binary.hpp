#ifndef METASHELL_ENGINE_CLANG_BINARY_HPP
#define METASHELL_ENGINE_CLANG_BINARY_HPP

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

#include <metashell/data/command_line_argument.hpp>
#include <metashell/data/cpp_code.hpp>
#include <metashell/data/engine_arguments.hpp>
#include <metashell/data/engine_name.hpp>
#include <metashell/data/executable_path.hpp>
#include <metashell/data/metaprogram_mode.hpp>
#include <metashell/data/result.hpp>

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/environment.hpp>
#include <metashell/iface/environment_detector.hpp>
#include <metashell/iface/executable.hpp>

#include <metashell/core/logger.hpp>

#include <boost/filesystem/path.hpp>

#include <optional>
#include <string>
#include <tuple>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      class binary : public iface::executable
      {
      public:
        binary(data::executable_path clang_path_,
               data::command_line_argument_list base_args_,
               boost::filesystem::path cwd_,
               core::logger* logger_);

        binary(bool use_internal_templight_,
               data::executable_path clang_path_,
               const data::command_line_argument_list& extra_clang_args_,
               boost::filesystem::path cwd_,
               const boost::filesystem::path& internal_dir_,
               iface::environment_detector& env_detector_,
               core::logger* logger_);

        virtual data::process_output
        run(const data::command_line_argument_list&,
            const std::string& stdin_) const override;

        data::result precompile(data::command_line_argument_list args_,
                                const data::cpp_code& exp_) const;

      private:
        data::executable_path _clang_path;
        data::command_line_argument_list _base_args;
        boost::filesystem::path _cwd;
        core::logger* _logger;
      };

      std::optional<data::executable_path>
      find_clang_nothrow(bool use_internal_templight_,
                         const data::engine_arguments& engine_,
                         const data::executable_path& metashell_binary_,
                         iface::environment_detector& env_detector_,
                         iface::displayer& displayer_,
                         core::logger* logger_);

      data::executable_path
      find_clang(bool use_internal_templight_,
                 const data::engine_arguments& engine_,
                 const data::executable_path& metashell_binary_,
                 iface::environment_detector& env_detector_,
                 iface::displayer& displayer_,
                 core::logger* logger_);

      data::process_output
      run_clang(const iface::executable& clang_binary_,
                data::command_line_argument_list clang_args_,
                const data::cpp_code& input_);

      data::result
      eval(const iface::environment& env_,
           const std::optional<data::cpp_code>& tmp_exp_,
           const std::optional<boost::filesystem::path>& env_path_,
           const std::optional<boost::filesystem::path>& templight_dump_path_,
           binary& clang_binary_);

      std::tuple<data::result, std::string> eval_with_templight_dump_on_stdout(
          const iface::environment& env_,
          const std::optional<data::cpp_code>& tmp_exp_,
          const std::optional<boost::filesystem::path>& env_path_,
          binary& clang_binary_,
          data::metaprogram_mode mode_);
    }
  }
}

#endif
