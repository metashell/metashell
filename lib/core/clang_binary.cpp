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

#include <metashell/clang_binary.hpp>
#include <metashell/metashell.hpp>
#include <metashell/process/run.hpp>

#include <boost/algorithm/string/join.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  std::string quote_argument(const std::string& arg_)
  {
#ifdef _WIN32
    return "\"" + arg_ + "\"";
#else
    return arg_;
#endif
  }
}

clang_binary::clang_binary(const boost::filesystem::path& path_,
                           const std::vector<std::string>& base_args_,
                           logger* logger_)
  : _base_args(base_args_.size() + 1), _logger(logger_)
{
  _base_args[0] = quote_argument(path_.string());
  std::transform(base_args_.begin(), base_args_.end(), _base_args.begin() + 1,
                 quote_argument);
}

data::process_output clang_binary::run(const std::vector<std::string>& args_,
                                       const std::string& stdin_) const
{
  std::vector<std::string> cmd(_base_args.size() + args_.size());

  std::transform(args_.begin(), args_.end(),
                 std::copy(_base_args.begin(), _base_args.end(), cmd.begin()),
                 quote_argument);

  METASHELL_LOG(_logger, "Running Clang: " + boost::algorithm::join(cmd, " "));

  const data::process_output o = process::run(cmd, stdin_);

  METASHELL_LOG(_logger, "Clang's exit code: " + to_string(o.exit_code));
  METASHELL_LOG(_logger, "Clang's stdout: " + o.standard_output);
  METASHELL_LOG(_logger, "Clang's stderr: " + o.standard_error);

  return o;
}

data::process_output
metashell::run_clang(const iface::executable& clang_binary_,
                     std::vector<std::string> clang_args_,
                     const std::string& input_)
{
  clang_args_.push_back("-"); // Compile from stdin

  return clang_binary_.run(clang_args_, input_);
}

data::result metashell::eval(
    const iface::environment& env_,
    const boost::optional<std::string>& tmp_exp_,
    const boost::optional<boost::filesystem::path>& env_path_,
    const boost::optional<boost::filesystem::path>& templight_dump_path_,
    clang_binary& clang_binary_)
{
  std::vector<std::string> clang_args{"-Xclang", "-ast-dump"};
  if (env_path_)
  {
    clang_args.push_back("-include");
    clang_args.push_back(env_path_->string());
  }
  if (templight_dump_path_)
  {
    clang_args.push_back("-Xtemplight");
    clang_args.push_back("-profiler");
    clang_args.push_back("-Xtemplight");
    clang_args.push_back("-safe-mode");

    // templight can't be forced to generate output file with
    // -Xtemplight -output=<file> for some reason
    // A workaround is to specify a standard output location with -o
    // then append ".trace.pbf" to the specified file (on the calling side)
    clang_args.push_back("-o");
    clang_args.push_back(templight_dump_path_->string());
  }

  const data::process_output output =
      run_clang(clang_binary_, clang_args,
                tmp_exp_ ?
                    env_.get_appended("::metashell::impl::wrap< " + *tmp_exp_ +
                                      " > __metashell_v;\n") :
                    env_.get());

  const bool success = output.exit_code == data::exit_code_t(0);

  return data::result{success,
                      success && tmp_exp_ ?
                          get_type_from_ast_string(output.standard_output) :
                          "",
                      success ? "" : output.standard_error, ""};
}
