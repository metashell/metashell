#ifndef METASHELL_CXSTRING_HPP
#define METASHELL_CXSTRING_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang-c/Index.h>

#include <boost/utility.hpp>

#include <string>

namespace metashell
{
  class cxstring : boost::noncopyable
  {
  public:
    // Takes ownership
    explicit cxstring(CXString s_);
    ~cxstring();

    operator std::string() const;
  private:
    CXString _s;
  };
}

#endif

