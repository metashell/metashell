#ifndef METASHELL_ENGINE_WAVE_EMPTY_ENVIRONMENT_HPP
#define METASHELL_ENGINE_WAVE_EMPTY_ENVIRONMENT_HPP

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

#include <metashell/iface/environment.hpp>

namespace metashell
{
  namespace engine
  {
    namespace wave
    {
      class empty_environment : public iface::environment
      {
      public:
        explicit empty_environment(
            const boost::filesystem::path& internal_dir_);

        virtual void append(const data::cpp_code& s_) override;
        virtual data::cpp_code get() const override;
        virtual data::cpp_code
        get_appended(const data::cpp_code& s_) const override;

        virtual const data::headers& get_headers() const override;

        virtual data::cpp_code get_all() const override;

      private:
        data::headers _headers;
      };
    }
  }
}

#endif
