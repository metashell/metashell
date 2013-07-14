// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "expression_evaluating_ast_consumer.hpp"

#include <clang/AST/Decl.h>

#include <llvm/Support/Casting.h>

using namespace metashell;
 
bool expression_evaluating_ast_consumer::HandleTopLevelDecl(
  clang::DeclGroupRef d_
)
{
  for (clang::DeclGroupRef::iterator it= d_.begin(); it != d_.end(); ++it)
  {
    if (clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(*it))
    {
      if (vd->getDeclName().getAsString() == "__metashell_v")
      {
        if (
          const clang::PointerType* t =
            llvm::dyn_cast<const clang::PointerType>(
              vd->getType().getTypePtr())
            )
        {
          clang::QualType tt =
            t->getPointeeType().getDesugaredType(*context());
          _out << tt.getAsString();
        }
      }
    }
  }
  return true;
}

std::string expression_evaluating_ast_consumer::result() const
{
  return _out.str();
}

