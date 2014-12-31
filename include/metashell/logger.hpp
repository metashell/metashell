#ifndef METASHELL_LOGGER_HPP
#define METASHELL_LOGGER_HPP

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

#include <metashell/logging_mode.hpp>
#include <metashell/iface/displayer.hpp>
#include <metashell/iface/file_writer.hpp>

#include <string>

namespace metashell
{
  class logger
  {
  public:
    logger(iface::displayer& displayer_, iface::file_writer& fwriter_);

    bool logging() const;
    logging_mode mode() const;

    void log_into_file(const std::string& filename_);
    void log_to_console();
    void stop_logging();

    void log(const std::string& msg_);
  private:
    logging_mode _mode;
    iface::file_writer& _fwriter;
    iface::displayer& _displayer;
  };
}

#ifdef METASHELL_LOG
#  error METASHELL_LOG already defined
#endif
#define METASHELL_LOG(logger_ptr, msg) \
  { \
    ::metashell::logger* l = (logger_ptr); \
    if (l != nullptr && l->logging()) \
    { \
      l->log(msg); \
    } \
  }

#endif

