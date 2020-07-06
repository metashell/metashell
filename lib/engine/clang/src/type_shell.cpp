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

#include <metashell/engine/clang/type_shell.hpp>

#include <metashell/data/exception.hpp>

#include <boost/algorithm/string/trim.hpp>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      type_shell::type_shell(const boost::filesystem::path& internal_dir_,
                             const boost::filesystem::path& env_filename_,
                             binary binary_,
                             core::logger* logger_)
        : _binary(binary_),
          _env_path(internal_dir_ / env_filename_),
          _logger(logger_)
      {
      }

      data::result
      type_shell::eval(const iface::environment& env_,
                       const std::optional<data::cpp_code>& tmp_exp_,
                       bool use_precompiled_headers_)
      {
        return clang::eval(
            env_, tmp_exp_,
            use_precompiled_headers_ ?
                std::optional<boost::filesystem::path>(_env_path) :
                std::nullopt,
            std::nullopt, _binary);
      }

      void type_shell::generate_precompiled_header(
          const boost::filesystem::path& fn_)
      {
        using boost::algorithm::trim_copy;

        const std::string filename = fn_.string();

        METASHELL_LOG(_logger, "Generating percompiled header for " + filename);

        const data::process_output o = _binary.run(
            {"-iquote", ".", "-w", "-o", filename + ".pch", filename}, "");
        const std::string err = o.standard_output + o.standard_error;
        if (!err.empty()
            // clang displays this even when "-w" is used. This can be ignored
            &&
            trim_copy(err) !=
                "warning: precompiled header used __DATE__ or __TIME__.")
        {
          throw data::exception("Error precompiling header " + filename + ": " +
                                err);
        }
      }
    }
  }
}
