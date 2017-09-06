#ifndef REFERENCE_GEN_FILE_CACHE_HPP
#define REFERENCE_GEN_FILE_CACHE_HPP

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

#include "cmd_t.hpp"

#include <boost/filesystem.hpp>

#include <map>
#include <string>

class file_cache
{
public:
  explicit file_cache(const std::vector<cmd_t>& cmds_);

  const std::string& operator[](const boost::filesystem::path& path_) const;
  std::string& operator[](const boost::filesystem::path& path_);

  void write_changes();

  bool changed() const;

private:
  class entry
  {
  public:
    explicit entry(std::string content_ = std::string());

    const std::string& content() const;
    std::string& content();

    void write_changes(const boost::filesystem::path& path_);

    bool changed() const;

  private:
    std::string _old;
    std::string _updated;
  };
  std::map<boost::filesystem::path, entry> _files;
};

#endif
