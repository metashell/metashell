#ifndef METASHELL_RAPID_OBJECT_HANDLER_HPP
#define METASHELL_RAPID_OBJECT_HANDLER_HPP

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

#include <metashell/iface/displayer.hpp>

#include <metashell/core/rapid_handler.hpp>

#include <map>
#include <optional>

namespace metashell
{
  namespace core
  {
    class rapid_object_handler
        : public rapid_handler<rapid_object_handler, false>
    {
    public:
      explicit rapid_object_handler(iface::displayer& displayer_);

      bool StartObject();

      std::optional<std::string> field(const std::string& name_) const;

      void fail(const std::string& msg_);

      bool string(const std::string& str_);
      bool key(const std::string& str_);
      bool end_object();

    private:
      std::map<std::string, std::string> _fields;

      bool _in_object;
      std::string _next_key;

      iface::displayer& _displayer;
    };
  }
}

#endif
