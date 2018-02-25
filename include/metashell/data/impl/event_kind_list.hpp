// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#ifndef PREPROCESSOR_EVENT_KIND
#error PREPROCESSOR_EVENT_KIND is not defined
#endif

#ifndef TEMPLATE_EVENT_KIND
#error TEMPLATE_EVENT_KIND is not defined
#endif

// clang-format off

PREPROCESSOR_EVENT_KIND(macro_expansion, "MacroExpansion")
PREPROCESSOR_EVENT_KIND(macro_definition, "MacroDefinition")
PREPROCESSOR_EVENT_KIND(macro_deletion, "MacroDeletion")
PREPROCESSOR_EVENT_KIND(rescanning, "Rescanning")
PREPROCESSOR_EVENT_KIND(expanded_code, "ExpandedCode")
PREPROCESSOR_EVENT_KIND(generated_token, "GeneratedToken")
PREPROCESSOR_EVENT_KIND(skipped_token, "SkippedToken")
PREPROCESSOR_EVENT_KIND(quote_include, "QuoteInclude")
PREPROCESSOR_EVENT_KIND(sys_include, "SysInclude")
PREPROCESSOR_EVENT_KIND(preprocessing_condition, "PreprocessingCondition")
PREPROCESSOR_EVENT_KIND(preprocessing_condition_result, "PreprocessingConditionResult")
PREPROCESSOR_EVENT_KIND(preprocessing_else, "PreprocessingElse")
PREPROCESSOR_EVENT_KIND(preprocessing_endif, "PreprocessingEndif")
PREPROCESSOR_EVENT_KIND(error_directive, "ErrorDirective")
PREPROCESSOR_EVENT_KIND(line_directive, "LineDirective")

TEMPLATE_EVENT_KIND(template_instantiation, "TemplateInstantiation")
TEMPLATE_EVENT_KIND(default_template_argument_instantiation, "DefaultTemplateArgumentInstantiation")
TEMPLATE_EVENT_KIND(default_function_argument_instantiation, "DefaultFunctionArgumentInstantiation")
TEMPLATE_EVENT_KIND(explicit_template_argument_substitution, "ExplicitTemplateArgumentSubstitution")
TEMPLATE_EVENT_KIND(deduced_template_argument_substitution, "DeducedTemplateArgumentSubstitution")
TEMPLATE_EVENT_KIND(prior_template_argument_substitution, "PriorTemplateArgumentSubstitution")
TEMPLATE_EVENT_KIND(default_template_argument_checking, "DefaultTemplateArgumentChecking")
TEMPLATE_EVENT_KIND(exception_spec_instantiation, "ExceptionSpecInstantiation")
TEMPLATE_EVENT_KIND(declaring_special_member, "DeclaringSpecialMember")
TEMPLATE_EVENT_KIND(defining_synthesized_function, "DefiningSynthesizedFunction")
TEMPLATE_EVENT_KIND(memoization, "Memoization")
TEMPLATE_EVENT_KIND(non_template_type, "NonTemplateType") // Used only if an evaluation result is not a template

// clang-format on
