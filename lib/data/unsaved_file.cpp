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

#include <metashell/data/unsaved_file.hpp>

using namespace metashell::data;

unsaved_file::unsaved_file(
  const std::string& filename_,
  const std::string& content_
) :
  _filename(filename_),
  _content(content_)
{}

const std::string& unsaved_file::filename() const
{
  return _filename;
}

const std::string& unsaved_file::content() const
{
  return _content;
}

