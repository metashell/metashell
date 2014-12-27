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

#include <functional>

class mock_command_processor : public metashell::iface::command_processor
{
public:
  mock_command_processor();

  virtual void line_available(
    const std::string& cmd_,
    metashell::iface::displayer& displayer_,
    metashell::iface::history& history_
  ) override;
  virtual void cancel_operation() override;

  virtual std::string prompt() const override;
  virtual bool stopped() const override;

  virtual void code_complete(
    const std::string& s_,
    std::set<std::string>& out_
  ) const override;

  std::function<void(const std::string&, std::set<std::string>&)>
    code_complete_callback;
};

#endif

