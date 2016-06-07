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

#include "mock_command_processor.hpp"

using namespace metashell;

mock_command_processor::mock_command_processor()
  : code_complete_callback([](const std::string&, std::set<std::string>&) {})
{
}

void mock_command_processor::line_available(const std::string&,
                                            iface::displayer&,
                                            iface::history&)
{
  // ignore
}

void mock_command_processor::cancel_operation()
{
  // ignore
}

std::string mock_command_processor::prompt() const { return ">"; }

bool mock_command_processor::stopped() const { return false; }

void mock_command_processor::code_complete(const std::string& s_,
                                           std::set<std::string>& out_) const
{
  code_complete_callback(s_, out_);
}
