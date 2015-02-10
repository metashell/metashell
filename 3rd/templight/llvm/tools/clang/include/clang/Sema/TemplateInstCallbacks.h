//===- TemplateInstCallbacks.h - Template Instantiation Callbacks - C++ --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
//
// This file defines the TemplateInstantiationCallbacks class, which is the 
// base class for callbacks that will be notified at template instantiations.
//
//===---------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TEMPLATE_INST_CALLBACKS_H
#define LLVM_CLANG_TEMPLATE_INST_CALLBACKS_H

#include "clang/Basic/SourceLocation.h"

#include "llvm/Support/Compiler.h"
#include <memory>

namespace clang {

class ActiveTemplateInstantiation;
class Sema;

/// \brief This is a base class for callbacks that will be notified at every 
/// template instantiation.
class TemplateInstantiationCallbacks {
public:
  
  /// \brief Called before doing AST-parsing.
  void initialize(const Sema &TheSema) {
    this->initializeImpl(TheSema);
    if (NextCallbacks)
      NextCallbacks->initialize(TheSema);
  };
  
  /// \brief Called after AST-parsing is completed.
  void finalize(const Sema &TheSema) {
    this->finalizeImpl(TheSema);
    if (NextCallbacks)
      NextCallbacks->finalize(TheSema);
  };
  
  /// \brief Called when instantiation of a template just began.
  void atTemplateBegin(const Sema &TheSema, 
                       const ActiveTemplateInstantiation &Inst) {
    this->atTemplateBeginImpl(TheSema, Inst);
    if (NextCallbacks)
      NextCallbacks->atTemplateBegin(TheSema, Inst);
  };
  
  /// \brief Called when instantiation of a template is just about to end.
  void atTemplateEnd(const Sema &TheSema, 
                     const ActiveTemplateInstantiation &Inst) {
    this->atTemplateEndImpl(TheSema, Inst);
    if (NextCallbacks)
      NextCallbacks->atTemplateEnd(TheSema, Inst);
  };
  
  virtual ~TemplateInstantiationCallbacks() { };
  
  /// \brief Appends a new observer to the end of this list.
  /// \note This function uses a tail-call recursion.
  static void appendNewCallbacks(
      std::unique_ptr<TemplateInstantiationCallbacks> &CurrentChain,
      TemplateInstantiationCallbacks *NewCallbacks) {
    if (!CurrentChain) {
      CurrentChain.reset(NewCallbacks);
      return;
    };
    appendNewCallbacks(CurrentChain->NextCallbacks, NewCallbacks);
  };
  
protected:
  
  /// \brief Called before doing AST-parsing.
  virtual void initializeImpl(const Sema &TheSema) { };
  /// \brief Called after AST-parsing is completed.
  virtual void finalizeImpl(const Sema &TheSema) { };
  
  /// \brief Called when instantiation of a template just began.
  virtual void atTemplateBeginImpl(const Sema &TheSema, 
                                   const ActiveTemplateInstantiation &Inst) {};
  /// \brief Called when instantiation of a template is just about to end.
  virtual void atTemplateEndImpl(const Sema &TheSema, 
                                 const ActiveTemplateInstantiation &Inst) {};
  
  std::unique_ptr<TemplateInstantiationCallbacks> NextCallbacks;
};

}

#endif
