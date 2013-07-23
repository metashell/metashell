// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxstring.hpp"

using namespace metashell;

cxstring::cxstring(CXString s_) : _s(s_) {}

cxstring::~cxstring()
{
  clang_disposeString(_s);
}

cxstring::operator std::string() const
{
  return clang_getCString(_s);
}

