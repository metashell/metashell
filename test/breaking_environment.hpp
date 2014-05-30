#ifndef METASHELL_TEST_BREAKING_ENVIRONMENT_HPP
#define METASHELL_TEST_BREAKING_ENVIRONMENT_HPP

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

class breaking_environment : public metashell::in_memory_environment
{
public:
  explicit breaking_environment(const metashell::config& cfg_);

  virtual void append(const std::string& s_);
  virtual std::string get_appended(const std::string& s_) const;

  void append_throw_from_now();
  void get_appended_throw_from_now();
private:
  bool _append_throw;
  bool _get_appended_throw;

  bool _in_append;
};

#endif


