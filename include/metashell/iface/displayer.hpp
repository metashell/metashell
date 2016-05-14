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

#include <metashell/data/text.hpp>
#include <metashell/data/backtrace.hpp>
#include <metashell/data/frame.hpp>
#include <metashell/data/type.hpp>
#include <metashell/data/file_location.hpp>
#include <metashell/data/type_or_error.hpp>

#include <metashell/iface/call_graph.hpp>

#include <metashell/boost/filesystem/path.hpp>

#include <string>
#include <vector>
#include <set>

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
      virtual void show_type(const data::type& type_) = 0;
      virtual void show_comment(const data::text& msg_) = 0;
      virtual void show_cpp_code(const std::string& code_) = 0;

      virtual void show_frame(const data::frame& frame_) = 0;
      virtual void show_file_section(const data::file_location& location_,
                                     const std::string& env_buffer_) = 0;
      virtual void show_backtrace(const data::backtrace& trace_) = 0;
      virtual void show_call_graph(const iface::call_graph& cg_) = 0;

      virtual void show_filename_list(
          const std::vector<boost::filesystem::path>& filenames_) = 0;

      virtual void show_filename_set(
          const std::set<boost::filesystem::path>& filenames_) = 0;

      void show_type_or_error(const data::type_or_error& te_)
      {
        if (te_.is_type())
        {
          show_type(te_.get_type());
        }
        else if (te_.is_error())
        {
          show_error(te_.get_error());
        }
      }
    };
  }
}

#endif
