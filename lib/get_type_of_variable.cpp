// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "get_type_of_variable.hpp"

using namespace metashell;

get_type_of_variable::get_type_of_variable(const std::string& name_) :
  _name(name_)
{}

CXChildVisitResult get_type_of_variable::operator()(
  cxcursor cursor_,
  cxcursor parent_
)
{
  if (cursor_.kind() == CXCursor_VarDecl && cursor_.spelling() == _name)
  {
    _result = cursor_.type().pointee_type().canonical_type().spelling();
    // continue to get the last one
  }
  return CXChildVisit_Continue;
}

const std::string& get_type_of_variable::result() const
{
  return _result;
}

