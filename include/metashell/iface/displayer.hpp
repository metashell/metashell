#ifndef METASHELL_IFACE_DISPLAYER_HPP
#define METASHELL_IFACE_DISPLAYER_HPP

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

#include <metashell/text.hpp>
#include <metashell/backtrace.hpp>
#include <metashell/frame.hpp>
#include <metashell/type.hpp>

#include <metashell/iface/call_graph.hpp>

#include <string>

namespace metashell
{
  namespace iface
  {
    class displayer
    {
    public:
      virtual ~displayer() {}

      virtual void show_raw_text(const std::string& text_) = 0;
      virtual void show_error(const std::string& msg_) = 0;
      virtual void show_type(const type& type_) = 0;
      virtual void show_comment(const text& msg_) = 0;
      virtual void show_cpp_code(const std::string& code_) = 0;

      virtual void show_frame(const frame& frame_) = 0;
      virtual void show_backtrace(const backtrace& trace_) = 0;
      virtual void show_call_graph(const iface::call_graph& cg_) = 0;
    };
  }
}

#endif

