#ifndef METASHELL_BREAKPOINT_HPP
#define METASHELL_BREAKPOINT_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <string>
#include <vector>

#include <boost/regex.hpp>
#include <boost/optional.hpp>

#include <metashell/data/frame.hpp>

namespace metashell {

class breakpoint {
public:
  breakpoint() = default;
  explicit breakpoint(const boost::regex& name_regex);

  // TODO: type overload should be removed, once this gets more complicated
  bool match(const data::type& type) const;
  bool match(const data::frame& frame) const;

  std::string to_string() const;
private:
  boost::optional<boost::regex> name_regex;
};

using breakpoints_t = std::vector<breakpoint>;

} // namespace metashell

#endif
