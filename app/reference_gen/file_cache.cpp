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

#include "file_cache.hpp"

#include <just/file.hpp>

file_cache::entry::entry(std::string content_)
  : _old(std::move(content_)), _updated(_old)
{
}

const std::string& file_cache::entry::content() const { return _updated; }

std::string& file_cache::entry::content() { return _updated; }

void file_cache::entry::write_changes(const boost::filesystem::path& path_)
{
  if (changed())
  {
    just::file::write(path_.string(), _updated);
  }
}

bool file_cache::entry::changed() const { return _old != _updated; }
file_cache::file_cache(const std::vector<cmd_t>& cmds_)
{
  for (const auto& cmd : cmds_)
  {
    if (_files.find(cmd.path()) == _files.end())
    {
      _files.insert({cmd.path(), entry(just::file::read<std::string>(
                                     cmd.path().string()))});
    }
  }
}

const std::string& file_cache::
operator[](const boost::filesystem::path& path_) const
{
  const auto i = _files.find(path_);
  assert(i != _files.end());
  return i->second.content();
}

std::string& file_cache::operator[](const boost::filesystem::path& path_)
{
  const auto i = _files.find(path_);
  assert(i != _files.end());
  return i->second.content();
}

void file_cache::write_changes()
{
  for (auto& p : _files)
  {
    p.second.write_changes(p.first);
  }
}

bool file_cache::changed() const
{
  for (const auto& p : _files)
  {
    if (p.second.changed())
    {
      return true;
    }
  }
  return false;
}
