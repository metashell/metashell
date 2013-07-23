// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxindex.hpp"

#include <clang-c/Index.h>

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/assign/list_of.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <string>
#include <algorithm>
#include <iterator>

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
  using boost::bind;
  using boost::transform_iterator;
  using boost::function;
  using boost::assign::list_of;

  using boost::phoenix::val;
  using boost::phoenix::arg_names::arg1;

  using std::string;
  using std::vector;

  typedef
    transform_iterator<
      function<const char*(string)>,
      vector<string>::const_iterator
    >
    c_str_it;

  const char fn[] = "__metashell_source_file";

  vector<string> args = list_of<string>("-x")("c++")("-std=c++1y");
  std::transform(
    config_.include_path.begin(),
    config_.include_path.end(),
    std::back_insert_iterator<vector<string> >(args),
    "-I" + arg1
  );

  const vector<const char*> argv(
    c_str_it(args.begin(), bind(&string::c_str, _1)),
    c_str_it(args.end())
  );

  CXUnsavedFile uf;
  uf.Filename = fn;
  uf.Contents = src_.c_str();
  uf.Length = src_.size();
  return
    boost::make_shared<cxtranslationunit>(
      clang_parseTranslationUnit(
        _index,
        fn,
        &argv[0],
        argv.size(),
        &uf,
        1,
        CXTranslationUnit_None 
      )
    );
}


