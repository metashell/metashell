#ifndef METASHELL_FORWARDING_AST_CONSUMER_HPP
#define METASHELL_FORWARDING_AST_CONSUMER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "llvm_defines.hpp"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclGroup.h>

namespace metashell
{
  // It is needed because the CompilerInstance takes ownership of
  // the ASTConsumer
  template <class DestASTConsumer>
  class forwarding_ast_consumer : public clang::ASTConsumer
  {
  public:
    forwarding_ast_consumer(DestASTConsumer& dst_) : _dst(dst_) {}
  
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef d_)
    {
      return _dst.HandleTopLevelDecl(d_);
    }
  private:
    DestASTConsumer& _dst;
  };
}

#endif

