#ifndef METASHELL_CXINDEX_HPP
#define METASHELL_CXINDEX_HPP

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

#include "cxtranslationunit.hpp"

#include <metashell/config.hpp>
#include <metashell/environment.hpp>
#include <metashell/unsaved_file.hpp>

#include <clang-c/Index.h>

#include <boost/utility.hpp>

#include <memory>

namespace metashell
{
  class cxindex : boost::noncopyable
  {
  public:
    cxindex();
    ~cxindex();

    std::unique_ptr<cxtranslationunit> parse_code(
      const unsaved_file& src_,
      const config& config_,
      const environment& env_
    );
  private:
    CXIndex _index;
  };
}

#endif

