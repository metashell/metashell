#ifndef METASHELL_MOCK_FILE_WRITER_HPP
#define METASHELL_MOCK_FILE_WRITER_HPP

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

#include <metashell/iface/file_writer.hpp>

#include <functional>

class mock_file_writer : public metashell::iface::file_writer
{
public:
  mock_file_writer();

  virtual bool open(const std::string& filename_) override;
  virtual void close() override;
  virtual bool is_open() const override;

  virtual bool write(const std::string& content_) override;

  std::function<bool(const std::string&)> open_callback;
  std::function<void()> close_callback;
  std::function<bool()> is_open_callback;
  std::function<bool(const std::string&)> write_callback;
};

#endif
