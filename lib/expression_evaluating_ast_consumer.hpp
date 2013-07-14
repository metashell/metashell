#ifndef METASHELL_EXPRESSION_EVALUATING_AST_CONSUMER_HPP
#define METASHELL_EXPRESSION_EVALUATING_AST_CONSUMER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "llvm_defines.hpp"

#include "metashell_ast_consumer.hpp"

#include <clang/AST/DeclGroup.h>

#include <sstream>
 
namespace metashell
{
  class expression_evaluating_ast_consumer : public metashell_ast_consumer
  {
  public:
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef d_);
  
    std::string result() const;
  private:
    std::ostringstream _out;
  };
}

#endif

