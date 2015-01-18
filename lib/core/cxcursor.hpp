#ifndef METASHELL_CXCURSOR_HPP
#define METASHELL_CXCURSOR_HPP

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


