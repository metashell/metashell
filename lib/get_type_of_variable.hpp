#ifndef METASHELL_GET_TYPE_OF_VARIABLE_HPP
#define METASHELL_GET_TYPE_OF_VARIABLE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxcursor.hpp"

#include <clang-c/Index.h>

#include <string>

namespace metashell
{
  class get_type_of_variable
  {
  public:
    explicit get_type_of_variable(const std::string& name_ = std::string());

    CXChildVisitResult operator()(cxcursor cursor_, cxcursor parent_);

    const std::string& result() const;
  private:
    std::string _name;
    std::string _result;
  };
}

#endif

