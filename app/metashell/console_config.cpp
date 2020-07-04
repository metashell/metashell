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

#include "console_config.hpp"

#include <metashell/core/console_displayer.hpp>
#include <metashell/core/json_displayer.hpp>
#include <metashell/core/json_line_reader.hpp>
#include <metashell/core/null_history.hpp>
#include <metashell/core/rapid_json_writer.hpp>
#include <metashell/core/stdout_console.hpp>
#include <metashell/core/stream_console.hpp>

#include <metashell/readline/history.hpp>
#include <metashell/readline/line_reader.hpp>

#include <iostream>

using namespace metashell;

namespace
{
  std::optional<data::user_input> plain_line_reader(const std::string& prompt_)
  {
    if (!prompt_.empty())
    {
      std::cout << prompt_ << ' ';
    }
    std::cin >> std::noskipws;
    std::string cmd;
    if (std::getline(std::cin, cmd))
    {
      return data::user_input(cmd);
    }
    else
    {
      return std::nullopt;
    }
  }

  std::unique_ptr<iface::console> create_console(data::console_type type_)
  {
    switch (type_)
    {
    case data::console_type::plain:
      return std::make_unique<core::stream_console>(std::cout);
    case data::console_type::readline:
      return std::make_unique<core::stdout_console>();
    case data::console_type::json:
      return nullptr;
    }
    return nullptr;
  }

  std::unique_ptr<iface::json_writer>
  create_json_writer(data::console_type type_)
  {
    switch (type_)
    {
    case data::console_type::plain:
      return nullptr;
    case data::console_type::readline:
      return nullptr;
    case data::console_type::json:
      return std::make_unique<core::rapid_json_writer>(std::cout);
    }
    return nullptr;
  }

  std::unique_ptr<iface::displayer>
  create_displayer(data::console_type type_,
                   bool indent_,
                   bool syntax_highlight_,
                   iface::console* console_,
                   iface::json_writer* json_writer_)
  {
    switch (type_)
    {
    case data::console_type::plain:
      return std::make_unique<core::console_displayer>(*console_, false, false);
    case data::console_type::readline:
      return std::make_unique<core::console_displayer>(
          *console_, indent_, syntax_highlight_);
    case data::console_type::json:
      return std::make_unique<core::json_displayer>(*json_writer_);
    }
    return nullptr;
  }

  std::unique_ptr<iface::history> create_history(data::console_type type_)
  {
    switch (type_)
    {
    case data::console_type::plain:
      return std::make_unique<core::null_history>();
    case data::console_type::readline:
      return std::make_unique<readline::history>();
    case data::console_type::json:
      return std::make_unique<core::null_history>();
    }
    return nullptr;
  }

  data::line_reader
  create_reader(data::console_type type_,
                iface::displayer* displayer_,
                iface::json_writer* json_writer_,
                core::command_processor_queue& processor_queue_)
  {
    switch (type_)
    {
    case data::console_type::plain:
      return plain_line_reader;
    case data::console_type::readline:
      return metashell::readline::line_reader(
          [&processor_queue_](const data::user_input& s_) {
            return processor_queue_.code_complete(s_);
          });
    case data::console_type::json:
      return build_json_line_reader(
          plain_line_reader, *displayer_, *json_writer_, processor_queue_);
    }
    // To avoid warnings
    return plain_line_reader;
  }
}

console_config::console_config(data::console_type type_,
                               bool indent_,
                               bool syntax_highlight_)
  : _console(create_console(type_)),
    _json_writer(create_json_writer(type_)),
    _displayer(create_displayer(
        type_, indent_, syntax_highlight_, _console.get(), _json_writer.get())),
    _history(create_history(type_)),
    _reader(create_reader(
        type_, _displayer.get(), _json_writer.get(), _processor_queue))
{
  _processor_queue.history(*_history);
}

iface::displayer& console_config::displayer() { return *_displayer; }

iface::history& console_config::history() { return *_history; }

data::line_reader& console_config::reader() { return _reader; }

core::command_processor_queue& console_config::processor_queue()
{
  return _processor_queue;
}
