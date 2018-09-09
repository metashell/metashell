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

#include <metashell/core/exception.hpp>
#include <metashell/core/metashell.hpp>
#include <metashell/core/type_shell_clang.hpp>

#include <boost/algorithm/string/trim.hpp>

namespace metashell
{
  namespace core
  {
    type_shell_clang::type_shell_clang(
        const boost::filesystem::path& internal_dir_,
        const boost::filesystem::path& env_filename_,
        clang_binary clang_binary_,
        logger* logger_)
      : _clang_binary(clang_binary_),
        _env_path(internal_dir_ / env_filename_),
        _logger(logger_)
    {
    }

    data::result
    type_shell_clang::eval(const iface::environment& env_,
                           const boost::optional<data::cpp_code>& tmp_exp_,
                           bool use_precompiled_headers_)
    {
      return core::eval(
          env_, tmp_exp_,
          use_precompiled_headers_ ?
              boost::optional<boost::filesystem::path>(_env_path) :
              boost::none,
          boost::none, _clang_binary);
    }

    void type_shell_clang::generate_precompiled_header(
        const boost::filesystem::path& fn_)
    {
      using boost::algorithm::trim_copy;

      const std::string filename = fn_.string();

      METASHELL_LOG(_logger, "Generating percompiled header for " + filename);

      std::vector<std::string> args{
          "-iquote", ".", "-w", "-o", filename + ".pch", filename};

      const data::process_output o = _clang_binary.run(args, "");
      const std::string err = o.standard_output + o.standard_error;
      if (!err.empty()
          // clang displays this even when "-w" is used. This can be ignored
          &&
          trim_copy(err) !=
              "warning: precompiled header used __DATE__ or __TIME__.")
      {
        throw exception("Error precompiling header " + filename + ": " + err);
      }
    }
  }
}
