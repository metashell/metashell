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

#include <metashell/core/json_displayer.hpp>

#include <cassert>
#include <map>

namespace metashell
{
  namespace core
  {
    namespace
    {
      class to_json_visitor
      {
      public:
        explicit to_json_visitor(iface::json_writer& writer_) : _writer(writer_)
        {
        }

        void operator()(const data::type& t_) const { operator()(t_.name()); }

        void operator()(const data::cpp_code& c_) const
        {
          _writer.string(c_.value());
        }

        void operator()(const data::token& t_) const
        {
          _writer.string(format_token(t_));
        }

        void operator()(const boost::filesystem::path& p_) const
        {
          _writer.string(p_.string());
        }

      private:
        iface::json_writer& _writer;
      };

      void
      show_object(iface::json_writer& writer_,
                  std::initializer_list<std::pair<std::string, std::string>> l_)
      {
        writer_.start_object();

        for (const auto& p : l_)
        {
          writer_.key(p.first);
          writer_.string(p.second);
        }

        writer_.end_object();
      }

      void show_frame_fields(iface::json_writer& writer_,
                             const data::frame& frame_)
      {
        writer_.key("name");
        visit(to_json_visitor(writer_), frame_.node());
        writer_.key("source_location");
        writer_.string(to_string(frame_.source_location()));

        if (frame_.is_full())
        {
          writer_.key("kind");
          writer_.string(to_string(frame_.kind()));
          writer_.key("point_of_event");
          writer_.string(to_string(frame_.point_of_event()));
        }
        if (const auto t = frame_.time_taken())
        {
          const auto r = frame_.time_taken_ratio();
          assert(bool(r));

          writer_.key("time_taken");
          writer_.double_(*t);
          writer_.key("time_taken_ratio");
          writer_.double_(*r);
        }
      }

      template <class InputIt>
      void write_filenames(iface::json_writer& writer_,
                           const std::string& type_,
                           InputIt begin_,
                           InputIt end_)
      {
        writer_.start_object();

        writer_.key("type");
        writer_.string(type_);

        writer_.key("filenames");
        writer_.start_array();
        for (InputIt i = begin_; i != end_; ++i)
        {
          writer_.string(i->string());
        }
        writer_.end_array();

        writer_.end_object();
        writer_.end_document();
      }
    }

    json_displayer::json_displayer(iface::json_writer& writer_)
      : _writer(writer_)
    {
    }

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
      show_object(_writer, {{"type", "type"}, {"name", type_.name().value()}});
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

    void json_displayer::show_cpp_code(const data::cpp_code& code_)
    {
      show_object(_writer, {{"type", "cpp_code"}, {"code", code_.value()}});
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

    void
    json_displayer::show_file_section(const data::file_location& /*location_*/,
                                      const std::string& /*env_buffer_*/)
    {
      // We don't show this in json. Whoever uses the json interface can do it
      // himself the way he wants it
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

    void json_displayer::show_filename_list(
        const std::vector<boost::filesystem::path>& filenames_)
    {
      write_filenames(
          _writer, "filename_list", filenames_.begin(), filenames_.end());
    }

    void json_displayer::show_filename_set(
        const std::set<boost::filesystem::path>& filenames_)
    {
      write_filenames(
          _writer, "filename_set", filenames_.begin(), filenames_.end());
    }
  }
}
