// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxcursor.hpp"
#include "cxstring.hpp"

using namespace metashell;

/*
 * namespace_iterator
 */
cxcursor::namespace_iterator::namespace_iterator() :
  _tu(CXCursor()),
  _end(true),
  _value(),
  _value_valid(false)
{}

cxcursor::namespace_iterator::namespace_iterator(cxcursor tu_) :
  _tu(tu_),
  _end(tu_.kind() != CXCursor_Namespace),
  _value(),
  _value_valid(false)
{
}

bool cxcursor::namespace_iterator::operator==(
  const cxcursor::namespace_iterator& i_
) const
{
  return (_end || i_._end) ? (_end && i_._end) : (_tu == i_._tu);
}

cxcursor::namespace_iterator& cxcursor::namespace_iterator::operator++()
{
  _tu = _tu.semantic_parent();
  _value_valid = false;
  _end |= _tu.kind() != CXCursor_Namespace;
  return *this;
}

const std::string& cxcursor::namespace_iterator::operator*() const
{
  if (!_value_valid)
  {
    _value = _tu.spelling();
    _value_valid = true;
  }
  return _value;
}

/*
 * cxcursor
 */

cxcursor::cxcursor(CXCursor cursor_) : _cursor(cursor_) {}

std::string cxcursor::spelling() const
{
  return cxstring(clang_getCursorDisplayName(_cursor));
}

cxtype cxcursor::type() const
{
  return cxtype(clang_getCursorType(_cursor));
}

CXCursorKind cxcursor::kind() const
{
  return clang_getCursorKind(_cursor);
}

cxcursor cxcursor::semantic_parent() const
{
  return cxcursor(clang_getCursorSemanticParent(_cursor));
}

cxcursor::namespace_iterator cxcursor::namespace_begin() const
{
  return namespace_iterator(semantic_parent());
}

cxcursor::namespace_iterator cxcursor::namespace_end() const
{
  return namespace_iterator();
}

bool cxcursor::operator==(const cxcursor& c_) const
{
  return clang_equalCursors(_cursor, c_._cursor);
}


