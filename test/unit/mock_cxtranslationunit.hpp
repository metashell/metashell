#ifndef METASHELL_MOCK_CXTRANSLATIONUNIT_HPP
#define METASHELL_MOCK_CXTRANSLATIONUNIT_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/cxtranslationunit.hpp>

namespace metashell
{
  class mock_cxtranslationunit : public iface::cxtranslationunit
  {
  public:
    mock_cxtranslationunit();

    virtual void visit_nodes(const visitor& f_) override;

    virtual std::string get_error_string() const override;

    virtual void code_complete(std::set<std::string>& out_) const override;

    void set_error(const std::string& msg_);
  private:
    std::string _error;
  };
}

#endif

