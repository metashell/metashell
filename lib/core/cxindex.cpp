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

#include "cxindex.hpp"

#include <clang-c/Index.h>

using namespace metashell;

cxindex::cxindex(logger* logger_) :
  _index(clang_createIndex(0, 0)),
  _logger(logger_)
{}

cxindex::~cxindex()
{
  clang_disposeIndex(_index);
}

std::unique_ptr<cxtranslationunit> cxindex::parse_code(
  const unsaved_file& src_,
  const environment& env_
)
{
  return
    std::unique_ptr<cxtranslationunit>(
      new cxtranslationunit(env_, src_, _index, _logger)
    );
}


