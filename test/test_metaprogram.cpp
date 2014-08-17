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

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_parse_empty_templight_xml)
{
  const std::string empty_xml =
  "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
  "<Trace>\n"
  "</Trace>\n";

  metaprogram mp = metaprogram::create_from_xml_string(empty_xml);

  JUST_ASSERT_EQUAL(mp.get_num_vertices(), 1u);
  JUST_ASSERT_EQUAL(mp.get_num_edges(), 0u);
  JUST_ASSERT_EQUAL(mp.get_vertex_property(0).name, "<root>");
}

