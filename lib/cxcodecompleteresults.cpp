// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxcodecompleteresults.hpp"
#include "cxstring.hpp"

using namespace metashell;

#include <iostream>

namespace
{
  template <class Cont>
  void add(const CXCompletionString& s_, Cont& out_)
  {
    for (int i = 0, e = clang_getNumCompletionChunks(s_); i != e; ++i)
    {
      if (clang_getCompletionChunkKind(s_, i) == CXCompletionChunk_TypedText)
      {
        out_.insert(cxstring(clang_getCompletionChunkText(s_, i)));
      }
    }
  }
}

cxcodecompleteresults::cxcodecompleteresults(CXCodeCompleteResults* r_) :
  _r(r_)
{
}

cxcodecompleteresults::~cxcodecompleteresults()
{
  clang_disposeCodeCompleteResults(_r);
}

void cxcodecompleteresults::fill(std::set<std::string>& out_) const
{
  out_.clear();

  if (_r)
  {
    for (unsigned int i = 0; i != _r->NumResults; ++i)
    {
      add(_r->Results[i].CompletionString, out_);
    }
  }
}


