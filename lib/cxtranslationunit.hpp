#ifndef METASHELL_CXTRANSLATIONUNIT_HPP
#define METASHELL_CXTRANSLATIONUNIT_HPP

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

#include <metashell/environment.hpp>
#include <metashell/text_position.hpp>
#include <metashell/unsaved_file.hpp>
#include <metashell/logger.hpp>

#include "cxcursor.hpp"
#include "indexing_iterator.hpp"

#include <clang-c/Index.h>

#include <boost/utility.hpp>

#include <string>
#include <set>
#include <vector>
#include <functional>

namespace metashell
{
  class cxtranslationunit : boost::noncopyable
  {
  public:
    typedef std::function<void(cxcursor, cxcursor)> visitor;

    typedef indexing_iterator<std::string> error_iterator;

    // takes ownership
    cxtranslationunit(
      const environment& env_,
      const unsaved_file& src_,
      CXIndex index_,
      logger* logger_
    );
    ~cxtranslationunit();

    void visit_nodes(const visitor& f_);

    error_iterator errors_begin() const;
    error_iterator errors_end() const;

    bool has_errors() const;

    void code_complete(std::set<std::string>& out_) const;
  private:
    unsaved_file _src;
    std::vector<CXUnsavedFile> _unsaved_files;
    CXTranslationUnit _tu;
    logger* _logger;
  };
}

#endif

