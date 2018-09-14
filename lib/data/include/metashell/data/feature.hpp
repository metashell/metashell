#ifndef METASHELL_FEATURE_HPP
#define METASHELL_FEATURE_HPP

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

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {
    class feature : boost::totally_ordered<feature>
    {
    public:
      static feature type_shell();
      static feature preprocessor_shell();
      static feature code_completer();
      static feature header_discoverer();
      static feature metaprogram_tracer();
      static feature cpp_validator();
      static feature macro_discovery();
      static feature preprocessor_tracer();

      static std::vector<feature> all();

      std::string description() const;

      int id() const;

    private:
      int _id;

      explicit feature(int id_);
    };

    bool operator<(feature a_, feature b_);
    bool operator==(feature a_, feature b_);
    std::ostream& operator<<(std::ostream& o_, feature f_);
    std::string to_string(feature f_);
  }
}

#endif
