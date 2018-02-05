#ifndef METASHELL_EVENT_KIND_HPP
#define METASHELL_EVENT_KIND_HPP

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

#include <ostream>
#include <string>

namespace metashell
{
  namespace data
  {
    enum class event_kind
    {
      // Preprocessor-related events
      macro_expansion,
      macro_definition,
      macro_deletion,
      rescanning,
      expanded_code,
      generated_token,
      skipped_token,
      quote_include,
      sys_include,
      preprocessing_condition,
      preprocessing_condition_result,
      preprocessing_else,
      preprocessing_endif,
      error_directive,
      line_directive,

      // Template instantiation-related events
      template_instantiation,
      default_template_argument_instantiation,
      default_function_argument_instantiation,
      explicit_template_argument_substitution,
      deduced_template_argument_substitution,
      prior_template_argument_substitution,
      default_template_argument_checking,
      exception_spec_instantiation,
      declaring_special_member,
      defining_synthesized_function,
      memoization,
      non_template_type // Used only if an evaluation result is not a template
    };

    std::ostream& operator<<(std::ostream& os, event_kind kind);

    std::string to_string(event_kind kind);
  }
}

#endif
