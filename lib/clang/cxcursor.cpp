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

#include <metashell/clang/cxcursor.hpp>
#include <metashell/clang/cxstring.hpp>

using namespace metashell::clang;

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
  return clang_equalCursors(_cursor, c_._cursor) != 0;
}


