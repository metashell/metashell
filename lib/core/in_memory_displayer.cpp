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

#include <metashell/core/in_memory_displayer.hpp>

using namespace metashell;

void in_memory_displayer::show_raw_text(const std::string& text_)
{
  _raw_texts.push_back(text_);
}

void in_memory_displayer::show_error(const std::string& msg_)
{
  _errors.push_back(msg_);
}

void in_memory_displayer::show_type(const data::type& type_)
{
  _types.push_back(type_);
}

void in_memory_displayer::show_comment(const data::text& msg_)
{
  _comments.push_back(msg_);
}

void in_memory_displayer::show_cpp_code(const data::cpp_code& code_)
{
  _cpp_codes.push_back(code_);
}

void in_memory_displayer::show_frame(const data::frame& frame_)
{
  _frames.push_back(frame_);
}

void in_memory_displayer::show_file_section(
    const data::file_location& location_, const std::string& /*env_buffer_*/)
{
  _file_locations.push_back(location_);
}

void in_memory_displayer::show_backtrace(const data::backtrace& trace_)
{
  _backtraces.push_back(trace_);
}

void in_memory_displayer::show_call_graph(const iface::call_graph& cg_)
{
  _call_graphs.push_back(call_graph(cg_.begin(), cg_.end()));
}

void in_memory_displayer::show_filename_list(
    const std::vector<boost::filesystem::path>& filenames_)
{
  _filename_lists.push_back(filenames_);
}

void in_memory_displayer::show_filename_set(
    const std::set<boost::filesystem::path>& filenames_)
{
  _filename_sets.push_back(filenames_);
}

const std::vector<std::string>& in_memory_displayer::errors() const
{
  return _errors;
}

const std::vector<std::string>& in_memory_displayer::raw_texts() const
{
  return _raw_texts;
}

const std::vector<data::type>& in_memory_displayer::types() const
{
  return _types;
}

const std::vector<data::text>& in_memory_displayer::comments() const
{
  return _comments;
}

const std::vector<data::cpp_code>& in_memory_displayer::cpp_codes() const
{
  return _cpp_codes;
}

const std::vector<data::frame>& in_memory_displayer::frames() const
{
  return _frames;
}

const std::vector<data::file_location>&
in_memory_displayer::file_locations() const
{
  return _file_locations;
}

const std::vector<data::backtrace>& in_memory_displayer::backtraces() const
{
  return _backtraces;
}

const std::vector<in_memory_displayer::call_graph>&
in_memory_displayer::call_graphs() const
{
  return _call_graphs;
}

const std::vector<std::vector<boost::filesystem::path>>&
in_memory_displayer::filename_lists() const
{
  return _filename_lists;
}

const std::vector<std::set<boost::filesystem::path>>&
in_memory_displayer::filename_sets() const
{
  return _filename_sets;
}

void in_memory_displayer::clear()
{
  _errors.clear();
  _raw_texts.clear();
  _types.clear();
  _comments.clear();
  _cpp_codes.clear();
  _frames.clear();
  _file_locations.clear();
  _backtraces.clear();
  _call_graphs.clear();
  _filename_lists.clear();
  _filename_sets.clear();
}

bool in_memory_displayer::empty() const
{
  return _errors.empty() && _raw_texts.empty() && _types.empty() &&
         _comments.empty() && _cpp_codes.empty() && _frames.empty() &&
         _backtraces.empty() && _call_graphs.empty() &&
         _filename_lists.empty() && _filename_sets.empty();
}
