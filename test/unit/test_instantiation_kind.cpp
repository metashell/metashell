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

#include <metashell/data/instantiation_kind.hpp>

#include <just/test.hpp>

#include <string>
#include <sstream>

using namespace metashell;

JUST_TEST_CASE(test_instantiation_kind_print)
{
  using data::instantiation_kind;

  JUST_ASSERT_EQUAL(to_string(instantiation_kind::template_instantiation),
                    "TemplateInstantiation");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::default_template_argument_instantiation),
      "DefaultTemplateArgumentInstantiation");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::default_function_argument_instantiation),
      "DefaultFunctionArgumentInstantiation");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::explicit_template_argument_substitution),
      "ExplicitTemplateArgumentSubstitution");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::deduced_template_argument_substitution),
      "DeducedTemplateArgumentSubstitution");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::prior_template_argument_substitution),
      "PriorTemplateArgumentSubstitution");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::default_template_argument_checking),
      "DefaultTemplateArgumentChecking");
  JUST_ASSERT_EQUAL(to_string(instantiation_kind::exception_spec_instantiation),
                    "ExceptionSpecInstantiation");
  JUST_ASSERT_EQUAL(to_string(instantiation_kind::memoization), "Memoization");
  JUST_ASSERT_EQUAL(
      to_string(instantiation_kind::non_template_type), "NonTemplateType");
}
