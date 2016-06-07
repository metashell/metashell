#ifndef METASHELL_JSON_LINE_READER_HPP
#define METASHELL_JSON_LINE_READER_HPP

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

#include <metashell/command_processor_queue.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/json_writer.hpp>
#include <metashell/line_reader.hpp>

namespace metashell
{
  line_reader
  build_json_line_reader(const line_reader& line_reader_,
                         iface::displayer& displayer_,
                         iface::json_writer& json_writer_,
                         command_processor_queue& command_processor_queue_);
}

#endif
