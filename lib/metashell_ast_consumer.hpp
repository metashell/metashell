#ifndef METASHELL_METASHELL_AST_CONSUMER_HPP
#define METASHELL_METASHELL_AST_CONSUMER_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <clang/AST/ASTConsumer.h>

namespace metashell
{
  class metashell_ast_consumer : public clang::ASTConsumer
  {
  public:
    void set_context(clang::ASTContext& ctx_);

    clang::ASTContext* context();
  private:
    clang::ASTContext* _ctx;
  };
}

#endif

