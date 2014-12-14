// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include "test_shell.hpp"
#include "argv0.hpp"

using namespace metashell;

namespace
{
  config default_config()
  {
    config cfg = empty_config(argv0::get());
#ifdef WINDOWS_HEADERS
    const std::string windows_headers = WINDOWS_HEADERS;
    cfg.include_path.push_back(windows_headers);
    cfg.include_path.push_back(windows_headers + "\\mingw32");
#endif
    cfg.include_path.push_back(BOOST_INCLUDE_PATH);
#ifdef LIBCXX_INCLUDE_PATH
    cfg.include_path.push_back(LIBCXX_INCLUDE_PATH);
#endif
    return cfg;
  }
}

test_shell::test_shell(iface::displayer& displayer_) :
  shell(default_config(), displayer_),
  _history(0)
{}

test_shell::test_shell(const config& cfg_, iface::displayer& displayer_) :
  shell(cfg_, displayer_),
  _history(0)
{}

test_shell::test_shell(
  std::vector<std::string>& history_,
  iface::displayer& displayer_
) :
  shell(default_config(), displayer_),
  _history(&history_)
{}

test_shell::test_shell(
  const metashell::config& cfg_,
  std::unique_ptr<environment> env_,
  iface::displayer& displayer_
) :
  shell(cfg_, std::move(env_), displayer_),
  _history(0)
{}

void test_shell::add_history(const std::string& s_)
{
  if (_history)
  {
    _history->push_back(s_);
  }
}

