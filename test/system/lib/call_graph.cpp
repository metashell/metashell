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

#include <metashell/system_test/call_graph.hpp>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <boost/range/algorithm/equal.hpp>

#include <stdexcept>

using namespace metashell::system_test;

call_graph::call_graph(std::vector<call_graph_node> call_graph_nodes_)
  : _call_graph_nodes(call_graph_nodes_)
{
}

call_graph::call_graph(const json_string& s_)
{
  rapidjson::Document d;
  d.Parse(s_.get().c_str());
  if (d.IsObject() && d.HasMember("type") &&
      is_string("call_graph", d["type"]) && d.HasMember("nodes") &&
      d["nodes"].IsArray() && no_other_members_than({"type", "nodes"}, d))
  {
    const auto& nodes = d["nodes"];
    _call_graph_nodes.reserve(nodes.Size());
    for (auto i = nodes.Begin(); i != nodes.End(); ++i)
    {
      _call_graph_nodes.push_back(call_graph_node(*i));
    }
  }
  else
  {
    throw std::runtime_error("Invalid call_graph: " + s_.get());
  }
}

call_graph::iterator call_graph::begin() const
{
  return _call_graph_nodes.begin();
}

call_graph::iterator call_graph::end() const { return _call_graph_nodes.end(); }

std::ostream& metashell::system_test::operator<<(std::ostream& o_,
                                                 const call_graph& c_)
{
  return o_ << to_json_string(c_);
}

json_string metashell::system_test::to_json_string(const call_graph& c_)
{
  rapidjson::StringBuffer buff;
  rapidjson::Writer<rapidjson::StringBuffer> w(buff);

  w.StartObject();

  w.Key("type");
  w.String("call_graph");

  w.Key("nodes");
  w.StartArray();
  for (const call_graph_node& n : c_)
  {
    w.StartObject();

    w.Key("name");
    const std::string name = n.current_frame().name().name();
    w.String(name.c_str());

    if (n.current_frame().has_kind())
    {
      w.Key("kind");
      const std::string kind = to_string(n.current_frame().kind());
      w.String(kind.c_str());
    }

    w.Key("depth");
    w.Int(n.depth());

    w.Key("children");
    w.Int(n.number_of_children());

    w.EndObject();
  }
  w.EndArray();

  w.EndObject();

  return json_string(buff.GetString());
}

bool metashell::system_test::operator==(const call_graph& c_,
                                        const json_string& s_)
{
  return c_ == call_graph(s_);
}

bool metashell::system_test::operator==(const call_graph& c_,
                                        const call_graph& s_)
{
  return boost::equal(c_, s_);
}
