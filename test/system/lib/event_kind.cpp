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

#include <metashell/system_test/event_kind.hpp>

#include <iostream>
#include <stdexcept>

using namespace metashell::system_test;

std::string metashell::system_test::to_string(event_kind kind_)
{
  switch (kind_)
  {
  case event_kind::macro_expansion:
    return "MacroExpansion";
  case event_kind::macro_definition:
    return "MacroDefinition";
  case event_kind::rescanning:
    return "Rescanning";
  case event_kind::expanded_code:
    return "ExpandedCode";
  case event_kind::generated_token:
    return "GeneratedToken";
  case event_kind::quote_include:
    return "QuoteInclude";
  case event_kind::sys_include:
    return "SysInclude";

  case event_kind::template_instantiation:
    return "TemplateInstantiation";
  case event_kind::default_template_argument_instantiation:
    return "DefaultTemplateArgumentInstantiation";
  case event_kind::default_function_argument_instantiation:
    return "DefaultFunctionArgumentInstantiation";
  case event_kind::explicit_template_argument_substitution:
    return "ExplicitTemplateArgumentSubstitution";
  case event_kind::deduced_template_argument_substitution:
    return "DeducedTemplateArgumentSubstitution";
  case event_kind::prior_template_argument_substitution:
    return "PriorTemplateArgumentSubstitution";
  case event_kind::default_template_argument_checking:
    return "DefaultTemplateArgumentChecking";
  case event_kind::exception_spec_instantiation:
    return "ExceptionSpecInstantiation";
  case event_kind::memoization:
    return "Memoization";
  case event_kind::non_template_type:
    return "NonTemplateType";
  }
  return "UnknownKind";
}

std::ostream& metashell::system_test::operator<<(std::ostream& o_,
                                                 event_kind kind_)
{
  return o_ << to_string(kind_);
}

event_kind parse_kind(const std::string& kind_)
{
  if (kind_ == "MacroExpansion")
  {
    return event_kind::macro_expansion;
  }
  else if (kind_ == "MacroDefinition")
  {
    return event_kind::macro_definition;
  }
  else if (kind_ == "Rescanning")
  {
    return event_kind::rescanning;
  }
  else if (kind_ == "ExpandedCode")
  {
    return event_kind::expanded_code;
  }
  else if (kind_ == "GeneratedToken")
  {
    return event_kind::generated_token;
  }
  else if (kind_ == "QuoteInclude")
  {
    return event_kind::quote_include;
  }
  else if (kind_ == "SysInclude")
  {
    return event_kind::sys_include;
  }
  else if (kind_ == "TemplateInstantiation")
  {
    return event_kind::template_instantiation;
  }
  else if (kind_ == "DefaultTemplateArgumentInstantiation")
  {
    return event_kind::default_template_argument_instantiation;
  }
  else if (kind_ == "DefaultFunctionArgumentInstantiation")
  {
    return event_kind::default_function_argument_instantiation;
  }
  else if (kind_ == "ExplicitTemplateArgumentSubstitution")
  {
    return event_kind::explicit_template_argument_substitution;
  }
  else if (kind_ == "DeducedTemplateArgumentSubstitution")
  {
    return event_kind::deduced_template_argument_substitution;
  }
  else if (kind_ == "PriorTemplateArgumentSubstitution")
  {
    return event_kind::prior_template_argument_substitution;
  }
  else if (kind_ == "DefaultTemplateArgumentChecking")
  {
    return event_kind::default_template_argument_checking;
  }
  else if (kind_ == "ExceptionSpecInstantiation")
  {
    return event_kind::exception_spec_instantiation;
  }
  else if (kind_ == "Memoization")
  {
    return event_kind::memoization;
  }
  else if (kind_ == "NonTemplateType")
  {
    return event_kind::non_template_type;
  }
  else
  {
    throw std::runtime_error("Invalid instantion_kind: " + kind_);
  }
}
