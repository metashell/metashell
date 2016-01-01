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

#include <metashell/logger.hpp>

using namespace metashell;

logger::logger(iface::displayer& displayer_, iface::file_writer& fwriter_)
  : _mode(data::logging_mode::none), _fwriter(fwriter_), _displayer(displayer_)
{
}

bool logger::logging() const { return _mode != data::logging_mode::none; }

data::logging_mode logger::mode() const { return _mode; }

void logger::log_into_file(const std::string& filename_)
{
  stop_logging();

  if (_fwriter.open(filename_))
  {
    _mode = data::logging_mode::file;
  }
}

void logger::log_to_console()
{
  stop_logging();

  _mode = data::logging_mode::console;
}

void logger::stop_logging()
{
  if (_fwriter.is_open())
  {
    _fwriter.close();
  }
  _mode = data::logging_mode::none;
}

void logger::log(const std::string& msg_)
{
  switch (_mode)
  {
  case data::logging_mode::none:
    // do nothing
    break;
  case data::logging_mode::file:
    _fwriter.write(msg_ + "\n");
    break;
  case data::logging_mode::console:
    _displayer.show_comment(data::text("Log: " + msg_));
    break;
  }
}
