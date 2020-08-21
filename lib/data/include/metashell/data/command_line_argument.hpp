#ifndef METASHELL_DATA_COMMAND_LINE_ARGUMENT_HPP
#define METASHELL_DATA_COMMAND_LINE_ARGUMENT_HPP

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

#include <metashell/data/string.hpp>

#include <boost/filesystem/path.hpp>

#include <optional>

namespace metashell
{
  namespace data
  {
    class command_line_argument : string<command_line_argument>
    {
    public:
      using string<command_line_argument>::value;
      using string<command_line_argument>::c_str;
      using string<command_line_argument>::begin;
      using string<command_line_argument>::end;
      using string<command_line_argument>::empty;
      using string<command_line_argument>::find;
      using string<command_line_argument>::substr;
      using string<command_line_argument>::size;

      explicit command_line_argument(const boost::filesystem::path&);
      explicit command_line_argument(const char*);
      command_line_argument() = default;

      std::optional<command_line_argument>
      remove_prefix(const command_line_argument&) const;

      template <size_t Len>
      std::optional<command_line_argument>
      remove_prefix(const char (&prefix_)[Len]) const
      {
        return remove_prefix(command_line_argument(prefix_));
      }

      template <size_t Len>
      bool starts_with(const char (&prefix_)[Len]) const
      {
        return starts_with_impl(prefix_);
      }

      template <size_t Len>
      bool contains(const char (&substr_)[Len]) const
      {
        return contains_impl(substr_);
      }

      std::optional<int> as_int() const;
      int as_int(const std::string& error_) const;

      static constexpr const char* name_of_type()
      {
        return "Command line argument";
      }

    private:
      bool starts_with_impl(const char*) const;
      bool contains_impl(const char*) const;
    };

    std::string quote(const command_line_argument&);
  }
}

#endif
