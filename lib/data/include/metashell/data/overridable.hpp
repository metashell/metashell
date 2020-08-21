#ifndef METASHELL_DATA_OVERRIDABLEHPP
#define METASHELL_DATA_OVERRIDABLEHPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#include <optional>

namespace metashell
{
  namespace data
  {
    template <class T>
    class overridable
    {
    public:
      overridable() = default;

      overridable(const T& value_) : _default(value_) {}

      overridable(T&& value_) : _default(std::move(value_)) {}

      const T& operator*() const { return _override ? *_override : _default; }
      const T* operator->() const { return &this->operator*(); }

      const T& default_value() const { return _default; }
      T& default_value() { return _default; }

      void set_default(T value_) { _default = std::move(value_); }

      void set_override(T value_) { _override = std::move(value_); }

      void set_override() { _override = std::nullopt; }

    private:
      T _default;
      std::optional<T> _override;
    };
  }
}

#endif
