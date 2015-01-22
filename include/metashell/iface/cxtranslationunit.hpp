#ifndef METASHELL_IFACE_CXTRANSLATIONUNIT_HPP
#define METASHELL_IFACE_CXTRANSLATIONUNIT_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/unsaved_file.hpp>
#include <metashell/data/text_position.hpp>

#include <metashell/iface/cxcursor.hpp>

#include <metashell/clang/indexing_iterator.hpp>

#include <functional>
#include <string>
#include <set>

namespace metashell
{
  namespace iface
  {
    class cxtranslationunit
    {
    public:
      virtual ~cxtranslationunit() {}

      typedef clang::indexing_iterator<std::string> error_iterator;
      typedef std::function<void(const cxcursor&)> visitor;

      virtual void visit_nodes(const visitor& f_) = 0;

      virtual error_iterator errors_begin() const = 0;
      virtual error_iterator errors_end() const = 0;

      virtual void code_complete(std::set<std::string>& out_) const = 0;
    };
  }
}

#endif

