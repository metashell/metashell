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

#include <metashell/core/rapid_object_handler.hpp>

namespace metashell
{
  namespace core
  {
    namespace
    {
      bool whitelisted(const std::string& field_)
      {
        return field_ == "type" || field_ == "cmd" || field_ == "code";
      }
    }

    rapid_object_handler::rapid_object_handler(iface::displayer& displayer_)
      : _in_object(false), _displayer(displayer_)
    {
    }

    bool rapid_object_handler::string(const std::string& str_)
    {
      not_empty();
      if (_in_object)
      {
        _fields.insert({_next_key, str_});
        return true;
      }
      else
      {
        fail("Unexpected string: " + str_);
        return false;
      }
    }

    bool rapid_object_handler::StartObject()
    {
      not_empty();
      if (_in_object)
      {
        fail("Unexpected nested object");
        return false;
      }
      else
      {
        _in_object = true;
        return true;
      }
    }

    bool rapid_object_handler::key(const std::string& str_)
    {
      not_empty();
      if (whitelisted(str_))
      {
        _next_key = str_;
        return true;
      }
      else
      {
        fail("Unknown field: " + str_);
        return false;
      }
    }

    bool rapid_object_handler::end_object()
    {
      not_empty();
      return true;
    }

    std::optional<std::string>
    rapid_object_handler::field(const std::string& name_) const
    {
      const auto i = _fields.find(name_);
      if (i == _fields.end())
      {
        return std::nullopt;
      }
      else
      {
        return i->second;
      }
    }

    void rapid_object_handler::fail(const std::string& msg_)
    {
      if (!failed())
      {
        _displayer.show_error(msg_);
        rapid_handler<rapid_object_handler, false>::fail(msg_);
      }
    }
  }
}
