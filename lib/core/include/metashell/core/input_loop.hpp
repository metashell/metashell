#ifndef METASHELL_CORE_INPUT_LOOP_HPP
#define METASHELL_CORE_INPUT_LOOP_HPP

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

#include <metashell/data/line_reader.hpp>

#include <metashell/core/command_processor_queue.hpp>

#include <metashell/iface/displayer.hpp>

namespace metashell
{
  namespace core
  {
    void input_loop(command_processor_queue& processor_queue_,
                    iface::displayer& displayer_,
                    const data::line_reader& line_reader_);
  }
} // namespace metashell

#endif
