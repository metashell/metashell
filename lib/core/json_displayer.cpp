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

#include <metashell/json_displayer.hpp>

#include <map>

using namespace metashell;

namespace
{
  void show_object(
    iface::json_writer& writer_,
    std::initializer_list<std::pair<std::string, std::string>> l_
  )
  {
    writer_.start_object();

    for (const auto& p : l_)
    {
      writer_.key(p.first);
      writer_.string(p.second);
    }

    writer_.end_object();
  }

  void show_frame_fields(iface::json_writer& writer_, const data::frame& frame_)
  {
    writer_.key("name");
    writer_.string(frame_.name().name());

    if (frame_.is_full())
    {
      writer_.key("kind");
      writer_.string(to_string(frame_.kind()));
      writer_.key("point_of_instantiation");
      writer_.string(to_string(frame_.point_of_instantiation()));
    }
  }
}

json_displayer::json_displayer(iface::json_writer& writer_) :
  _writer(writer_)
{}

void json_displayer::show_raw_text(const std::string& text_)
{
  show_object(_writer, {{"type", "raw_text"}, {"value", text_}});
  _writer.end_document();
}

void json_displayer::show_error(const std::string& msg_)
{
  show_object(_writer, {{"type", "error"}, {"msg", msg_}});
  _writer.end_document();
}

void json_displayer::show_type(const data::type& type_)
{
  show_object(_writer, {{"type", "type"}, {"name", type_.name()}});
  _writer.end_document();
}

void json_displayer::show_comment(const data::text& msg_)
{
  _writer.start_object();

  _writer.key("type");
  _writer.string("comment");

  _writer.key("paragraphs");
  _writer.start_array();
  for (const data::paragraph& p : msg_.paragraphs)
  {
    _writer.start_object();

    _writer.key("first_line_indentation");
    _writer.string(p.first_line_indentation);

    _writer.key("rest_of_lines_indentation");
    _writer.string(p.rest_of_lines_indentation);

    _writer.key("content");
    _writer.string(p.content);

    _writer.end_object();
  }
  _writer.end_array();

  _writer.end_object();
  _writer.end_document();
}

void json_displayer::show_cpp_code(const std::string& code_)
{
  show_object(_writer, {{"type", "cpp_code"}, {"code", code_}});
  _writer.end_document();
}

void json_displayer::show_frame(const data::frame& frame_)
{
  _writer.start_object();

  _writer.key("type");
  _writer.string("frame");

  show_frame_fields(_writer, frame_);

  _writer.end_object();
  _writer.end_document();
}

void json_displayer::show_backtrace(const data::backtrace& trace_)
{
  _writer.start_object();

  _writer.key("type");
  _writer.string("backtrace");

  _writer.key("frames");
  _writer.start_array();
  for (const data::frame& f : trace_)
  {
    _writer.start_object();

    show_frame_fields(_writer, f);

    _writer.end_object();
  }
  _writer.end_array();

  _writer.end_object();
  _writer.end_document();
}

void json_displayer::show_call_graph(const iface::call_graph& cg_)
{
  _writer.start_object();

  _writer.key("type");
  _writer.string("call_graph");

  _writer.key("nodes");
  _writer.start_array();
  for (const data::call_graph_node& n : cg_)
  {
    _writer.start_object();

    show_frame_fields(_writer, n.current_frame());

    _writer.key("depth");
    _writer.int_(n.depth());

    _writer.key("children");
    _writer.int_(n.number_of_children());

    _writer.end_object();
  }
  _writer.end_array();

  _writer.end_object();
  _writer.end_document();
}

