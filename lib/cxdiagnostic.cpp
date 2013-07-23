// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxdiagnostic.hpp"
#include "cxstring.hpp"

#include <clang-c/Index.h>

using namespace metashell;

cxdiagnostic::cxdiagnostic(CXDiagnostic d_) : _d(d_) {}

cxdiagnostic::~cxdiagnostic()
{
  clang_disposeDiagnostic(_d);
}

std::string cxdiagnostic::spelling() const
{
  return cxstring(clang_getDiagnosticSpelling(_d));
}

