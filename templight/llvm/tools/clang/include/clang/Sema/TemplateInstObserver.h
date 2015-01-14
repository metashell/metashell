//===--- TemplateInstObserver.h - Template Instantiation Observer --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the TemplateInstantiationObserver class, which is the base
// class for an observer that will be notified (callback) at every template instantiation.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TEMPLATE_INST_OBSERVER_H
#define LLVM_CLANG_TEMPLATE_INST_OBSERVER_H

#include "clang/Basic/SourceLocation.h"

#include "llvm/Support/Compiler.h"
#include <memory>

namespace clang {
  
  class Sema;
  class ActiveTemplateInstantiation;
  
/// \brief This is a base class for an observer that will be notified (callback) at every template instantiation.
class TemplateInstantiationObserver {
public:
  
  /// \brief Called before doing AST-parsing.
  void initialize(const Sema &TheSema) {
    this->initializeImpl(TheSema);
    if ( NextObserver )
      NextObserver->initialize(TheSema);
  };
  
  /// \brief Called after AST-parsing is completed.
  void finalize(const Sema &TheSema) {
    this->finalizeImpl(TheSema);
    if ( NextObserver )
      NextObserver->finalize(TheSema);
  };
  
  /// \brief Called when instantiation of a template just began.
  void atTemplateBegin(const Sema &TheSema, const ActiveTemplateInstantiation &Inst) {
    this->atTemplateBeginImpl(TheSema, Inst);
    if ( NextObserver )
      NextObserver->atTemplateBegin(TheSema, Inst);
  };
  
  /// \brief Called when instantiation of a template is just about to end.
  void atTemplateEnd(const Sema &TheSema, const ActiveTemplateInstantiation& Inst) {
    this->atTemplateEndImpl(TheSema, Inst);
    if ( NextObserver )
      NextObserver->atTemplateEnd(TheSema, Inst);
  };
  
  virtual ~TemplateInstantiationObserver() { };
  
  /// \brief Appends a new observer to the end of this list.
  /// \note This function uses a tail-call recursion (and is not performance-critical).
  static void appendNewObserver(std::unique_ptr<TemplateInstantiationObserver>& CurrentChain,
                                TemplateInstantiationObserver* NewObserver) {
    if ( !CurrentChain ) {
      CurrentChain.reset(NewObserver);
      return;
    };
    appendNewObserver(CurrentChain->NextObserver, NewObserver);
  };
  
protected:
  
  /// \brief Called before doing AST-parsing.
  virtual void initializeImpl(const Sema &TheSema) { };
  /// \brief Called after AST-parsing is completed.
  virtual void finalizeImpl(const Sema &TheSema) { };
  
  /// \brief Called when instantiation of a template just began.
  virtual void atTemplateBeginImpl(const Sema &TheSema, 
                                   const ActiveTemplateInstantiation& Inst) { };
  /// \brief Called when instantiation of a template is just about to end.
  virtual void atTemplateEndImpl(const Sema &TheSema, 
                                 const ActiveTemplateInstantiation& Inst) { };
  
  std::unique_ptr<TemplateInstantiationObserver> NextObserver;
};



}


#endif



