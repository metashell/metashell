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

#include <metashell/core/cpp_validator_vc.hpp>
#include <metashell/core/metashell.hpp>

#include <stdexcept>

namespace metashell
{
  namespace core
  {
    cpp_validator_vc::cpp_validator_vc(
        const boost::filesystem::path& internal_dir_,
        const boost::filesystem::path& env_filename_,
        vc_binary vc_binary_,
        logger* logger_)
      : _vc_binary(vc_binary_),
        _env_path(internal_dir_ / env_filename_),
        _logger(logger_)
    {
    }

    data::result cpp_validator_vc::validate_code(const data::cpp_code& src_,
                                                 const data::config& config_,
                                                 const iface::environment& env_,
                                                 bool)
    {
      METASHELL_LOG(_logger, "Validating code " + src_.value());

      try
      {
        const data::cpp_code src = env_.get_appended(src_);
        const data::process_output output = run_vc(_vc_binary, {"/c"}, src);
        const std::string error = vc_error_report_on_stdout(output);

        const bool accept =
            output.exit_code == data::exit_code_t(0) && error.empty();

        return data::result{
            accept, "", error, accept && config_.verbose ? src.value() : ""};
      }
      catch (const std::exception& e)
      {
        return data::result(false, "", e.what(), "");
      }
    }
  }
}
