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

  class bool_override_guard
  {
  public:
    explicit bool_override_guard(bool& b_) : _b(b_)
    {
      _b = true;
    }

    ~bool_override_guard()
    {
      _b = false;
    }
  private:
    bool& _b;
  };
}

breaking_environment::breaking_environment(const data::config& cfg_) :
  in_memory_environment("__breaking_environment", cfg_),
  _append_throw(false),
  _get_appended_throw(false),
  _in_append(false)
{}

void breaking_environment::append(const std::string& s_)
{
  throw_(_append_throw);

  bool_override_guard g(_in_append);
  in_memory_environment::append(s_);
}

std::string breaking_environment::get_appended(const std::string& s_) const
{
  throw_(!_in_append && _get_appended_throw);
  return in_memory_environment::get_appended(s_);
}

void breaking_environment::append_throw_from_now()
{
  assert(!_append_throw);
  _append_throw = true;
}

void breaking_environment::get_appended_throw_from_now()
{
  assert(!_get_appended_throw);
  _get_appended_throw = true;
}

