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
#include <metashell/headers.hpp>
#include <metashell/exception.hpp>

#include <metashell/data/text_position.hpp>

#include <metashell/clang/cxtranslationunit.hpp>
#include <metashell/clang/cxdiagnostic.hpp>
#include <metashell/clang/cxcodecompleteresults.hpp>

#include <clang-c/Index.h>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/algorithm/string/join.hpp>

#include <functional>
#include <sstream>
#include <iostream>

using namespace metashell::clang;

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

  const char* c_str(const std::string& s_)
  {
    return s_.c_str();
  }

  std::string log_libclang_invocation(
    const std::string& filename_,
    const std::vector<CXUnsavedFile>& unsaved_files_,
    const std::vector<std::string>& clang_args_
  )
  {
    std::ostringstream s;

    s
      << "Building syntax tree with libclang. The source code to compile is "
      << filename_ << std::endl
      << "The unsaved files passed to libclang:" << std::endl;

    for (const CXUnsavedFile& f : unsaved_files_)
    {
      s
        << "  Filename: " << f.Filename << std::endl
        << "  Content: " << std::string(f.Contents, f.Length) << std::endl;
    }

    s
      << "The command line arguments passed to libclang: "
      << boost::algorithm::join(clang_args_, " ");

    return s.str();
  }
}

cxtranslationunit::cxtranslationunit(
  const environment& env_,
  const unsaved_file& src_,
  CXIndex index_,
  logger* logger_
) :
  _src(src_),
  _unsaved_files(),
  _logger(logger_)
{
  using boost::transform_iterator;
  using boost::algorithm::join;

  using std::string;
  using std::vector;

  typedef
    transform_iterator<
      std::function<const char*(const string&)>,
      vector<string>::const_iterator
    >
    c_str_it;

  _unsaved_files.reserve(env_.get_headers().size() + 1);
  for (const unsaved_file& uf : env_.get_headers())
  {
    _unsaved_files.push_back(uf.get());
  }
  _unsaved_files.push_back(_src.get());

  const vector<const char*> argv(
    c_str_it(env_.clang_arguments().begin(), c_str),
    c_str_it(env_.clang_arguments().end())
  );

  METASHELL_LOG(
    _logger,
    log_libclang_invocation(
      src_.filename(),
      _unsaved_files,
      env_.clang_arguments()
    )
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
  if (_tu)
  {
    METASHELL_LOG(_logger, "Syntax tree construction succeeded.");
  }
  else
  {
    METASHELL_LOG(_logger, "Syntax tree construction failed.");
    throw
      exception(
        "Error parsing source code (" + src_.filename() + ": "
        + src_.content() + ")"
      );
  }
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
  return
    error_iterator(
      [this] (int n_) { return get_nth_error_msg(this->_tu, n_); },
      0
    );
}

cxtranslationunit::error_iterator cxtranslationunit::errors_end() const
{
  return
    error_iterator(
      [this] (int n_) { return get_nth_error_msg(this->_tu, n_); },
      clang_getNumDiagnostics(_tu)
    );
}

bool cxtranslationunit::has_errors() const
{
  return clang_getNumDiagnostics(_tu) > 0;
}

void cxtranslationunit::code_complete(std::set<std::string>& out_) const
{
  using metashell::data::text_position;

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


