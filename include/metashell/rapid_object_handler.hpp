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

#include <boost/optional.hpp>

#include <cassert>
#include <map>
#include <string>

namespace metashell
{
  class rapid_object_handler
  {
  public:
    explicit rapid_object_handler(iface::displayer& displayer_);

    bool Null();
    bool Bool(bool);
    bool Int(int);
    bool Uint(unsigned);
    bool Int64(int64_t);
    bool Uint64(uint64_t);
    bool Double(double);

    template <class SizeType>
    bool String(const char* str_, SizeType length_, bool copy_)
    {
      assert(copy_);
      return string(std::string(str_, length_));
    }

    bool StartObject();

    template <class SizeType>
    bool Key(const char* str_, SizeType length_, bool copy_)
    {
      assert(copy_);
      return key(std::string(str_, length_));
    }

    template <class SizeType>
    bool EndObject(SizeType)
    {
      return end_object();
    }

    bool StartArray();

    template <class SizeType>
    bool EndArray(SizeType)
    {
      return end_array();
    }

    bool failed() const;

    boost::optional<std::string> field(const std::string& name_) const;

    bool empty() const;

  private:
    bool _empty;
    bool _failed;
    std::map<std::string, std::string> _fields;

    bool _in_object;
    std::string _next_key;

    iface::displayer& _displayer;

    void fail(const std::string& msg_);

    bool string(const std::string& str_);
    bool key(const std::string& str_);
    bool end_object();
    bool end_array();
  };
}

#endif
