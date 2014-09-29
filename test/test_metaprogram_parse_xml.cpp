// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/metaprogram.hpp>

#include <boost/graph/lookup_edge.hpp>

#include <just/test.hpp>

using namespace metashell;

// Helper function, so there is not much repetition in test code
void test_single_node_templight_parsing(
    const std::string& xml_type, const std::string actual_type,
    const std::string& xml_kind, instantiation_kind actual_kind)
{
  const std::string xml =
  "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
  "<Trace>\n"
  "<TemplateBegin>\n"
  "<Kind>" + xml_kind + "</Kind>\n"
  "<Context context = \"" + xml_type + "\"/>\n"
  "<PointOfInstantiation>foo.hpp|10|20</PointOfInstantiation>\n"
  "<TimeStamp time = \"50.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateBegin>\n"
  "<TemplateEnd>\n"
  "<Kind>" + xml_kind + "</Kind>\n"
  "<TimeStamp time = \"100.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateEnd>\n"
  "</Trace>\n";

  metaprogram mp = metaprogram::create_from_xml_string(
      xml, "some_type", "the_result_type");

  JUST_ASSERT_EQUAL(mp.get_evaluation_result(), "the_result_type");
  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 2u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 1u);
  JUST_ASSERT_EQUAL(mp.get_vertex_property(0).name, "some_type");
  JUST_ASSERT_EQUAL(mp.get_vertex_property(1).name, actual_type);

  metaprogram::edge_descriptor edge;
  bool found;
  std::tie(edge, found) = boost::lookup_edge(0, 1, mp.get_graph());

  JUST_ASSERT(found);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge).kind, actual_kind);
}

JUST_TEST_CASE(test_parse_empty_templight_xml)
{
  const std::string xml =
  "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
  "<Trace>\n"
  "</Trace>\n";

  metaprogram mp = metaprogram::create_from_xml_string(
      xml, "some_type", "the_result_type");

  JUST_ASSERT_EQUAL(mp.get_evaluation_result(), "the_result_type");
  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 1u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 0u);
  JUST_ASSERT_EQUAL(mp.get_vertex_property(0).name, "some_type");
}

JUST_TEST_CASE(test_parse_one_node_templight_xml_with_different_kinds)
{
  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "TemplateInstantiation",
       instantiation_kind::template_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "DefaultTemplateArgumentInstantiation",
       instantiation_kind::default_template_argument_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "DefaultFunctionArgumentInstantiation",
       instantiation_kind::default_function_argument_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "ExplicitTemplateArgumentSubstitution",
       instantiation_kind::explicit_template_argument_substitution);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "DeducedTemplateArgumentSubstitution",
       instantiation_kind::deduced_template_argument_substitution);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "PriorTemplateArgumentSubstitution",
       instantiation_kind::prior_template_argument_substitution);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "DefaultTemplateArgumentChecking",
       instantiation_kind::default_template_argument_checking);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "ExceptionSpecInstantiation",
       instantiation_kind::exception_spec_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo", "metashell::foo",
      "Memoization",
       instantiation_kind::memoization);
}

JUST_TEST_CASE(test_parse_one_node_templight_xml_with_different_types)
{
  test_single_node_templight_parsing(
      "metashell::foo&lt;10&gt;", "metashell::foo<10>",
      "TemplateInstantiation", instantiation_kind::template_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo&lt;&apos;a&apos;&gt;", "metashell::foo<'a'>",
      "TemplateInstantiation", instantiation_kind::template_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo&lt;&quot;foobar&quot;&gt;", "metashell::foo<\"foobar\">",
      "TemplateInstantiation", instantiation_kind::template_instantiation);

  test_single_node_templight_parsing(
      "metashell::foo&lt;3&amp;4&gt;", "metashell::foo<3&4>",
      "TemplateInstantiation", instantiation_kind::template_instantiation);
}

JUST_TEST_CASE(test_parse_two_nested_node_templight_xml)
{
  const std::string xml =
  "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
  "<Trace>\n"
  "<TemplateBegin>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<Context context = \"metashell::foo\"/>\n"
  "<PointOfInstantiation>foo.hpp|10|20</PointOfInstantiation>\n"
  "<TimeStamp time = \"50.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateBegin>\n"
  "<TemplateBegin>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<Context context = \"metashell::bar\"/>\n"
  "<PointOfInstantiation>bar.hpp|20|30</PointOfInstantiation>\n"
  "<TimeStamp time = \"60.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateBegin>\n"
  "<TemplateEnd>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<TimeStamp time = \"70.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateEnd>\n"
  "<TemplateEnd>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<TimeStamp time = \"100.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateEnd>\n"
  "</Trace>\n";

  metaprogram mp = metaprogram::create_from_xml_string(
      xml, "some_type", "the_result_type");

  JUST_ASSERT_EQUAL(mp.get_evaluation_result(), "the_result_type");
  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 3u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 2u);
  JUST_ASSERT_EQUAL(mp.get_vertex_property(0).name, "some_type");
  JUST_ASSERT_EQUAL(mp.get_vertex_property(1).name, "metashell::foo");
  JUST_ASSERT_EQUAL(mp.get_vertex_property(2).name, "metashell::bar");

  metaprogram::edge_descriptor edge;
  bool found;
  std::tie(edge, found) = boost::lookup_edge(0, 1, mp.get_graph());

  JUST_ASSERT(found);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge).kind,
      instantiation_kind::template_instantiation);

  std::tie(edge, found) = boost::lookup_edge(1, 2, mp.get_graph());

  JUST_ASSERT(found);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge).kind,
      instantiation_kind::template_instantiation);
}

JUST_TEST_CASE(test_parse_two_sequential_node_templight_xml)
{
  const std::string xml =
  "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
  "<Trace>\n"
  "<TemplateBegin>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<Context context = \"metashell::foo\"/>\n"
  "<PointOfInstantiation>foo.hpp|10|20</PointOfInstantiation>\n"
  "<TimeStamp time = \"50.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateBegin>\n"
  "<TemplateEnd>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<TimeStamp time = \"100.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateEnd>\n"
  "<TemplateBegin>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<Context context = \"metashell::bar\"/>\n"
  "<PointOfInstantiation>bar.hpp|20|30</PointOfInstantiation>\n"
  "<TimeStamp time = \"60.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateBegin>\n"
  "<TemplateEnd>\n"
  "<Kind>TemplateInstantiation</Kind>\n"
  "<TimeStamp time = \"70.0\"/>\n"
  "<MemoryUsage bytes = \"0\"/>\n"
  "</TemplateEnd>\n"
  "</Trace>\n";

  metaprogram mp = metaprogram::create_from_xml_string(
      xml, "some_type", "the_result_type");

  JUST_ASSERT_EQUAL(mp.get_evaluation_result(), "the_result_type");
  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 3u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 2u);
  JUST_ASSERT_EQUAL(mp.get_vertex_property(0).name, "some_type");
  JUST_ASSERT_EQUAL(mp.get_vertex_property(1).name, "metashell::foo");
  JUST_ASSERT_EQUAL(mp.get_vertex_property(2).name, "metashell::bar");

  metaprogram::edge_descriptor edge;
  bool found;
  std::tie(edge, found) = boost::lookup_edge(0, 1, mp.get_graph());

  JUST_ASSERT(found);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge).kind,
      instantiation_kind::template_instantiation);

  std::tie(edge, found) = boost::lookup_edge(0, 2, mp.get_graph());

  JUST_ASSERT(found);
  JUST_ASSERT_EQUAL(mp.get_edge_property(edge).kind,
      instantiation_kind::template_instantiation);
}

