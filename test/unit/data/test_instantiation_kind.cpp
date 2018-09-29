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

#include <metashell/data/event_kind.hpp>

#include <gtest/gtest.h>

#include <sstream>
#include <string>

using namespace metashell;

TEST(event_kind, print)
{
  using data::event_kind;

  ASSERT_EQ(
      to_string(event_kind::template_instantiation), "TemplateInstantiation");
  ASSERT_EQ(to_string(event_kind::default_template_argument_instantiation),
            "DefaultTemplateArgumentInstantiation");
  ASSERT_EQ(to_string(event_kind::default_function_argument_instantiation),
            "DefaultFunctionArgumentInstantiation");
  ASSERT_EQ(to_string(event_kind::explicit_template_argument_substitution),
            "ExplicitTemplateArgumentSubstitution");
  ASSERT_EQ(to_string(event_kind::deduced_template_argument_substitution),
            "DeducedTemplateArgumentSubstitution");
  ASSERT_EQ(to_string(event_kind::prior_template_argument_substitution),
            "PriorTemplateArgumentSubstitution");
  ASSERT_EQ(to_string(event_kind::default_template_argument_checking),
            "DefaultTemplateArgumentChecking");
  ASSERT_EQ(to_string(event_kind::exception_spec_instantiation),
            "ExceptionSpecInstantiation");
  ASSERT_EQ(to_string(event_kind::memoization), "Memoization");
  ASSERT_EQ(to_string(event_kind::non_template_type), "NonTemplateType");
}
