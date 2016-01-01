#ifndef METASHELL_MOCK_JSON_WRITER_HPP
#define METASHELL_MOCK_JSON_WRITER_HPP

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

#include <metashell/iface/json_writer.hpp>

#include <vector>
#include <string>

class mock_json_writer : public metashell::iface::json_writer
{
public:
  virtual void string(const std::string& value_) override;
  virtual void int_(int value_) override;
  virtual void double_(double value) override;

  virtual void start_object() override;
  virtual void key(const std::string& key_) override;
  virtual void end_object() override;

  virtual void start_array() override;
  virtual void end_array() override;

  virtual void end_document() override;

  const std::vector<std::string>& calls() const;

private:
  std::vector<std::string> _calls;
};

#endif
