#ifndef METASHELL_CXTYPE_HPP
#define METASHELL_CXTYPE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang-c/Index.h>

#include <string>

namespace metashell
{
  class cxcursor;

  class cxtype
  {
  public:
    cxtype();
    explicit cxtype(CXType type_);

    std::string spelling() const;

    cxtype canonical_type() const;
  private:
    CXType _type;
  };
}

#endif

