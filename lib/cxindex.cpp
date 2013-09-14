// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxindex.hpp"

#include <clang-c/Index.h>

#include <boost/make_shared.hpp>

using namespace metashell;

cxindex::cxindex() : _index(clang_createIndex(0, 0)) {}

cxindex::~cxindex()
{
  clang_disposeIndex(_index);
}

boost::shared_ptr<cxtranslationunit> cxindex::parse_code(
  const std::string& src_,
  const config& config_
)
{
  return boost::make_shared<cxtranslationunit>(config_, src_, _index);
}


