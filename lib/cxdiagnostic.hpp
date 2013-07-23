#ifndef METASHELL_CXDIAGNOSTIC_HPP
#define METASHELL_CXDIAGNOSTIC_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang-c/Index.h>

#include <boost/utility.hpp>

#include <string>

namespace metashell
{
  class cxdiagnostic : boost::noncopyable
  {
  public:
    // takes ownership
    explicit cxdiagnostic(CXDiagnostic d_);
    ~cxdiagnostic();

    std::string spelling() const;
  private:
    CXDiagnostic _d;
  };
}

#endif

