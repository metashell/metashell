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

#include <metashell/console_config.hpp>

#include <metashell/stdout_console.hpp>
#include <metashell/stream_console.hpp>
#include <metashell/json_displayer.hpp>
#include <metashell/console_displayer.hpp>
#include <metashell/readline_history.hpp>
#include <metashell/null_history.hpp>
#include <metashell/readline_line_reader.hpp>
#include <metashell/json_line_reader.hpp>
#include <metashell/rapid_json_writer.hpp>

#include <iostream>

using namespace metashell;

namespace
{
  boost::optional<std::string> plain_line_reader(const std::string& prompt_)
  {
    if (!prompt_.empty())
    {
      std::cout << prompt_ << ' ';
    }
    std::cin >> std::noskipws;
    std::string cmd;
    if (std::getline(std::cin, cmd))
    {
      return cmd;
    }
    else
    {
      return boost::none;
    }
  }
}

console_config::console_config(
  console_type type_,
  bool indent_,
  bool syntax_highlight_
)
{
  switch (type_)
  {
  case console_type::plain:
    _console.reset(new stream_console(std::cout));
    _displayer.reset(new console_displayer(*_console, false, false));
    _history.reset(new null_history);
    _reader = plain_line_reader;
    break;
  case console_type::readline:
    _console.reset(new stdout_console());
    _displayer.reset(
      new console_displayer(*_console, indent_, syntax_highlight_)
    );
    _history.reset(new readline_history);
    _reader = metashell::readline_line_reader(_processor_queue);
    break;
  case console_type::json:
    _json_writer.reset(new rapid_json_writer(std::cout));
    _displayer.reset(new json_displayer(*_json_writer));
    _history.reset(new null_history);
    _reader =
      build_json_line_reader(
        plain_line_reader,
        *_displayer,
        *_json_writer,
        _processor_queue
      );
    break;
  }

  _processor_queue.history(*_history);
}

iface::displayer& console_config::displayer()
{
  return *_displayer;
}

iface::history& console_config::history()
{
  return *_history;
}

line_reader& console_config::reader()
{
  return _reader;
}

command_processor_queue& console_config::processor_queue()
{
  return _processor_queue;
}

