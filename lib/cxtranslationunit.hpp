#ifndef METASHELL_CXTRANSLATIONUNIT_HPP
#define METASHELL_CXTRANSLATIONUNIT_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxcursor.hpp"
#include "indexing_iterator.hpp"

#include <clang-c/Index.h>

#include <boost/utility.hpp>
#include <boost/function.hpp>

#include <string>

namespace metashell
{
  class cxtranslationunit : boost::noncopyable
  {
  public:
    typedef boost::function<void(cxcursor, cxcursor)> visitor;

    typedef indexing_iterator<std::string> error_iterator;

    // takes ownership
    explicit cxtranslationunit(CXTranslationUnit tu_);
    ~cxtranslationunit();

    void visit_nodes(const visitor& f_);

    error_iterator errors_begin() const;
    error_iterator errors_end() const;

    bool has_errors() const;
  private:
    CXTranslationUnit _tu;
  };
}

#endif

