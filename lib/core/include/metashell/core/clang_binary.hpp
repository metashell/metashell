#ifndef METASHELL_CLANG_BINARY_HPP
#define METASHELL_CLANG_BINARY_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/result.hpp>

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/environment.hpp>
#include <metashell/iface/environment_detector.hpp>
#include <metashell/iface/executable.hpp>

#include <metashell/core/logger.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>
#include <tuple>
#include <vector>

namespace metashell
{
  namespace core
  {
    class clang_binary : public iface::executable
    {
    public:
      clang_binary(boost::filesystem::path clang_path_,
                   std::vector<std::string> base_args_,
                   logger* logger_);

      clang_binary(bool use_internal_templight_,
                   boost::filesystem::path clang_path_,
                   const std::vector<std::string>& extra_clang_args_,
                   const boost::filesystem::path& internal_dir_,
                   iface::environment_detector& env_detector_,
                   logger* logger_);

      virtual data::process_output
      run(const std::vector<std::string>& args_,
          const std::string& stdin_) const override;

      data::result precompile(std::vector<std::string> args_,
                              const data::cpp_code& exp_) const;

    private:
      boost::filesystem::path _clang_path;
      std::vector<std::string> _base_args;
      logger* _logger;
    };

    boost::filesystem::path
    find_clang(bool use_internal_templight_,
               const std::vector<std::string>& extra_clang_args_,
               const std::string& metashell_binary_,
               const std::string& engine_,
               iface::environment_detector& env_detector_,
               iface::displayer& displayer_,
               logger* logger_);

    data::process_output run_clang(const iface::executable& clang_binary_,
                                   std::vector<std::string> clang_args_,
                                   const data::cpp_code& input_);

    data::result
    eval(const iface::environment& env_,
         const boost::optional<data::cpp_code>& tmp_exp_,
         const boost::optional<boost::filesystem::path>& env_path_,
         const boost::optional<boost::filesystem::path>& templight_dump_path_,
         clang_binary& clang_binary_);

    std::tuple<data::result, std::string> eval_with_templight_dump_on_stdout(
        const iface::environment& env_,
        const boost::optional<data::cpp_code>& tmp_exp_,
        const boost::optional<boost::filesystem::path>& env_path_,
        clang_binary& clang_binary_);
  }
}

#endif
