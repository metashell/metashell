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
#include <metashell/iface/pragma_handler.hpp>
#include <metashell/metashell_pragma.hpp>
#include <metashell/data/command.hpp>

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
  const data::command::iterator& name_begin_,
  const data::command::iterator& name_end_,
  const data::command::iterator& args_begin_,
  const data::command::iterator& args_end_,
  iface::displayer& displayer_
) const
{
  assert(_body);
  _body->run(name_begin_, name_end_, args_begin_, args_end_, displayer_);
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

void metashell::run(
  const iface::pragma_handler& handler_,
  const std::string& args_,
  iface::displayer& displayer_
)
{
  const data::command cmd(args_);
  handler_.run(
    cmd.begin(),
    cmd.begin(),
    cmd.begin(),
    end_of_pragma_argument_list(cmd.begin(), cmd.end()),
    displayer_
  );
}

