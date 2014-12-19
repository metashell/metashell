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
#include <metashell/command.hpp>
#include <metashell/command_processor_queue.hpp>

#include <map>
#include <vector>
#include <string>
#include <cassert>

namespace metashell
{
  class shell;

  class pragma_handler_map
  {
  public:
    template <class Handler>
      // requires: Handler implements iface::pragma_handler
    pragma_handler_map& add(const std::string& name_, Handler handler_)
    {
      add(std::vector<std::string>(1, name_), handler_);
      return *this;
    }

    template <class Handler>
      // requires: Handler implements iface::pragma_handler
    pragma_handler_map& add(
      const std::string& name1_,
      const std::string& name2_,
      Handler handler_
    )
    {
      std::vector<std::string> params;
      params.reserve(2);

      params.push_back(name1_);
      params.push_back(name2_);

      add(params, handler_);
      return *this;
    }

    template <class Handler>
      // requires: Handler implements iface::pragma_handler
    pragma_handler_map& add(
      const std::string& name1_,
      const std::string& name2_,
      const std::string& name3_,
      Handler handler_
    )
    {
      std::vector<std::string> params;
      params.reserve(3);

      params.push_back(name1_);
      params.push_back(name2_);
      params.push_back(name3_);

      add(params, handler_);
      return *this;
    }

    template <class Handler>
      // requires: Handler implements iface::pragma_handler
    pragma_handler_map& add(
      const std::vector<std::string>& names_,
      Handler handler_
    )
    {
      assert(!names_.empty());

      _handlers.insert(std::make_pair(names_, pragma_handler(handler_)));
      return *this;
    }


    void process(
      const command::iterator& p_,
      const command::iterator& end_,
      iface::displayer& displayer_
    ) const;

    typedef
      std::map<std::vector<std::string>, pragma_handler>::const_iterator
      iterator;
    typedef iterator const_iterator;

    iterator begin() const;
    iterator end() const;
    iterator find(const std::vector<std::string>& p_) const;

    static pragma_handler_map
      build_default(shell& shell_, command_processor_queue* cpq_);
  private:
    std::map<std::vector<std::string>, pragma_handler> _handlers;
  };
}

#endif

