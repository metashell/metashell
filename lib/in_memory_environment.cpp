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

#include <metashell/in_memory_environment.hpp>
#include <metashell/standard.hpp>
#include <metashell/config.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <algorithm>
#include <iterator>
#include <cassert>

using namespace metashell;

namespace
{
  const std::vector<std::string> no_extra_arguments;

  template <class Cont>
  void add_with_prefix(
    const std::string& prefix_,
    const Cont& cont_,
    std::vector<std::string>& v_
  )
  {
    using boost::phoenix::arg_names::arg1;

    std::transform(
      cont_.begin(),
      cont_.end(),
      std::back_insert_iterator<std::vector<std::string> >(v_),
      prefix_ + arg1
    );
  }

}

in_memory_environment::in_memory_environment(
  const std::string& internal_dir_,
  const config& config_,
  const std::string& clang_extra_arg_
) :
  _buffer(),
  _headers(internal_dir_),
  _clang_args()
{
  assert(!internal_dir_.empty());

  _clang_args.push_back("-x");
  _clang_args.push_back("c++-header");
  _clang_args.push_back(clang_argument(config_.standard_to_use));
  _clang_args.push_back("-I" + internal_dir());

  add_with_prefix("-I", config_.include_path, _clang_args);
  add_with_prefix("-D", config_.macros, _clang_args);

  if (!config_.warnings_enabled)
  {
    _clang_args.push_back("-w");
  }

  _clang_args.insert(
    _clang_args.end(),
    config_.extra_clang_args.begin(),
    config_.extra_clang_args.end()
  );

  if (!clang_extra_arg_.empty())
  {
    _clang_args.push_back(clang_extra_arg_);
  }
}

void in_memory_environment::append(const std::string& s_)
{
  _buffer = get_appended(s_);
}

std::string in_memory_environment::get() const
{
  return _buffer;
}

std::string in_memory_environment::get_appended(const std::string& s_) const
{
  return _buffer.empty() ? s_ : (_buffer + '\n' + s_);
}

std::vector<std::string>& in_memory_environment::clang_arguments()
{
  return _clang_args;
}

const std::vector<std::string>&
  in_memory_environment::clang_arguments() const
{
  return _clang_args;
}

std::string in_memory_environment::internal_dir() const
{
  return _headers.internal_dir();
}

const headers& in_memory_environment::get_headers() const
{
  return _headers;
}

void in_memory_environment::add_clang_arg(const std::string& arg_)
{
  _clang_args.push_back(arg_);
}

std::string in_memory_environment::get_all() const
{
  return _buffer;
}

