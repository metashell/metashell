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

#include <boost/algorithm/string/join.hpp>

#include <just/process.hpp>

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

clang_binary::clang_binary(const std::string& path_,
                           const std::vector<std::string>& base_args_,
                           logger* logger_)
  : _base_args(base_args_.size() + 1), _logger(logger_)
{
  _base_args[0] = quote_argument(path_);
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

  const just::process::output o = just::process::run(cmd, stdin_);

  METASHELL_LOG(_logger, "Clang's exit code: " + std::to_string(o.exit_code()));
  METASHELL_LOG(_logger, "Clang's stdout: " + o.standard_output());
  METASHELL_LOG(_logger, "Clang's stderr: " + o.standard_error());

  return data::process_output(data::exit_code_t(o.exit_code()),
                              o.standard_output(), o.standard_error());
}
