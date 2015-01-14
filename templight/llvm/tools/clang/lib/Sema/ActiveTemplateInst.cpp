//===--- ActiveTemplateInst.cpp - Active Template Instantiation Records ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements basic operators on the class that represents active template 
// instantiations during semantic analysis.
//
//===----------------------------------------------------------------------===//

#include "clang/Sema/ActiveTemplateInst.h"

#include "llvm/Support/ErrorHandling.h"

namespace clang {

bool operator==(const ActiveTemplateInstantiation &X,
                const ActiveTemplateInstantiation &Y) {
  if (X.Kind != Y.Kind)
    return false;

  if (X.Entity != Y.Entity)
    return false;

  switch (X.Kind) {
    case ActiveTemplateInstantiation::TemplateInstantiation:
    case ActiveTemplateInstantiation::ExceptionSpecInstantiation:
    case ActiveTemplateInstantiation::Memoization:
      return true;
    
    case ActiveTemplateInstantiation::PriorTemplateArgumentSubstitution:
    case ActiveTemplateInstantiation::DefaultTemplateArgumentChecking:
      return X.Template == Y.Template && X.TemplateArgs == Y.TemplateArgs;
    
    case ActiveTemplateInstantiation::DefaultTemplateArgumentInstantiation:
    case ActiveTemplateInstantiation::ExplicitTemplateArgumentSubstitution:
    case ActiveTemplateInstantiation::DeducedTemplateArgumentSubstitution:
    case ActiveTemplateInstantiation::DefaultFunctionArgumentInstantiation:
      return X.TemplateArgs == Y.TemplateArgs;
  }

  llvm_unreachable("Invalid InstantiationKind!");
}

bool ActiveTemplateInstantiation::isInstantiationRecord() const {
  switch (Kind) {
    case TemplateInstantiation:
    case ExceptionSpecInstantiation:
    case DefaultTemplateArgumentInstantiation:
    case DefaultFunctionArgumentInstantiation:
    case ExplicitTemplateArgumentSubstitution:
    case DeducedTemplateArgumentSubstitution:
    case PriorTemplateArgumentSubstitution:
      return true;

    case DefaultTemplateArgumentChecking:
      return false;
    
    // FIXME Should this do a break or not? 
    // Memoization kind should never occur here, so, it doesn't really matter.
    case Memoization:
      break;
  }

  llvm_unreachable("Invalid InstantiationKind!");
}


}

