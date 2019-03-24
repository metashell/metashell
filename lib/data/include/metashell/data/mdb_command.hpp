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

#include <metashell/data/regex.hpp>
#include <metashell/data/whitespace.hpp>

#include <boost/operators.hpp>
#include <boost/optional.hpp>

#include <iosfwd>
#include <string>
#include <tuple>

namespace metashell
{
  namespace data
  {
    class mdb_command : boost::equality_comparable<mdb_command>
    {
    public:
      class name_type : boost::totally_ordered<name_type>
      {
      public:
        using iterator = std::string::const_iterator;
        using const_iterator = std::string::const_iterator;

        name_type() = default;
        explicit name_type(std::string);

        template <class InputIt>
        name_type(InputIt begin_, InputIt end_)
          : name_type(std::string(begin_, end_))
        {
        }

        const std::string& value() const;

        bool empty() const;

        const_iterator begin() const;
        const_iterator end() const;

      private:
        std::string _value;
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

      class arguments_type : boost::equality_comparable<arguments_type>
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

        arguments_type() = default;
        explicit arguments_type(std::string);

        template <class InputIt>
        arguments_type(InputIt begin_, InputIt end_)
          : arguments_type(std::string(begin_, end_))
        {
        }

        const std::string& value() const;

        bool empty() const;

        iterator begin() const;
        iterator end() const;

        explicit operator int() const;
        explicit operator regex() const;
        explicit operator mdb_command() const;

      private:
        std::string _value;
      };

      using tuple_t =
          std::tuple<whitespace, name_type, whitespace, arguments_type>;

      mdb_command() = default;
      explicit mdb_command(const std::string&);

      bool empty() const;
      bool only_whitespace() const;

      const whitespace& prefix() const;
      const name_type& name() const;
      const whitespace& separator() const;
      const arguments_type& arguments() const;

      const tuple_t tuple() const;

    private:
      tuple_t _val;
    };

    std::ostream& operator<<(std::ostream&, const mdb_command&);
    std::string to_string(const mdb_command&);

    bool operator==(const mdb_command&, const mdb_command&);

    std::ostream& operator<<(std::ostream&, const mdb_command::name_type&);
    std::string to_string(const mdb_command::name_type&);

    bool operator==(const mdb_command::name_type&,
                    const mdb_command::name_type&);
    bool operator<(const mdb_command::name_type&,
                   const mdb_command::name_type&);

    std::ostream& operator<<(std::ostream&, const mdb_command::arguments_type&);
    std::string to_string(const mdb_command::arguments_type&);

    bool operator==(const mdb_command::arguments_type&,
                    const mdb_command::arguments_type&);

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
