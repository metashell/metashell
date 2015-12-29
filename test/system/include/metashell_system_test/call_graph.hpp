#ifndef METASHELL_SYSTEM_TEST_CALL_GRAPH_HPP
#define METASHELL_SYSTEM_TEST_CALL_GRAPH_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell_system_test/call_graph_node.hpp>
#include <metashell_system_test/json_string.hpp>

#include <boost/operators.hpp>

#include <vector>

namespace metashell_system_test
{
  class call_graph : boost::equality_comparable<call_graph, json_string>
  {
  public:
    explicit call_graph(std::vector<call_graph_node> call_graph_nodes_);
  
    typedef std::vector<call_graph_node>::const_iterator iterator;
    typedef iterator const_iterator;
  
    iterator begin() const;
    iterator end() const;
  private:
    std::vector<call_graph_node> _call_graph_nodes;
  };
  
  std::ostream& operator<<(std::ostream& o_, const call_graph& c_);

  json_string to_json_string(const call_graph& c_);

  bool operator==(const call_graph& c_, const json_string& s_);
}

#endif

