#ifndef METASHELL_ENGINE_CONSTANT_TYPE_SHELL_HPP
#define METASHELL_ENGINE_CONSTANT_TYPE_SHELL_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/iface/type_shell.hpp>

namespace metashell
{
  namespace engine
  {
    namespace constant
    {
      class type_shell : public iface::type_shell
      {
      public:
        explicit type_shell(data::result result_);

        virtual data::result eval(const iface::environment&,
                                  const std::optional<data::cpp_code>&,
                                  bool) override;

        virtual void
        generate_precompiled_header(const boost::filesystem::path&) override;

      private:
        data::result _result;
      };
    }
  }
}

#endif
