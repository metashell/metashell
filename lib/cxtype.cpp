// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxtype.hpp"
#include "cxcursor.hpp"
#include "cxstring.hpp"

#include <sstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/bind.hpp>
#include <boost/iterator/transform_iterator.hpp>

using namespace metashell;

cxtype::cxtype() {}

cxtype::cxtype(CXType type_) : _type(type_) {}

std::string cxtype::spelling() const
{
  return cxcursor(clang_getTypeDeclaration(_type)).spelling();
}

cxtype cxtype::canonical_type() const
{
  return cxtype(clang_getCanonicalType(_type));
}

