#ifndef METASHELL_DATA_METAPROGRAM_NODE_HPP
#define METASHELL_DATA_METAPROGRAM_NODE_HPP

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

#include <metashell/data/cpp_code.hpp>
#include <metashell/data/token.hpp>
#include <metashell/data/type.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/operators.hpp>

#include <variant.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class metaprogram_node : boost::equality_comparable<metaprogram_node>
    {
    public:
      metaprogram_node();
      metaprogram_node(type);
      metaprogram_node(token);
      metaprogram_node(cpp_code);
      metaprogram_node(boost::filesystem::path);

      const mpark::variant<type, token, cpp_code, boost::filesystem::path>&
      variant() const;

    private:
      mpark::variant<type, token, cpp_code, boost::filesystem::path> _value;
    };

    std::string to_string(const metaprogram_node&);
    std::ostream& operator<<(std::ostream&, const metaprogram_node&);

    bool operator==(const metaprogram_node&, const metaprogram_node&);

    template <class Visitor>
    void visit(Visitor&& visitor_, const metaprogram_node& n_)
    {
      mpark::visit(std::forward<Visitor>(visitor_), n_.variant());
    }
  }
}

#endif
