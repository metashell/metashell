#ifndef METASHELL_PRAGMA_HANDLER_HPP
#define METASHELL_PRAGMA_HANDLER_HPP

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

#include <metashell/data/command.hpp>
#include <metashell/data/cpp_code.hpp>

#include <metashell/iface/pragma_handler.hpp>

#include <metashell/core/make_unique.hpp>

#include <memory>

namespace metashell
{
  namespace core
  {
    class pragma_handler
    {
    public:
      template <class Impl>
      pragma_handler(Impl impl_) : _body(core::make_unique<Impl>(impl_))
      {
        static_assert(std::is_base_of<iface::pragma_handler, Impl>::value,
                      "Invalid impl");
      }

      pragma_handler(const pragma_handler& h_);
      pragma_handler& operator=(pragma_handler h_);

      void swap(pragma_handler& h_);

      std::string arguments() const;
      std::string description() const;

      void run(const data::command::iterator& name_begin_,
               const data::command::iterator& name_end_,
               const data::command::iterator& args_begin_,
               const data::command::iterator& args_end_,
               iface::displayer& displayer_) const;

    private:
      std::unique_ptr<iface::pragma_handler> _body;
    };

    void run(const iface::pragma_handler& handler_,
             const data::cpp_code& args_,
             iface::displayer& displayer_);
  }
}

#endif
