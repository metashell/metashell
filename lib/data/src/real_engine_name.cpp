// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/exception.hpp>
#include <metashell/data/real_engine_name.hpp>

#include <cassert>
#include <iostream>

namespace metashell
{
  namespace data
  {
    real_engine_name parse_real_engine_name(const std::string_view& s_)
    {
      if (s_ == "internal")
      {
        return real_engine_name::internal;
      }
      else if (s_ == "templight")
      {
        return real_engine_name::templight;
      }
      else if (s_ == "clang")
      {
        return real_engine_name::clang;
      }
      else if (s_ == "null")
      {
        return real_engine_name::null;
      }
      else if (s_ == "gcc")
      {
        return real_engine_name::gcc;
      }
      else if (s_ == "msvc")
      {
        return real_engine_name::msvc;
      }
      else if (s_ == "wave")
      {
        return real_engine_name::wave;
      }
      else if (s_ == "pure_wave")
      {
        return real_engine_name::pure_wave;
      }
      else
      {
        throw exception("Invalid engine name: " + std::string(s_));
      }
    }

    std::string to_string(real_engine_name e_)
    {
      switch (e_)
      {
      case real_engine_name::internal:
        return "internal";
      case real_engine_name::templight:
        return "templight";
      case real_engine_name::clang:
        return "clang";
      case real_engine_name::null:
        return "null";
      case real_engine_name::gcc:
        return "gcc";
      case real_engine_name::msvc:
        return "msvc";
      case real_engine_name::wave:
        return "wave";
      case real_engine_name::pure_wave:
        return "pure_wave";
      }
      assert(!"Invalid engine name");
      return "";
    }

    std::ostream& operator<<(std::ostream& out_, real_engine_name e_)
    {
      return out_ << to_string(e_);
    }

    std::string operator+(const std::string& lhs_, real_engine_name rhs_)
    {
      return lhs_ + to_string(rhs_);
    }

    std::string operator+(real_engine_name lhs_, const std::string& rhs_)
    {
      return to_string(lhs_) + rhs_;
    }
  }
}
