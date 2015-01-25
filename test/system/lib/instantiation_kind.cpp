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

#include <metashell_system_test/instantiation_kind.hpp>

#include <iostream>
#include <stdexcept>

using namespace metashell_system_test;

std::string metashell_system_test::to_string(instantiation_kind kind_)
{
  switch (kind_)
  {
  case instantiation_kind::template_instantiation:
    return "TemplateInstantiation";
  case instantiation_kind::default_template_argument_instantiation:
    return "DefaultTemplateArgumentInstantiation";
  case instantiation_kind::default_function_argument_instantiation:
    return "DefaultFunctionArgumentInstantiation";
  case instantiation_kind::explicit_template_argument_substitution:
    return "ExplicitTemplateArgumentSubstitution";
  case instantiation_kind::deduced_template_argument_substitution:
    return "DeducedTemplateArgumentSubstitution";
  case instantiation_kind::prior_template_argument_substitution:
    return "PriorTemplateArgumentSubstitution";
  case instantiation_kind::default_template_argument_checking:
    return "DefaultTemplateArgumentChecking";
  case instantiation_kind::exception_spec_instantiation:
    return "ExceptionSpecInstantiation";
  case instantiation_kind::memoization:
    return "Memoization";
  case instantiation_kind::non_template_type:
    return "NonTemplateType";
  }
  return "UnknownKind";
}

std::ostream& metashell_system_test::operator<<(
  std::ostream& o_,
  instantiation_kind kind_
)
{
  return o_ << to_string(kind_);
}

instantiation_kind parse_kind(const std::string& kind_)
{
  if (kind_ == "TemplateInstantiation")
  {
    return instantiation_kind::template_instantiation;
  }
  else if (kind_ == "DefaultTemplateArgumentInstantiation")
  {
    return instantiation_kind::default_template_argument_instantiation;
  }
  else if (kind_ == "DefaultFunctionArgumentInstantiation")
  {
    return instantiation_kind::default_function_argument_instantiation;
  }
  else if (kind_ == "ExplicitTemplateArgumentSubstitution")
  {
    return instantiation_kind::explicit_template_argument_substitution;
  }
  else if (kind_ == "DeducedTemplateArgumentSubstitution")
  {
    return instantiation_kind::deduced_template_argument_substitution;
  }
  else if (kind_ == "PriorTemplateArgumentSubstitution")
  {
    return instantiation_kind::prior_template_argument_substitution;
  }
  else if (kind_ == "DefaultTemplateArgumentChecking")
  {
    return instantiation_kind::default_template_argument_checking;
  }
  else if (kind_ == "ExceptionSpecInstantiation")
  {
    return instantiation_kind::exception_spec_instantiation;
  }
  else if (kind_ == "Memoization")
  {
    return instantiation_kind::memoization;
  }
  else if (kind_ == "NonTemplateType")
  {
    return instantiation_kind::non_template_type;
  }
  else
  {
    throw std::runtime_error("Invalid instantion_kind: " + kind_);
  }
}

