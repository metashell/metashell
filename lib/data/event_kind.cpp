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

namespace metashell
{
  namespace data
  {
    std::ostream& operator<<(std::ostream& os, event_kind kind)
    {
      return os << to_string(kind);
    }

    std::string to_string(event_kind kind)
    {
      switch (kind)
      {
      case event_kind::macro_expansion:
        return "MacroExpansion";
      case event_kind::macro_definition:
        return "MacroDefinition";
      case event_kind::macro_deletion:
        return "MacroDeletion";
      case event_kind::rescanning:
        return "Rescanning";
      case event_kind::expanded_code:
        return "ExpandedCode";
      case event_kind::generated_token:
        return "GeneratedToken";
      case event_kind::skipped_token:
        return "SkippedToken";
      case event_kind::quote_include:
        return "QuoteInclude";
      case event_kind::sys_include:
        return "SysInclude";
      case event_kind::preprocessing_condition:
        return "PreprocessingCondition";
      case event_kind::preprocessing_condition_result:
        return "PreprocessingConditionResult";
      case event_kind::preprocessing_else:
        return "PreprocessingElse";
      case event_kind::preprocessing_endif:
        return "PreprocessingEndif";
      case event_kind::error_directive:
        return "ErrorDirective";
      case event_kind::line_directive:
        return "LineDirective";
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
      case event_kind::declaring_special_member:
        return "DeclaringSpecialMember";
      case event_kind::defining_synthesized_function:
        return "DefiningSynthesizedFunction";
      case event_kind::memoization:
        return "Memoization";
      case event_kind::non_template_type:
        return "NonTemplateType";
      }
      return "UnknownKind";
    }
  }
}
