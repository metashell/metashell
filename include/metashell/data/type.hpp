#ifndef METASHELL_DATA_TYPE_HPP
#define METASHELL_DATA_TYPE_HPP

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

#include <metashell/data/cpp_code.hpp>

#include <boost/operators.hpp>
#include <boost/optional.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class type : boost::totally_ordered<type>
    {
    public:
      typedef std::string::const_iterator iterator;
      typedef std::string::const_iterator const_iterator;

      type();
      explicit type(const std::string& name_);
      explicit type(const cpp_code& name_);

      const cpp_code& name() const;

      bool is_integral_constant(const type& type_,
                                const std::string& value_) const;

      operator cpp_code() const;

      const_iterator begin() const;
      const_iterator end() const;

    private:
      cpp_code _name;
    };

    std::ostream& operator<<(std::ostream& o_, const type& t_);
    bool operator==(const type& a_, const type& b_);
    bool operator<(const type& a_, const type& b_);

    boost::optional<type> trim_wrap_type(const type& type_);
    bool is_remove_ptr(const type& type_);
  }
}

#endif
