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

#include "breaking_environment.hpp"

#include <stdexcept>
#include <cassert>

using namespace metashell;

namespace
{
  void throw_(bool should_throw_)
  {
    if (should_throw_)
    {
      throw std::runtime_error("Test exception");
    }
  }
}

breaking_environment::breaking_environment(const config& cfg_) :
  in_memory_environment("", cfg_),
  _throwing(false)
{}

void breaking_environment::append(const std::string& s_)
{
  throw_(_throwing);
  in_memory_environment::append(s_);
}

std::string breaking_environment::get() const
{
  throw_(_throwing);
  return in_memory_environment::get();
}

std::string breaking_environment::get_appended(const std::string& s_) const
{
  throw_(_throwing);
  return in_memory_environment::get_appended(s_);
}

std::string breaking_environment::internal_dir() const
{
  throw_(_throwing);
  return in_memory_environment::internal_dir();
}

const std::vector<std::string>& breaking_environment::clang_arguments() const
{
  throw_(_throwing);
  return in_memory_environment::clang_arguments();
}

const headers& breaking_environment::get_headers() const
{
  throw_(_throwing);
  return in_memory_environment::get_headers();
}

void breaking_environment::throw_from_now()
{
  assert(!_throwing);
  _throwing = true;
}


