#ifndef METASHELL_DATA_MDB_COMMAND_HPP
#define METASHELL_DATA_MDB_COMMAND_HPP

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

#include <metashell/data/constraint/code.hpp>
#include <metashell/data/constraint/name_first.hpp>
#include <metashell/data/constraint/name_non_first.hpp>

#include <metashell/data/regex.hpp>
#include <metashell/data/string.hpp>
#include <metashell/data/whitespace.hpp>

#include <boost/operators.hpp>

#include <iosfwd>
#include <optional>
#include <string>
#include <tuple>

namespace metashell
{
  namespace data
  {
    class mdb_command : boost::equality_comparable<mdb_command>
    {
    public:
      class name_type : string<name_type,
                               false,
                               constraint::name_non_first,
                               constraint::name_first>
      {
      public:
        using string<name_type,
                     false,
                     constraint::name_non_first,
                     constraint::name_first>::string;

        using string<name_type,
                     false,
                     constraint::name_non_first,
                     constraint::name_first>::value;

        using string<name_type,
                     false,
                     constraint::name_non_first,
                     constraint::name_first>::empty;

        using string<name_type,
                     false,
                     constraint::name_non_first,
                     constraint::name_first>::size;

        using string<name_type,
                     false,
                     constraint::name_non_first,
                     constraint::name_first>::begin;

        using string<name_type,
                     false,
                     constraint::name_non_first,
                     constraint::name_first>::end;

        name_type() = delete;

        static constexpr const char* name_of_type()
        {
          return "name of MDB command";
        }
      };

      class argument_type : boost::equality_comparable<argument_type>
      {
      public:
        argument_type() = default;
        argument_type(std::string, whitespace);

        template <class InputIt>
        argument_type(InputIt begin_, InputIt end_, whitespace suffix_)
          : argument_type(std::string(begin_, end_), std::move(suffix_))
        {
        }

        const std::string& value() const;
        const whitespace& suffix() const;

        explicit operator int() const;

      private:
        std::string _value;
        whitespace _suffix;
      };

      class arguments_type : string<arguments_type, true, constraint::code>
      {
      public:
        class iterator
            : public boost::forward_iterator_helper<iterator,
                                                    const argument_type,
                                                    std::ptrdiff_t,
                                                    const argument_type*,
                                                    const argument_type&>
        {
        public:
          iterator(std::string::const_iterator, std::string::const_iterator);

          iterator& operator++();
          const argument_type& operator*() const;

          bool operator==(const iterator&) const;

        private:
          argument_type _value;
          std::string::const_iterator _at;
          std::string::const_iterator _end;
        };

        using string<arguments_type, true, constraint::code>::string;
        using string<arguments_type, true, constraint::code>::value;
        using string<arguments_type, true, constraint::code>::empty;

        static constexpr const char* name_of_type()
        {
          return "arguments of MDB command";
        }

        iterator begin() const;
        iterator end() const;

        explicit operator int() const;
        explicit operator regex() const;
        explicit operator std::optional<mdb_command>() const;
      };

      using tuple_t =
          std::tuple<whitespace, name_type, whitespace, arguments_type>;

      mdb_command(whitespace, name_type, whitespace, arguments_type);

      const whitespace& prefix() const;
      const name_type& name() const;
      const whitespace& separator() const;
      const arguments_type& arguments() const;

      const tuple_t tuple() const;

      static std::optional<mdb_command> parse(const std::string&);

    private:
      tuple_t _val;
    };

    std::ostream& operator<<(std::ostream&, const mdb_command&);
    std::string to_string(const mdb_command&);

    bool empty(const mdb_command&);

    bool operator==(const mdb_command&, const mdb_command&);

    std::string join(mdb_command::arguments_type::iterator,
                     mdb_command::arguments_type::iterator);

    template <size_t Len>
    bool operator==(const mdb_command::argument_type& lhs_,
                    const char (&rhs_)[Len])
    {
      return lhs_.value() == rhs_;
    }

    template <size_t Len>
    bool operator==(const char (&lhs_)[Len],
                    const mdb_command::argument_type& rhs_)
    {
      return lhs_ == rhs_.value();
    }

    template <size_t Len>
    bool operator!=(const mdb_command::argument_type& lhs_,
                    const char (&rhs_)[Len])
    {
      return lhs_.value() != rhs_;
    }

    template <size_t Len>
    bool operator!=(const char (&lhs_)[Len],
                    const mdb_command::argument_type& rhs_)
    {
      return lhs_ != rhs_.value();
    }

    bool operator==(const mdb_command::argument_type&,
                    const mdb_command::argument_type&);
  }
}

#endif
