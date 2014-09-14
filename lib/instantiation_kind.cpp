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

#include <metashell/instantiation_kind.hpp>

#include <sstream>

namespace metashell {

std::ostream& operator<<(std::ostream& os, instantiation_kind kind) {
  switch (kind) {
    default:
        os << "UnknownKind"; break;
    case template_instantiation:
        os << "TemplateInstantiation"; break;
    case default_template_argument_instantiation:
        os << "DefaultTemplateArgumentInstantiation"; break;
    case default_function_argument_instantiation:
        os << "DefaultFunctionArgumentInstantiation"; break;
    case explicit_template_argument_substitution:
        os << "ExplicitTemplateArgumentSubstitution"; break;
    case deduced_template_argument_substitution:
        os << "DeducedTemplateArgumentSubstitution"; break;
    case prior_template_argument_substitution:
        os << "PriorTemplateArgumentSubstitution"; break;
    case default_template_argument_checking:
        os << "DefaultTemplateArgumentChecking"; break;
    case exception_spec_instantiation:
        os << "ExceptionSpecInstantiation"; break;
    case memoization:
        os << "Memoization"; break;
  }
  return os;
}

std::string to_string(instantiation_kind kind) {
  std::stringstream ss;
  ss << kind;
  return ss.str();
}

}

