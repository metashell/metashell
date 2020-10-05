// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/core/comment_json_writer.hpp>

namespace metashell
{
  namespace core
  {
    comment_json_writer::comment_json_writer(iface::displayer& displayer_)
      : _displayer(displayer_), _buff(), _writer(_buff)
    {
    }

    comment_json_writer::~comment_json_writer()
    {
      _displayer.show_comment(data::text(_buff.str()));
    }

    void comment_json_writer::string(const std::string& value_)
    {
      _writer.string(value_);
    }

    void comment_json_writer::int_(int value_) { _writer.int_(value_); }

    void comment_json_writer::double_(double value_)
    {
      _writer.double_(value_);
    }

    void comment_json_writer::bool_(bool value_) { _writer.bool_(value_); }

    void comment_json_writer::start_object() { _writer.start_object(); }

    void comment_json_writer::key(const std::string& key_)
    {
      _writer.key(key_);
    }

    void comment_json_writer::end_object() { _writer.end_object(); }

    void comment_json_writer::start_array() { _writer.start_array(); }

    void comment_json_writer::end_array() { _writer.end_array(); }

    void comment_json_writer::end_document() { _writer.end_document(); }
  } // namespace core
} // namespace metashell
