#ifndef METASHELL_CXCODECOMPLETERESULTS_HPP
#define METASHELL_CXCODECOMPLETERESULTS_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang-c/Index.h>

#include <boost/utility.hpp>

#include <set>
#include <string>

namespace metashell
{
  class cxcodecompleteresults : boost::noncopyable
  {
  public:
    cxcodecompleteresults(CXCodeCompleteResults* r_);
    ~cxcodecompleteresults();

    void fill(std::set<std::string>& out_) const;
  private:
    CXCodeCompleteResults* _r;
  };
}

#endif

