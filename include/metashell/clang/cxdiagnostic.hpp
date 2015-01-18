#ifndef METASHELL_CXDIAGNOSTIC_HPP
#define METASHELL_CXDIAGNOSTIC_HPP

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

#include <clang-c/Index.h>

#include <boost/utility.hpp>

#include <string>

namespace metashell
{
  namespace clang
  {
    class cxdiagnostic : boost::noncopyable
    {
    public:
      // takes ownership
      explicit cxdiagnostic(CXDiagnostic d_);
      ~cxdiagnostic();

      std::string spelling() const;
    private:
      CXDiagnostic _d;
    };
  }
}

#endif

