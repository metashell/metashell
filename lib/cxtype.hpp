#ifndef METASHELL_CXTYPE_HPP
#define METASHELL_CXTYPE_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxtypekind.hpp"
#include "indexing_iterator.hpp"

#include <clang-c/Index.h>

#include <string>

namespace metashell
{
  class cxcursor;

  class cxtype
  {
  public:
    cxtype();
    explicit cxtype(CXType type_);

    std::string spelling() const;
    cxtypekind kind() const;

    cxtype pointee_type() const;
    cxtype canonical_type() const;

    cxcursor declaration() const;

    bool function_pointer() const;

    cxtype element_type() const;
    long long num_elements() const;

    cxtype result_type() const;

    typedef indexing_iterator<cxtype> arg_iterator;
    arg_iterator args_begin() const;
    arg_iterator args_end() const;
  private:
    CXType _type;
  };
}

#endif

