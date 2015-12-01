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

#include <metashell/header_file_environment.hpp>
#include <metashell/data/headers.hpp>
#include <metashell/config.hpp>
#include <metashell/clang_binary.hpp>
#include <metashell/exception.hpp>
#include <metashell/unsaved_file.hpp>
#include <metashell/headers.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/algorithm/string/trim.hpp>

#include <fstream>
#include <vector>

using namespace metashell;

namespace
{
  const char env_fn[] = "metashell_environment.hpp";

  void extend_to_find_headers_in_local_dir(std::vector<std::string>& v_)
  {
    v_.push_back("-iquote");
    v_.push_back(".");
  }

  void precompile(
    const std::string& clang_path_,
    const std::vector<std::string>& clang_args_,
    const std::string& fn_,
    logger* logger_
  )
  {
    using boost::algorithm::trim_copy;

    METASHELL_LOG(logger_, "Generating percompiled header for " + fn_);

    std::vector<std::string> args(clang_args_);
    extend_to_find_headers_in_local_dir(args);
    args.push_back("-w");
    args.push_back("-o");
    args.push_back(fn_ + ".pch");
    args.push_back(fn_);

    const data::process_output
      o = clang_binary(clang_path_, logger_).run(args, "");
    const std::string err = o.standard_output() + o.standard_error();
    if (
      !err.empty()
      // clang displays this even when "-w" is used. This can be ignored
      && trim_copy(err) !=
        "warning: precompiled header used __DATE__ or __TIME__."
    )
    {
      throw exception("Error precompiling header " + fn_ + ": " + err);
    }
  }
}

header_file_environment::header_file_environment(
  const config& config_,
  logger* logger_
) :
  _dir(),
  _buffer(_dir.path(), config_, "-I" + _dir.path(), logger_),
  _clang_args(),
  _empty_headers(_buffer.internal_dir()),
  _use_precompiled_headers(config_.use_precompiled_headers),
  _clang_path(config_.clang_path)
{
  _clang_args = _buffer.clang_arguments();
  if (_use_precompiled_headers)
  {
    _clang_args.push_back("-include");
    _clang_args.push_back(env_filename());
  }

  _clang_args.push_back("-Xclang");
  _clang_args.push_back("-ast-dump");

  _clang_args.push_back("-Wfatal-errors");

  extend_to_find_headers_in_local_dir(_clang_args);

  save();
  generate(_buffer.get_headers());
}

void header_file_environment::append(const std::string& s_)
{
  _buffer.append(s_);
  save();
}

std::string header_file_environment::get() const
{
  return
    _use_precompiled_headers ?
      std::string() : // The -include directive includes the header
      "#include <" + std::string(env_fn) + ">\n";
}

std::string header_file_environment::get_appended(const std::string& s_) const
{
  return get() + s_;
}

std::vector<std::string>& header_file_environment::clang_arguments()
{
  return _clang_args;
}

const std::vector<std::string>&
  header_file_environment::clang_arguments() const
{
  return _clang_args;
}

std::string header_file_environment::env_filename() const
{
  return internal_dir() + "/" + env_fn;
}

void header_file_environment::save()
{
  const std::string fn = env_filename();
  {
    std::ofstream f(fn.c_str());
    if (f)
    {
      f << _buffer.get();
    }
    else
    {
      throw exception("Error saving environment to " + fn);
    }
  }

  if (_use_precompiled_headers)
  {
    precompile(
      _clang_path,
      _buffer.clang_arguments(),
      fn,
      _buffer.get_logger()
    );
  }
}

std::string header_file_environment::internal_dir() const
{
  return _dir.path();
}

const data::headers& header_file_environment::get_headers() const
{
  return _empty_headers;
}

std::string header_file_environment::get_all() const
{
  return _buffer.get_all();
}

