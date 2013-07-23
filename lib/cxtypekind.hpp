#ifndef METASHELL_CXTYPEKIND_HPP
#define METASHELL_CXTYPEKIND_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang-c/Index.h>

#include <boost/operators.hpp>

#include <string>

namespace metashell
{
  class cxtypekind : public boost::equality_comparable<cxtypekind>
  {
  public:
    cxtypekind(CXTypeKind kind_);

    std::string spelling() const;

    bool operator==(const cxtypekind& k_) const;

    CXTypeKind value() const;
  private:
    CXTypeKind _kind;
  };
}

#endif


