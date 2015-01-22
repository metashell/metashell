#ifndef METASHELL_CXTYPE_HPP
#define METASHELL_CXTYPE_HPP

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

#include <metashell/iface/cxtype.hpp>

#include <clang-c/Index.h>

#include <string>

namespace metashell
{
  namespace clang
  {
    class cxtype : public iface::cxtype
    {
    public:
      cxtype();
      explicit cxtype(CXType type_);

      virtual std::string spelling() const override;

      virtual std::unique_ptr<iface::cxtype> canonical_type() const override;
    private:
      CXType _type;
    };
  }
}

#endif

