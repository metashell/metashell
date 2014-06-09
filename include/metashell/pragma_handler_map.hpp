#ifndef METASHELL_PRAGMA_HANDLER_MAP_HPP
#define METASHELL_PRAGMA_HANDLER_MAP_HPP

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

#include <metashell/pragma_handler.hpp>

#include <map>
#include <string>

namespace metashell
{
  class metashell_pragma;
  class shell;

  class pragma_handler_map
  {
  public:
    template <class Handler>
      // requires: Handler implements pragma_handler_interface
    pragma_handler_map& add(const std::string& name_, Handler handler_)
    {
      _handlers.insert(std::make_pair(name_, pragma_handler(handler_)));
      return *this;
    }

    void process(const metashell_pragma& p_) const;

    typedef std::map<std::string, pragma_handler>::const_iterator iterator;
    typedef iterator const_iterator;

    iterator begin() const;
    iterator end() const;

    static pragma_handler_map build_default(shell& shell_);
  private:
    std::map<std::string, pragma_handler> _handlers;
  };
}

#endif

