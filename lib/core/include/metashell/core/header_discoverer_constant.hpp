#ifndef METASHELL_HEADER_DISCOVERER_CONSTANT_HPP
#define METASHELL_HEADER_DISCOVERER_CONSTANT_HPP

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

#include <metashell/data/includes.hpp>
#include <metashell/iface/header_discoverer.hpp>

namespace metashell
{
  namespace core
  {
    class header_discoverer_constant : public iface::header_discoverer
    {
    public:
      header_discoverer_constant(
          std::vector<boost::filesystem::path> sysincludes_,
          std::vector<boost::filesystem::path> quoteincludes_);

      virtual std::vector<boost::filesystem::path>
      include_path(data::include_type type_) override;

      virtual std::set<boost::filesystem::path>
      files_included_by(const data::cpp_code&) override;

    private:
      data::includes _includes;
    };
  }
}

#endif
