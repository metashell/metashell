#ifndef METASHELL_TEMPLATE_TRACER_CONSTANT_HPP
#define METASHELL_TEMPLATE_TRACER_CONSTANT_HPP

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

#include <metashell/iface/template_tracer.hpp>

namespace metashell
{
  class template_tracer_constant : public iface::template_tracer
  {
  public:
    explicit template_tracer_constant(data::result result_);

    virtual data::result eval(const iface::environment&,
                              const boost::optional<std::string>&,
                              bool,
                              const boost::filesystem::path&) override;

  private:
    data::result _result;
  };
}

#endif
