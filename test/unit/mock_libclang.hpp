#ifndef METASHELL_MOCK_LIBCLANG_HPP
#define METASHELL_MOCK_LIBCLANG_HPP

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

#include <metashell/iface/libclang.hpp>

#include <deque>

namespace metashell
{
  class mock_libclang : public iface::libclang
  {
  public:
    virtual std::unique_ptr<iface::cxindex> create_index(
      const iface::environment& env_,
      logger* logger_
    ) override;

    void create_index_returns(std::unique_ptr<iface::cxindex> result_);
  private:
    std::deque<std::unique_ptr<iface::cxindex>> _create_index_results;
  };
}

#endif

