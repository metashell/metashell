#ifndef METASHELL_IFACE_COMMAND_PROCESSOR_HPP
#define METASHELL_IFACE_COMMAND_PROCESSOR_HPP

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

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/history.hpp>

#include <metashell/data/code_completion.hpp>
#include <metashell/data/user_input.hpp>

#include <string>

namespace metashell
{
  namespace iface
  {
    class command_processor
    {
    public:
      virtual ~command_processor() = default;

      virtual void line_available(const data::user_input& cmd_,
                                  iface::displayer& displayer_,
                                  iface::history& history_) = 0;
      virtual void cancel_operation() = 0;

      virtual std::string prompt() const = 0;
      virtual bool stopped() const = 0;

      virtual data::code_completion
      code_complete(const data::user_input& s_, bool metashell_extensions_) = 0;
    };
  }
}

#endif
