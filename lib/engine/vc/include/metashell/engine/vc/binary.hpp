#ifndef METASHELL_ENGINE_VC_BINARY_HPP
#define METASHELL_ENGINE_VC_BINARY_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/result.hpp>

#include <metashell/iface/executable.hpp>

#include <metashell/core/logger.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  namespace engine
  {
    namespace vc
    {
      class binary : public iface::executable
      {
      public:
        binary(boost::filesystem::path cl_path_,
               std::vector<std::string> base_args_,
               boost::filesystem::path temp_dir_,
               core::logger* logger_);

        virtual data::process_output
        run(const std::vector<std::string>& args_,
            const std::string& stdin_) const override;

        const boost::filesystem::path& temp_dir() const;
        const std::vector<std::string>& base_args() const;

      private:
        boost::filesystem::path _cl_path;
        std::vector<std::string> _base_args;
        boost::filesystem::path _temp_dir;
        core::logger* _logger;
      };

      data::process_output run(const binary& binary_,
                               std::vector<std::string> args_,
                               const data::cpp_code& input_);

      std::string error_report_on_stdout(const data::process_output& output_);

      std::string error_report_on_stderr(const data::process_output& output_);
    }
  }
}

#endif
