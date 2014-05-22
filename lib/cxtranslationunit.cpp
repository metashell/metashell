// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <metashell/shell.hpp>
#include <metashell/text_position.hpp>

#include "cxtranslationunit.hpp"
#include "cxdiagnostic.hpp"
#include "cxcodecompleteresults.hpp"

#include <clang-c/Index.h>

#include <boost/foreach.hpp>
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

  template <class Cont>
  void add_with_prefix(
    const std::string& prefix_,
    const Cont& cont_,
    std::vector<std::string>& v_
  )
  {
    using boost::phoenix::arg_names::arg1;

    std::transform(
      cont_.begin(),
      cont_.end(),
      std::back_insert_iterator<std::vector<std::string> >(v_),
      prefix_ + arg1
    );
  }

  const char* c_str(const std::string& s_)
  {
    return s_.c_str();
  }
}

namespace
{
  std::vector<std::string> get_clang_args(
    const config& config_,
    const environment& env_
  )
  {
    using boost::assign::list_of;

    std::vector<std::string> args = list_of<std::string>
      ("-x")("c++")
      (clang_argument(config_.standard_to_use))
      ("-I" + env_.internal_dir());
    add_with_prefix("-I", config_.include_path, args);
    add_with_prefix("-D", config_.macros, args);

    if (!config_.warnings_enabled)
    {
      args.push_back("-w");
    }

    args.insert(
      args.end(),
      config_.extra_clang_args.begin(),
      config_.extra_clang_args.end()
    );

    args.insert(
      args.end(),
      env_.extra_clang_arguments().begin(),
      env_.extra_clang_arguments().end()
    );

    return args;
  }
}

cxtranslationunit::cxtranslationunit(
  const config& config_,
  const environment& env_,
  const unsaved_file& src_,
  CXIndex index_
) :
  _headers(env_.internal_dir()),
  _src(src_),
  _unsaved_files()
{
  using boost::bind;
  using boost::transform_iterator;
  using boost::function;

  using std::string;
  using std::vector;

  typedef
    transform_iterator<
      function<const char*(string)>,
      vector<string>::const_iterator
    >
    c_str_it;

  _unsaved_files.reserve(_headers.size() + 1);
  BOOST_FOREACH(const unsaved_file& uf, _headers)
  {
    _unsaved_files.push_back(uf.get());
  }
  _unsaved_files.push_back(_src.get());

  const vector<string> args = get_clang_args(config_, env_);

  const vector<const char*> argv(
    c_str_it(args.begin(), c_str),
    c_str_it(args.end())
  );

  _tu =
    clang_parseTranslationUnit(
      index_,
      _src.filename().c_str(),
      &argv[0],
      argv.size(),
      &_unsaved_files[0],
      _unsaved_files.size(),
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
  const text_position pos = text_position() + _src.content();
  cxcodecompleteresults(
    clang_codeCompleteAt(
      _tu,
      _src.filename().c_str(),
      pos.line(),
      pos.column() - 1, // -1 because of the extra space at the end of the input
      // I assume this won't be changed
      const_cast<CXUnsavedFile*>(&_unsaved_files[0]),
      _unsaved_files.size(),
      clang_defaultCodeCompleteOptions()
    )
  ).fill(out_);
}


