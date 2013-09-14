#ifndef METASHELL_STRING_SET_HPP
#define METASHELL_STRING_SET_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
  #define METASHELL_STRING_SET_CONST(z, n, unused) \
    string_set(BOOST_PP_ENUM_PARAMS(n, const std::string& s));

  BOOST_PP_REPEAT(
    METASHELL_STRING_SET_CONST_LIMIT,
    METASHELL_STRING_SET_CONST,
    ~
  )

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

