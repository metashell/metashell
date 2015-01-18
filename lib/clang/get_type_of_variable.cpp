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

#include <metashell/clang/get_type_of_variable.hpp>

#include <metashell/exception.hpp>

#include <boost/algorithm/string/trim.hpp>

using namespace metashell::clang;

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
      throw
        metashell::exception(
          "\"" + prefix_ + "\" is not a prefix of \"" + s_ + "\""
        );
    }
    else if (
      s_.size() < suffix_.size()
      || std::string(s_.end() - suffix_.size(), s_.end()) != suffix_
    )
    {
      throw
        metashell::exception(
          "\"" + suffix_ + "\" is not a suffix of \"" + s_ + "\""
        );
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

CXChildVisitResult get_type_of_variable::operator()(cxcursor cursor_)
{
  if (cursor_.kind() == CXCursor_VarDecl && cursor_.spelling() == _name)
  {
    try
    {
      _result =
        unwrap("wrap<", cursor_.type().canonical_type().spelling(), ">");
    }
    catch (const exception&)
    {
      return CXChildVisit_Continue;
    }
    boost::algorithm::trim(_result);
    // continue to get the last one
  }
  return CXChildVisit_Continue;
}

const std::string& get_type_of_variable::result() const
{
  return _result;
}

