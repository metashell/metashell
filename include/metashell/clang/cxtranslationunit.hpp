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

#include <metashell/iface/environment.hpp>
#include <metashell/logger.hpp>

#include <metashell/iface/cxtranslationunit.hpp>

#include <metashell/data/unsaved_file.hpp>
#include <metashell/data/text_position.hpp>

#include <metashell/clang/cxcursor.hpp>

#include <clang-c/Index.h>

#include <vector>

namespace metashell
{
  namespace clang
  {
    class cxtranslationunit : public iface::cxtranslationunit
    {
    public:
      cxtranslationunit(
        const iface::environment& env_,
        const data::unsaved_file& src_,
        CXIndex index_,
        logger* logger_
      );
      ~cxtranslationunit();

      virtual void visit_nodes(const visitor& f_) override;

      virtual std::string get_error_string() const override;

      virtual void code_complete(std::set<std::string>& out_) const override;
    private:
      data::unsaved_file _src;
      std::vector<CXUnsavedFile> _unsaved_files;
      CXTranslationUnit _tu;
      logger* _logger;
    };
  }
}

#endif

