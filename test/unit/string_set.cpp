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

#include "string_set.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/preprocessor/cat.hpp>

#include <iostream>
#include <algorithm>

using namespace metashell;

#ifdef METASHELL_STRING_SET_CONST_ADD
#error METASHELL_STRING_SET_CONST_ADD already defined
#endif
#define METASHELL_STRING_SET_CONST_ADD(z, n, unused)                           \
  _data.insert(BOOST_PP_CAT(s, n));

#ifdef METASHELL_STRING_SET_CONST
#error METASHELL_STRING_SET_CONST already defined
#endif
#define METASHELL_STRING_SET_CONST(z, n, unused)                               \
  string_set::string_set(BOOST_PP_ENUM_PARAMS(n, const std::string& s))        \
  {                                                                            \
    BOOST_PP_REPEAT(n, METASHELL_STRING_SET_CONST_ADD, ~);                     \
  }

BOOST_PP_REPEAT(METASHELL_STRING_SET_CONST_LIMIT, METASHELL_STRING_SET_CONST, ~)

#undef METASHELL_STRING_SET_CONST
#undef METASHELL_STRING_SET_CONST_ADD

string_set::string_set(const shell& shell_, const std::string& s_)
{
  _data.insert("garbage");
  shell_.code_complete(s_, _data);
}

string_set::iterator string_set::begin() const { return _data.begin(); }

string_set::iterator string_set::end() const { return _data.end(); }

bool string_set::operator==(const string_set& a_) const
{
  return _data.size() == a_._data.size() &&
         std::equal(begin(), end(), a_.begin());
}

std::ostream& operator<<(std::ostream& o_, const string_set& a_)
{
  const boost::iterator_range<string_set::iterator> r(a_.begin(), a_.end());
  return o_ << "[" << boost::algorithm::join(r, ", ") << "]";
}

string_set operator+(const string_set& a_, const string_set& b_)
{
  string_set r(a_);
  for (string_set::iterator i = b_.begin(), e = b_.end(); i != e; ++i)
  {
    r._data.insert(*i);
  }

  return r;
}
