#ifndef METASHELL_SYSTEM_TEST_QUERY_JSON_HPP
#define METASHELL_SYSTEM_TEST_QUERY_JSON_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <rapidjson/document.h>

#include <set>
#include <string>

namespace metashell_system_test
{
  namespace impl
  {
    template <class Encoding, class Allocator, bool ExpectFullMatch>
    bool member_compare(const std::set<std::string>& members_,
                        const rapidjson::GenericValue<Encoding, Allocator>& v_)
    {
      if (v_.IsObject())
      {
        decltype(members_.size()) n = 0;
        for (auto i = v_.MemberBegin(), e = v_.MemberEnd(); i != e; ++i)
        {
          const auto& name = i->name;
          if (!name.IsString() ||
              members_.find(std::string(
                  name.GetString(), name.GetStringLength())) == members_.end())
          {
            return false;
          }
          if (ExpectFullMatch)
          {
            ++n;
          }
        }
        return !ExpectFullMatch || n == members_.size();
      }
      else
      {
        return false;
      }
    }
  }

  template <class Encoding, class Allocator>
  bool
  no_other_members_than(const std::set<std::string>& members_,
                        const rapidjson::GenericValue<Encoding, Allocator>& v_)
  {
    return impl::member_compare<Encoding, Allocator, false>(members_, v_);
  }

  // members_are(m, v) == no_other_members_than(m, v) && has_members(m, v)
  template <class Encoding, class Allocator, bool ExpectFullMatch = true>
  bool members_are(const std::set<std::string>& members_,
                   const rapidjson::GenericValue<Encoding, Allocator>& v_)
  {
    return impl::member_compare<Encoding, Allocator, true>(members_, v_);
  }

  template <class Encoding, class Allocator>
  bool has_members(std::set<std::string> members_,
                   const rapidjson::GenericValue<Encoding, Allocator>& v_)
  {
    if (v_.IsObject())
    {
      for (auto i = v_.MemberBegin(), e = v_.MemberEnd(); i != e; ++i)
      {
        const auto& name = i->name;
        if (name.IsString())
        {
          const auto m = members_.find(
              std::string(name.GetString(), name.GetStringLength()));
          if (m != members_.end())
          {
            members_.erase(m);
            if (members_.empty())
            {
              return true;
            }
          }
        }
      }
      return members_.empty();
    }
    return false;
  }

  template <class Encoding, class Allocator>
  bool is_string(const std::string& s_,
                 const rapidjson::GenericValue<Encoding, Allocator>& v_)
  {
    return v_.IsString() &&
           std::string(v_.GetString(), v_.GetStringLength()) == s_;
  }
}

#endif
