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

#include <metashell/core/feature_validator.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace metashell
{
  namespace core
  {
    feature_validator::feature_validator(
        data::real_engine_name engine_,
        std::vector<data::feature> supported_features_)
      : _engine(std::move(engine_)),
        _supported_features(move(supported_features_)),
        _all_checked(false)
    {
    }

    void feature_validator::check(data::feature feature_,
                                  bool really_supported_)
    {
      assert(!_all_checked);

      const auto old_size = _supported_features.size();

      _supported_features.erase(
          std::remove(
              _supported_features.begin(), _supported_features.end(), feature_),
          _supported_features.end());

      const bool doc_says_supported = _supported_features.size() != old_size;

      if (really_supported_)
      {
        if (!doc_says_supported)
        {
          throw std::runtime_error(
              "Documentation of engine " + _engine + " states feature " +
              to_string(feature_) +
              " is not supported, but the engine supports it.");
        }
      }
      else if (doc_says_supported)
      {
        throw std::runtime_error(
            "Documentation of engine " + _engine + " states feature " +
            to_string(feature_) +
            " is supported, but the engine does not support it.");
      }
    }

    void feature_validator::all_checked()
    {
      using boost::adaptors::transformed;

      assert(!_all_checked);

      if (!_supported_features.empty())
      {
        throw std::runtime_error(
            "During the validaton of engine " + _engine +
            ", the following features have not been checked: " +
            boost::algorithm::join(
                _supported_features |
                    transformed([](data::feature f_) { return to_string(f_); }),
                ", "));
      }

      _all_checked = true;
    }
  } // namespace core
} // namespace metashell
