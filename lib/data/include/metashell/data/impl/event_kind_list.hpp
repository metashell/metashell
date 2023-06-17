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

#ifndef MISC_EVENT_KIND
#error MISC_EVENT_KIND is not defined
#endif

#ifdef EVENT_KIND_SEP
#error EVENT_KIND_SEP already defined
#endif
#define EVENT_KIND_SEP ,

// clang-format off

PREPROCESSOR_EVENT_KIND(macro_expansion,                     "MacroExpansion",                       open)
PREPROCESSOR_EVENT_KIND(macro_expansion_end,                 "MacroExpansionEnd",                    close)
PREPROCESSOR_EVENT_KIND(macro_definition,                    "MacroDefinition",                      flat)
PREPROCESSOR_EVENT_KIND(macro_deletion,                      "MacroDeletion",                        flat)
PREPROCESSOR_EVENT_KIND(rescanning,                          "Rescanning",                           open)
PREPROCESSOR_EVENT_KIND(rescanning_end,                      "RescanningEnd",                        close)
PREPROCESSOR_EVENT_KIND(expanded_code,                       "ExpandedCode",                         flat)
PREPROCESSOR_EVENT_KIND(generated_token,                     "GeneratedToken",                       flat)
PREPROCESSOR_EVENT_KIND(skipped_token,                       "SkippedToken",                         flat)
PREPROCESSOR_EVENT_KIND(quote_include,                       "QuoteInclude",                         open)
PREPROCESSOR_EVENT_KIND(sys_include,                         "SysInclude",                           open)
PREPROCESSOR_EVENT_KIND(include_end,                         "IncludeEnd",                           close)
PREPROCESSOR_EVENT_KIND(preprocessing_condition,             "PreprocessingCondition",               open)
PREPROCESSOR_EVENT_KIND(preprocessing_condition_result,      "PreprocessingConditionResult",         flat)
PREPROCESSOR_EVENT_KIND(preprocessing_condition_end,         "PreprocessingConditionEnd",            close)
PREPROCESSOR_EVENT_KIND(preprocessing_else,                  "PreprocessingElse",                    flat)
PREPROCESSOR_EVENT_KIND(preprocessing_endif,                 "PreprocessingEndif",                   flat)
PREPROCESSOR_EVENT_KIND(error_directive,                     "ErrorDirective",                       flat)
PREPROCESSOR_EVENT_KIND(line_directive,                      "LineDirective",                        flat)

TEMPLATE_EVENT_KIND(template_instantiation,                  "TemplateInstantiation",                open)
TEMPLATE_EVENT_KIND(default_template_argument_instantiation, "DefaultTemplateArgumentInstantiation", open)
TEMPLATE_EVENT_KIND(default_function_argument_instantiation, "DefaultFunctionArgumentInstantiation", open)
TEMPLATE_EVENT_KIND(explicit_template_argument_substitution, "ExplicitTemplateArgumentSubstitution", open)
TEMPLATE_EVENT_KIND(deduced_template_argument_substitution,  "DeducedTemplateArgumentSubstitution",  open)
TEMPLATE_EVENT_KIND(prior_template_argument_substitution,    "PriorTemplateArgumentSubstitution",    open)
TEMPLATE_EVENT_KIND(default_template_argument_checking,      "DefaultTemplateArgumentChecking",      open)
TEMPLATE_EVENT_KIND(exception_spec_evaluation,               "ExceptionSpecEvaluation",              flat)
TEMPLATE_EVENT_KIND(exception_spec_instantiation,            "ExceptionSpecInstantiation",           open)
TEMPLATE_EVENT_KIND(requirement_instantiation,               "RequirementInstantiation",             flat)
TEMPLATE_EVENT_KIND(nested_requirement_constraints_check,    "NestedRequirementConstraintsCheck",    flat)
TEMPLATE_EVENT_KIND(declaring_special_member,                "DeclaringSpecialMember",               open)
TEMPLATE_EVENT_KIND(declaring_implicit_equality_comparison,  "DeclaringImplicitEqualityComparison",  flat)
TEMPLATE_EVENT_KIND(defining_synthesized_function,           "DefiningSynthesizedFunction",          open)
TEMPLATE_EVENT_KIND(constraints_check,                       "ConstraintsCheck",                     flat)
TEMPLATE_EVENT_KIND(constraint_substitution,                 "ConstraintSubstitution",               flat)
TEMPLATE_EVENT_KIND(constraint_normalization,                "ConstraintNormalization",              flat)
TEMPLATE_EVENT_KIND(requirement_parameter_instantiation,     "RequirementParameterInstantiation",    flat)
TEMPLATE_EVENT_KIND(parameter_mapping_substitution,          "ParameterMappingSubstitution",         flat)
TEMPLATE_EVENT_KIND(rewriting_operator_as_spaceship,         "RewritingOperatorAsSpaceship",         flat)
TEMPLATE_EVENT_KIND(initializing_structured_binding,         "InitializingStructuredBinding",        flat)
TEMPLATE_EVENT_KIND(marking_class_dll_exported,              "MarkingClassDllexported",              flat)
TEMPLATE_EVENT_KIND(building_builtin_dump_struct_call,       "BuildingBuiltinDumpStructCall",        open)
TEMPLATE_EVENT_KIND(memoization,                             "Memoization",                          open)
TEMPLATE_EVENT_KIND(template_end,                            "TemplateEnd",                          close)
TEMPLATE_EVENT_KIND(non_template_type,                       "NonTemplateType",                      open) // Used only if an evaluation result is not a template

#undef EVENT_KIND_SEP
#define EVENT_KIND_SEP
MISC_EVENT_KIND(evaluation_end,                              "EvaluationEnd",                        end)

// clang-format on

#undef EVENT_KIND_SEP
