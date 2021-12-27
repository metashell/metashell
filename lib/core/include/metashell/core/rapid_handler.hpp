#ifndef METASHELL_RAPID_HANDLER_HPP
#define METASHELL_RAPID_HANDLER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/json_parsing_error.hpp>

#include <cassert>
#include <cstdint>
#include <string>

namespace metashell
{
  namespace core
  {
    // Using CRTP
    template <class Impl, bool ThrowOnFailure>
    class rapid_handler
    {
    public:
      virtual ~rapid_handler() {}

      bool Null()
      {
        impl().not_empty();
        impl().fail("Unexpected null element");
        return false;
      }

      bool Bool(bool b_)
      {
        impl().not_empty();
        impl().fail("Unexpected bool element: " +
                    std::string(b_ ? "true" : "false"));
        return false;
      }

      template <class SizeType>
      bool RawNumber(const char* str_, SizeType length_, bool)
      {
        impl().not_empty();
        impl().fail("Unexpected number: " + std::string{str_, length_});
        return false;
      }

      bool Int(int i_)
      {
        impl().not_empty();
        impl().fail(unexpected_integer_element(i_));
        return false;
      }

      bool Uint(unsigned i_)
      {
        impl().not_empty();
        impl().fail(unexpected_integer_element(i_));
        return false;
      }

      bool Int64(std::int64_t i_)
      {
        impl().not_empty();
        impl().fail(unexpected_integer_element(i_));
        return false;
      }

      bool Uint64(std::uint64_t i_)
      {
        impl().not_empty();
        impl().fail(unexpected_integer_element(i_));
        return false;
      }

      bool Double(double d_)
      {
        impl().not_empty();
        std::string d = std::to_string(d_);
        const auto last_non_zero = d.find_last_not_of('0');
        if (last_non_zero != std::string::npos)
        {
          d.erase(d[last_non_zero] == '.' ? last_non_zero : last_non_zero + 1);
        }
        impl().fail("Unexpected double element: " + d);
        return false;
      }

      template <class SizeType>
      bool String(const char* str_, SizeType length_, bool copy_)
      {
        assert(copy_);
        return impl().string(std::string(str_, length_));
      }

      bool string(const std::string& str_)
      {
        impl().not_empty();
        impl().fail("Unexpected string element: \"" + str_ + "\"");
        return false;
      }

      bool StartObject()
      {
        impl().not_empty();
        impl().fail("Unexpected object.");
        return false;
      }

      template <class SizeType>
      bool Key(const char* str_, SizeType length_, bool copy_)
      {
        assert(copy_);
        return impl().key(std::string(str_, length_));
      }

      bool key(const std::string& str_)
      {
        impl().not_empty();
        impl().fail("Unexpected key: " + str_);
        return false;
      }

      template <class SizeType>
      bool EndObject(SizeType)
      {
        return impl().end_object();
      }

      bool end_object()
      {
        impl().not_empty();
        impl().fail("Unexpected object.");
        return false;
      }

      bool StartArray()
      {
        impl().not_empty();
        impl().fail("Unexpected array");
        return false;
      }

      template <class SizeType>
      bool EndArray(SizeType)
      {
        return impl().end_array();
      }

      bool end_array()
      {
        impl().not_empty();
        impl().fail("Unexpected array");
        return false;
      }

      bool failed() const { return _failed; }

      bool empty() const { return _empty; }

      void fail(const std::string& msg_)
      {
        _failed = true;
        if (ThrowOnFailure)
        {
          throw json_parsing_error(msg_);
        }
      }

      void not_empty() { _empty = false; }

    private:
      bool _empty = true;
      bool _failed = false;

      Impl& impl() { return *dynamic_cast<Impl*>(this); }

      template <class T>
      static std::string unexpected_integer_element(T value_)
      {
        return "Unexpected integer element: " + std::to_string(value_);
      }
    };
  } // namespace core
} // namespace metashell

#endif
