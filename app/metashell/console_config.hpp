#ifndef METASHELL_CONSOLE_CONFIG_HPP
#define METASHELL_CONSOLE_CONFIG_HPP

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

#include <metashell/data/console_type.hpp>
#include <metashell/data/line_reader.hpp>

#include <metashell/core/command_processor_queue.hpp>

#include <metashell/iface/console.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/history.hpp>
#include <metashell/iface/json_writer.hpp>

#include <memory>

namespace metashell
{
  class console_config
  {
  public:
    console_config(data::console_type type_,
                   bool indent_,
                   bool syntax_highlight_);

    iface::displayer& displayer();
    iface::history& history();
    data::line_reader& reader();
    core::command_processor_queue& processor_queue();

  private:
    core::command_processor_queue _processor_queue;
    std::unique_ptr<iface::console> _console;
    std::unique_ptr<iface::json_writer> _json_writer;
    std::unique_ptr<iface::displayer> _displayer;
    std::unique_ptr<iface::history> _history;
    data::line_reader _reader;
  };
} // namespace metashell

#endif
