#ifndef METASHELL_MOCK_ENVIRONMENT_HPP
#define METASHELL_MOCK_ENVIRONMENT_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <gmock/gmock.h>

namespace metashell
{
  namespace mock
  {
    class environment : public iface::environment
    {
    public:
      MOCK_METHOD1(append, void(const data::cpp_code&));
      MOCK_CONST_METHOD0(get, data::cpp_code());
      MOCK_CONST_METHOD1(get_appended, data::cpp_code(const data::cpp_code&));

      MOCK_CONST_METHOD0(get_headers, const data::headers&());

      MOCK_CONST_METHOD0(get_all, data::cpp_code());
    };
  } // namespace mock
} // namespace metashell

#endif
