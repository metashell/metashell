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

#include <metashell/engine/clang/cpp_validator.hpp>

#include <boost/algorithm/string/trim.hpp>

#include <stdexcept>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      cpp_validator::cpp_validator(const boost::filesystem::path& internal_dir_,
                                   const boost::filesystem::path& env_filename_,
                                   binary binary_,
                                   core::logger* logger_)
        : _binary(binary_),
          _env_path(internal_dir_ / env_filename_),
          _logger(logger_)
      {
      }

      data::result cpp_validator::validate_code(const data::cpp_code& src_,
                                                const data::config& config_,
                                                const iface::environment& env_,
                                                bool use_precompiled_headers_)
      {
        METASHELL_LOG(_logger, "Validating code " + src_.value());

        try
        {
          const data::cpp_code src = env_.get_appended(src_);
          data::command_line_argument_list clang_args{"-fsyntax-only"};
          if (use_precompiled_headers_)
          {
            clang_args.push_back("-include", _env_path);
          }

          const data::process_output output =
              run_clang(_binary, std::move(clang_args), src);

          const bool accept =
              exit_success(output) && output.standard_error.empty();

          return data::result{accept, "", output.standard_error,
                              accept && config_.verbose ? src.value() : ""};
        }
        catch (const std::exception& e)
        {
          return data::result(false, "", e.what(), "");
        }
      }
    } // namespace clang
  } // namespace engine
} // namespace metashell
