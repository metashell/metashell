//===--- ActiveTemplateInst.h - Active Template Instantiation Records --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the ActiveTemplateInstantiation class, which represents
// a template instantiation during semantic analysis.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_ACTIVE_TEMPLATE_INST_H
#define LLVM_CLANG_ACTIVE_TEMPLATE_INST_H

#include "clang/Basic/SourceLocation.h"

namespace clang {
  
  class Decl;
  class NamedDecl;
  class TemplateArgument;

namespace sema {
  class TemplateDeductionInfo;
}


/// \brief A template instantiation that is currently in progress.
class ActiveTemplateInstantiation {
public:
  /// \brief The kind of template instantiation we are performing
  enum InstantiationKind {
    /// We are instantiating a template declaration. The entity is
    /// the declaration we're instantiating (e.g., a CXXRecordDecl).
    TemplateInstantiation,

    /// We are instantiating a default argument for a template
    /// parameter. The Entity is the template, and
    /// TemplateArgs/NumTemplateArguments provides the template
    /// arguments as specified.
    /// FIXME: Use a TemplateArgumentList
    DefaultTemplateArgumentInstantiation,

    /// We are instantiating a default argument for a function.
    /// The Entity is the ParmVarDecl, and TemplateArgs/NumTemplateArgs
    /// provides the template arguments as specified.
    DefaultFunctionArgumentInstantiation,

    /// We are substituting explicit template arguments provided for
    /// a function template. The entity is a FunctionTemplateDecl.
    ExplicitTemplateArgumentSubstitution,

    /// We are substituting template argument determined as part of
    /// template argument deduction for either a class template
    /// partial specialization or a function template. The
    /// Entity is either a ClassTemplatePartialSpecializationDecl or
    /// a FunctionTemplateDecl.
    DeducedTemplateArgumentSubstitution,

    /// We are substituting prior template arguments into a new
    /// template parameter. The template parameter itself is either a
    /// NonTypeTemplateParmDecl or a TemplateTemplateParmDecl.
    PriorTemplateArgumentSubstitution,

    /// We are checking the validity of a default template argument that
    /// has been used when naming a template-id.
    DefaultTemplateArgumentChecking,

    /// We are instantiating the exception specification for a function
    /// template which was deferred until it was needed.
    ExceptionSpecInstantiation,
    
    /// Added for Template instantiation observation
    /// Memoization means we are _not_ instantiating a template because 
    /// it is already instantiated (but we entered a context where we 
    /// would have had to if it was not already instantiated).
    Memoization
    
  } Kind;

  /// \brief The point of instantiation within the source code.
  SourceLocation PointOfInstantiation;

  /// \brief The template (or partial specialization) in which we are
  /// performing the instantiation, for substitutions of prior template
  /// arguments.
  NamedDecl *Template;

  /// \brief The entity that is being instantiated.
  Decl *Entity;

  /// \brief The list of template arguments we are substituting, if they
  /// are not part of the entity.
  const TemplateArgument *TemplateArgs;

  /// \brief The number of template arguments in TemplateArgs.
  unsigned NumTemplateArgs;

  /// \brief The template deduction info object associated with the
  /// substitution or checking of explicit or deduced template arguments.
  sema::TemplateDeductionInfo *DeductionInfo;

  /// \brief The source range that covers the construct that cause
  /// the instantiation, e.g., the template-id that causes a class
  /// template instantiation.
  SourceRange InstantiationRange;

  ActiveTemplateInstantiation()
    : Kind(TemplateInstantiation), Template(0), Entity(0), TemplateArgs(0),
      NumTemplateArgs(0), DeductionInfo(0) {}

  /// \brief Determines whether this template is an actual instantiation
  /// that should be counted toward the maximum instantiation depth.
  bool isInstantiationRecord() const;

};


bool operator==(const ActiveTemplateInstantiation &X,
                const ActiveTemplateInstantiation &Y);

inline
bool operator!=(const ActiveTemplateInstantiation &X,
                const ActiveTemplateInstantiation &Y) {
  return !(X == Y);
}



}


#endif



