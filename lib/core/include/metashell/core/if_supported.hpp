#ifndef METASHELL_IF_SUPPORTED_HPP
#define METASHELL_IF_SUPPORTED_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/not_supported.hpp>
#include <metashell/core/supported.hpp>

#include <metashell/data/feature_not_supported.hpp>

#include <string>
#include <type_traits>

namespace metashell
{
  namespace core
  {
    template <class Expected, class Real>
    typename std::enable_if<supported<Real>::value, Expected&>::type
    if_supported(Real& real_, const std::string&)
    {
      return real_;
    }

    template <class Expected, class Real>
    const Expected& if_supported(const Real& real_, const std::string&)
    {
      return real_;
    }

    template <class Expected>
    Expected& if_supported(const not_supported&,
                           const std::string& engine_name_)
    {
      throw data::feature_not_supported<Expected>(engine_name_);
    }
  }
}

#endif
