#ifndef METASHELL_METASHELL_PRAGMA_HPP
#define METASHELL_METASHELL_PRAGMA_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/token_iterator.hpp>

#include <boost/optional.hpp>

#include <string>

namespace metashell
{
  class metashell_pragma
  {
  public:
    std::string name() const;

    token_iterator begin() const;
    token_iterator end() const;

    static boost::optional<metashell_pragma> parse(const std::string& s_);
  private:
    std::string _line;
    token_iterator _name;
    token_iterator _begin;
    token_iterator _end;

    metashell_pragma(const std::string& line_);
  };
}

#endif

