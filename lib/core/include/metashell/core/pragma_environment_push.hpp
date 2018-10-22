#ifndef METASHELL_PRAGMA_ENVIRONMENT_PUSH_HPP
#define METASHELL_PRAGMA_ENVIRONMENT_PUSH_HPP

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

#include <metashell/core/pragma_without_arguments.hpp>

#include <string>

namespace metashell
{
  namespace core
  {
    class pragma_environment_push : public pragma_without_arguments
    {
    public:
      virtual std::string description() const override;

      virtual void run(iface::shell& shell_,
                       iface::displayer& displayer_) const override;
    };
  }
}

#endif
