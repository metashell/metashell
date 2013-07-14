// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metashell_ast_consumer.hpp"

using namespace metashell;

void metashell_ast_consumer::set_context(clang::ASTContext& ctx_)
{
  _ctx = &ctx_;
}

clang::ASTContext* metashell_ast_consumer::context()
{
  return _ctx;
}

