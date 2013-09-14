// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <metashell/shell.hpp>
#include <metashell/text_position.hpp>

#include "cxtranslationunit.hpp"
#include "cxdiagnostic.hpp"
#include "headers.hpp"
#include "cxcodecompleteresults.hpp"

#include <clang-c/Index.h>

#include <boost/bind.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

using namespace metashell;

namespace
{
  CXChildVisitResult visitor(
    CXCursor cursor_,
    CXCursor parent_,
    CXClientData client_data_
  )
  {
    const cxtranslationunit::visitor& f =
      *static_cast<cxtranslationunit::visitor*>(client_data_);

    f(cxcursor(cursor_), cxcursor(parent_));
    return CXChildVisit_Recurse;
  }

  std::string get_nth_error_msg(CXTranslationUnit tu_, int n_)
  {
    return cxdiagnostic(clang_getDiagnostic(tu_, n_)).spelling();
  }
}

cxtranslationunit::cxtranslationunit(
  const config& config_,
  const std::string& src_,
  CXIndex index_
) :
  _headers(src_)
{
  using boost::bind;
  using boost::transform_iterator;
  using boost::function;
  using boost::assign::list_of;

  using boost::phoenix::arg_names::arg1;

  using std::string;
  using std::vector;

  typedef
    transform_iterator<
      function<const char*(string)>,
      vector<string>::const_iterator
    >
    c_str_it;

  vector<string> args = list_of<string>
    ("-x")("c++")("-std=c++1y")("-I" + _headers.internal_dir());
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

  std::vector<CXUnsavedFile> uf(_headers.begin(), _headers.end());
  _tu =
    clang_parseTranslationUnit(
      index_,
      shell::input_filename(),
      &argv[0],
      argv.size(),
      &uf[0],
      uf.size(),
      CXTranslationUnit_None 
    );
}

cxtranslationunit::~cxtranslationunit()
{
  clang_disposeTranslationUnit(_tu);
}

void cxtranslationunit::visit_nodes(const visitor& f_)
{
  clang_visitChildren(
    clang_getTranslationUnitCursor(_tu),
    ::visitor,
    const_cast<visitor*>(&f_)
  );
}

cxtranslationunit::error_iterator cxtranslationunit::errors_begin() const
{
  return error_iterator(boost::bind(get_nth_error_msg, _tu, _1), 0);
}

cxtranslationunit::error_iterator cxtranslationunit::errors_end() const
{
  return
    error_iterator(
      boost::bind(get_nth_error_msg, _tu, _1),
      clang_getNumDiagnostics(_tu)
    );
}

bool cxtranslationunit::has_errors() const
{
  return clang_getNumDiagnostics(_tu) > 0;
}

void cxtranslationunit::code_complete(std::set<std::string>& out_) const
{
  std::vector<CXUnsavedFile> uf(_headers.begin(), _headers.end());
  const text_position pos = text_position() + _headers[shell::input_filename()];
  cxcodecompleteresults(
    clang_codeCompleteAt(
      _tu,
      shell::input_filename(),
      pos.line(),
      pos.column() - 1, // -1 because of the extra space at the end of the input
      &uf[0],
      uf.size(),
      clang_defaultCodeCompleteOptions()
    )
  ).fill(out_);
}


