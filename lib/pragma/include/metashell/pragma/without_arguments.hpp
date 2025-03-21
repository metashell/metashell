#ifndef METASHELL_PRAGMA_WITHOUT_ARGUMENTS_HPP
#define METASHELL_PRAGMA_WITHOUT_ARGUMENTS_HPP

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

#include <metashell/iface/displayer.hpp>
#include <metashell/iface/pragma_handler.hpp>

namespace metashell
{
  namespace pragma
  {
    class without_arguments : public iface::pragma_handler
    {
    public:
      std::string arguments() const override;

      void run(const data::command::iterator& name_begin_,
               const data::command::iterator& name_end_,
               const data::command::iterator& args_begin_,
               const data::command::iterator& args_end_,
               iface::main_shell& shell_,
               iface::displayer& displayer_) const override;

      virtual void run(iface::main_shell&, iface::displayer&) const = 0;

      data::code_completion code_complete(data::command::const_iterator,
                                          data::command::const_iterator,
                                          iface::main_shell&) const override;
    };
  } // namespace pragma
} // namespace metashell

#endif
