#ifndef METASHELL_VC_BINARY_HPP
#define METASHELL_VC_BINARY_HPP

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
#include <metashell/logger.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <vector>

namespace metashell
{
  class vc_binary : public iface::executable
  {
  public:
    vc_binary(boost::filesystem::path cl_path_,
              std::vector<std::string> base_args_,
              boost::filesystem::path temp_dir_,
              logger* logger_);

    virtual data::process_output run(const std::vector<std::string>& args_,
                                     const std::string& stdin_) const override;

    const boost::filesystem::path& temp_dir() const;
    const std::vector<std::string>& base_args() const;

  private:
    boost::filesystem::path _cl_path;
    std::vector<std::string> _base_args;
    boost::filesystem::path _temp_dir;
    logger* _logger;
  };

  data::process_output run_vc(const vc_binary& vc_binary_,
                              std::vector<std::string> vc_args_,
                              const data::cpp_code& input_);

  std::string vc_error_report_on_stdout(const data::process_output& vc_output_);
  std::string vc_error_report_on_stderr(const data::process_output& vc_output_);
}

#endif
