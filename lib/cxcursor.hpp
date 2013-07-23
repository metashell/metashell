#ifndef METASHELL_CXCURSOR_HPP
#define METASHELL_CXCURSOR_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxtype.hpp"

#include <clang-c/Index.h>

#include <boost/operators.hpp>

#include <string>

namespace metashell
{
  class cxcursor : public boost::equality_comparable<cxcursor>
  {
  public:
    class namespace_iterator;

    explicit cxcursor(CXCursor cursor_);

    CXCursorKind kind() const;
    std::string spelling() const;

    cxcursor semantic_parent() const;

    cxtype type() const;

    namespace_iterator namespace_begin() const;
    namespace_iterator namespace_end() const;

    bool operator==(const cxcursor& c_) const;
  private:
    CXCursor _cursor;
  };

  class cxcursor::namespace_iterator :
    public
      boost::forward_iterator_helper<
        namespace_iterator,
        std::string*,
        int,
        std::string&,
        std::string
      >
  {
  public:
    friend class cxcursor;

    typedef std::forward_iterator_tag iterator_category;

    bool operator==(const namespace_iterator& i_) const;

    namespace_iterator& operator++();
    const std::string& operator*() const;
  private:
    cxcursor _tu;
    bool _end;
    mutable std::string _value;
    mutable bool _value_valid;

    namespace_iterator();
    namespace_iterator(cxcursor tu_);
  };
}

#endif


