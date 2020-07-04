#ifndef METASHELL_MOCK_COMMAND_PROCESSOR_HPP
#define METASHELL_MOCK_COMMAND_PROCESSOR_HPP

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

#include <metashell/iface/command_processor.hpp>

#include <gmock/gmock.h>

namespace metashell
{
  namespace mock
  {
    class command_processor : public iface::command_processor
    {
    public:
      MOCK_METHOD3(line_available,
                   void(const data::user_input&,
                        iface::displayer&,
                        iface::history&));
      MOCK_METHOD0(cancel_operation, void());

      MOCK_CONST_METHOD0(prompt, std::string());
      MOCK_CONST_METHOD0(stopped, bool());

      MOCK_METHOD2(code_complete,
                   data::code_completion(const data::user_input&, bool));
    };
  }
}

#endif
