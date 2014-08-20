#ifndef METASHELL_GET_TYPE_OF_VARIABLE_HPP
#define METASHELL_GET_TYPE_OF_VARIABLE_HPP

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

#include "cxcursor.hpp"

#include <clang-c/Index.h>

#include <string>
#include <functional>

namespace metashell
{
  class get_type_of_variable :
    public std::binary_function<cxcursor, cxcursor, CXChildVisitResult>
  {
  public:
    explicit get_type_of_variable(const std::string& name_ = std::string());

    CXChildVisitResult operator()(cxcursor cursor_);

    const std::string& result() const;
  private:
    std::string _name;
    std::string _result;
  };
}

#endif

