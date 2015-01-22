#ifndef METASHELL_IFACE_CXCURSOR_HPP
#define METASHELL_IFACE_CXCURSOR_HPP

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

#include <metashell/iface/cxtype.hpp>

#include <string>
#include <memory>

namespace metashell
{
  namespace iface
  {
    class cxcursor
    {
    public:
      virtual ~cxcursor() {}

      virtual std::string spelling() const = 0;

      virtual std::unique_ptr<cxtype> type() const = 0;

      virtual bool variable_declaration() const = 0;
    };
  }
}

#endif

