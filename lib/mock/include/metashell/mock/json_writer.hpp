#ifndef METASHELL_MOCK_JSON_WRITER_HPP
#define METASHELL_MOCK_JSON_WRITER_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/json_writer.hpp>

#include <gmock/gmock.h>

namespace metashell
{
  namespace mock
  {
    class json_writer : public iface::json_writer
    {
    public:
      MOCK_METHOD1(string, void(const std::string&));
      MOCK_METHOD1(int_, void(int));
      MOCK_METHOD1(double_, void(double));
      MOCK_METHOD1(bool_, void(bool));

      MOCK_METHOD0(start_object, void());
      MOCK_METHOD1(key, void(const std::string&));
      MOCK_METHOD0(end_object, void());

      MOCK_METHOD0(start_array, void());
      MOCK_METHOD0(end_array, void());

      MOCK_METHOD0(end_document, void());
    };
  } // namespace mock
} // namespace metashell

#endif
