#ifndef METASHELL_STRING_SET_HPP
#define METASHELL_STRING_SET_HPP

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

#include <metashell/shell.hpp>

#include <boost/operators.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include <set>
#include <iosfwd>

#ifdef METASHELL_STRING_SET_CONST_LIMIT
#error METASHELL_STRING_SET_CONST_LIMIT already defined
#endif
#define METASHELL_STRING_SET_CONST_LIMIT 10

class string_set : public boost::equality_comparable<string_set>
{
private:
  typedef std::set<std::string> container;

public:
  string_set(const metashell::shell& shell_, const std::string& s_);

#ifdef METASHELL_STRING_SET_CONST
#error METASHELL_STRING_SET_CONST already defined
#endif
#define METASHELL_STRING_SET_CONST(z, n, unused)                               \
  string_set(BOOST_PP_ENUM_PARAMS(n, const std::string& s));

  BOOST_PP_REPEAT(METASHELL_STRING_SET_CONST_LIMIT,
                  METASHELL_STRING_SET_CONST,
                  ~)

#undef METASHELL_STRING_SET_CONST

  typedef container::const_iterator iterator;

  iterator begin() const;
  iterator end() const;

  bool operator==(const string_set& a_) const;

  friend string_set operator+(const string_set& a_, const string_set& b_);

private:
  container _data;
};

std::ostream& operator<<(std::ostream& o_, const string_set& a_);

#endif
