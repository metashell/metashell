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

#include <metashell/pragma_handler.hpp>

#include <cassert>

using namespace metashell;

// assumes: h_._body is not null pointer
pragma_handler::pragma_handler(const pragma_handler& h_) :
  _body(h_._body->clone())
{}

pragma_handler& pragma_handler::operator=(pragma_handler h_)
{
  swap(h_);
  return *this;
}

void pragma_handler::swap(pragma_handler& h_)
{
  _body.swap(h_._body);
}

void pragma_handler::run(
  const command::iterator& args_begin_,
  const command::iterator& args_end_
) const
{
  assert(_body);
  _body->run(args_begin_, args_end_);
}

std::string pragma_handler::arguments() const
{
  assert(_body);
  return _body->arguments();
}

std::string pragma_handler::description() const
{
  assert(_body);
  return _body->description();
}

