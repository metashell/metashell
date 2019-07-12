// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/some_feature_not_supported.hpp>

namespace metashell
{
  namespace data
  {
    some_feature_not_supported::some_feature_not_supported(
        const engine_name& engine_, const feature& feature_)
      : exception("Feature " + to_string(feature_) +
                  " is not supported by the " + engine_ + " engine.")
    {
    }
  }
}
