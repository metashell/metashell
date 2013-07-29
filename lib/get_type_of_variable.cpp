// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "get_type_of_variable.hpp"
#include "exception.hpp"

#include <boost/algorithm/string/trim.hpp>

using namespace metashell;

namespace
{
  std::string unwrap(
    const std::string& prefix_,
    const std::string& s_,
    const std::string& suffix_
  )
  {
    if (
      s_.size() < prefix_.size()
      || std::string(s_.begin(), s_.begin() + prefix_.size()) != prefix_
    )
    {
      throw exception("\"" + prefix_ + "\" is not a prefix of \"" + s_ + "\"");
    }
    else if (
      s_.size() < suffix_.size()
      || std::string(s_.end() - suffix_.size(), s_.end()) != suffix_
    )
    {
      throw exception("\"" + suffix_ + "\" is not a suffix of \"" + s_ + "\"");
    }
    else
    {
      return
        std::string(s_.begin() + prefix_.size(), s_.end() - suffix_.size());
    }
  }
}

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
    _result = unwrap("wrap<", cursor_.type().canonical_type().spelling(), ">");
    boost::algorithm::trim(_result);
    // continue to get the last one
  }
  return CXChildVisit_Continue;
}

const std::string& get_type_of_variable::result() const
{
  return _result;
}

