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

#include <metashell/core/json_line_reader.hpp>
#include <metashell/core/rapid_object_handler.hpp>

#include <rapidjson/reader.h>

#include <cassert>
#include <map>

namespace metashell
{
  namespace core
  {
    namespace
    {
      void show_code_complete_result(iface::json_writer& writer_,
                                     data::code_completion&& completions_)
      {
        writer_.start_object();

        writer_.key("type");
        writer_.string("code_completion_result");

        writer_.key("completions");
        writer_.start_array();
        while (const std::optional<data::user_input> c = completions_.pop())
        {
          writer_.string(c->value());
        }
        writer_.end_array();

        writer_.end_object();
        writer_.end_document();
      }

      void show_prompt(iface::json_writer& writer_, const std::string& prompt_)
      {
        writer_.start_object();

        writer_.key("type");
        writer_.string("prompt");

        writer_.key("prompt");
        writer_.string(prompt_);

        writer_.end_object();
        writer_.end_document();
      }

      std::optional<data::user_input>
      read_next_line(const data::line_reader& line_reader_,
                     iface::json_writer& json_writer_,
                     const std::string& prompt_)
      {
        show_prompt(json_writer_, prompt_);
        return line_reader_("");
      }

      std::optional<data::user_input>
      json_line_reader(const data::line_reader& line_reader_,
                       iface::displayer& displayer_,
                       iface::json_writer& json_writer_,
                       command_processor_queue& command_processor_queue_,
                       const std::string& prompt_)
      {
        while (const auto s =
                   read_next_line(line_reader_, json_writer_, prompt_))
        {
          rapid_object_handler handler(displayer_);
          rapidjson::Reader reader;
          rapidjson::StringStream string_stream(s->c_str());
          reader.Parse(string_stream, handler);
          if (!handler.failed())
          {
            if (handler.empty())
            {
              return data::user_input();
            }
            else if (const auto type = handler.field("type"))
            {
              if (*type == "cmd")
              {
                if (const auto cmd = handler.field("cmd"))
                {
                  return data::user_input(*cmd);
                }
                else
                {
                  displayer_.show_error(
                      "The cmd field of the cmd command is missing");
                }
              }
              else if (*type == "code_completion")
              {
                if (const auto code = handler.field("code"))
                {
                  show_code_complete_result(
                      json_writer_, command_processor_queue_.code_complete(
                                        data::user_input{*code}));
                }
                else
                {
                  displayer_.show_error(
                      "The code field of the code_completion command is "
                      "missing");
                }
              }
              else
              {
                displayer_.show_error("Unknown command type: " + *type);
              }
            }
            else
            {
              displayer_.show_error("Command without a type: " + s->value());
            }
          }
        }
        return std::nullopt;
      }
    }

    data::line_reader
    build_json_line_reader(const data::line_reader& line_reader_,
                           iface::displayer& displayer_,
                           iface::json_writer& json_writer_,
                           command_processor_queue& command_processor_queue_)
    {
      return [line_reader_, &displayer_, &json_writer_,
              &command_processor_queue_](const std::string& prompt_) {
        return json_line_reader(line_reader_, displayer_, json_writer_,
                                command_processor_queue_, prompt_);
      };
    }
  }
}
