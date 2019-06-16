#ifndef METASHELL_DATA_STRING_HPP
#define METASHELL_DATA_STRING_HPP

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

#include <metashell/data/constraint/any.hpp>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace metashell
{
  namespace data
  {
    namespace impl
    {
      template <class Char>
      std::basic_string<Char> c_escape(const std::basic_string<Char>& s_)
      {
        std::basic_ostringstream<Char> result;
        for (Char c : s_)
        {
          if (c == '\a')
          {
            result << '\\' << 'a';
          }
          else if (c == '\b')
          {
            result << '\\' << 'b';
          }
          else if (c == '\f')
          {
            result << '\\' << 'b';
          }
          else if (c == '\n')
          {
            result << '\\' << 'n';
          }
          else if (c == '\r')
          {
            result << '\\' << 'r';
          }
          else if (c == '\t')
          {
            result << '\\' << 't';
          }
          else if (c == '\v')
          {
            result << '\\' << 'v';
          }
          else if (c < Char(32) || Char(126) < c)
          {
            result << '\\' << 'x' << std::hex << int(c);
          }
          else
          {
            if (c == '\\' || c == '"' || c == '\'')
            {
              result << '\\';
            }
            result << c;
          }
        }
        return result.str();
      }

      inline std::string to_string(const std::string& s_) { return s_; }

      template <class CharT, class Traits, class Allocator>
      auto c_str(const std::basic_string<CharT, Traits, Allocator>& s_)
      {
        return s_.c_str();
      }

      template <class CharT, class Traits, class Allocator>
      auto empty(const std::basic_string<CharT, Traits, Allocator>& s_)
      {
        return s_.empty();
      }

      template <class CharT, class Traits, class Allocator>
      void clear(std::basic_string<CharT, Traits, Allocator>& s_)
      {
        s_.clear();
      }

      template <class CharT, class Traits, class Allocator>
      auto size(const std::basic_string<CharT, Traits, Allocator>& s_)
      {
        return s_.size();
      }

      template <class CharT, class Traits, class Allocator>
      auto substr(
          const std::basic_string<CharT, Traits, Allocator>& s_,
          typename std::basic_string<CharT, Traits, Allocator>::size_type pos_ =
              0,
          typename std::basic_string<CharT, Traits, Allocator>::size_type len_ =
              std::basic_string<CharT, Traits, Allocator>::npos)
      {
        return s_.substr(pos_, len_);
      }

      template <class Derived,
                bool AllowEmpty,
                class NonFirstCharConstraints,
                class FirstCharConstraints,
                class String>
      class string_base
      {
      public:
        explicit string_base(String value_) : _value(std::move(value_))
        {
          check_invariant(_value);
        }

        static void check_invariant(const String& s_)
        {
          if (!AllowEmpty && empty(s_))
          {
            throw std::runtime_error(
                "Empty " + std::string(Derived::name_of_type()) + " value");
          }

          bool first = true;
          for (auto c : s_)
          {
            if ((first && !FirstCharConstraints::allowed_char(c)) ||
                (!first && !NonFirstCharConstraints::allowed_char(c)))
            {
              throw std::runtime_error(
                  "Invalid character in " +
                  std::string(Derived::name_of_type()) + ": " +
                  c_escape(std::basic_string<decltype(c)>(1, c)));
            }
          }
        }

        String _value;
      };

      template <class Derived,
                bool AllowEmpty,
                class NonFirstCharConstraints,
                class FirstCharConstraints,
                class String>
      class string : public string_base<Derived,
                                        AllowEmpty,
                                        NonFirstCharConstraints,
                                        FirstCharConstraints,
                                        String>
      {
      public:
        string()
          : string_base<Derived,
                        AllowEmpty,
                        NonFirstCharConstraints,
                        FirstCharConstraints,
                        String>(String())
        {
          static_assert(AllowEmpty, "Empty values are disabled.");
        }

        explicit string(String value_)
          : string_base<Derived,
                        AllowEmpty,
                        NonFirstCharConstraints,
                        FirstCharConstraints,
                        String>(std::move(value_))
        {
        }

        explicit string(std::string value_)
          : string_base<Derived,
                        AllowEmpty,
                        NonFirstCharConstraints,
                        FirstCharConstraints,
                        String>(String(std::move(value_)))
        {
        }

        template <class InputIterator>
        string(InputIterator begin_, InputIterator end_)
          : string_base<Derived,
                        AllowEmpty,
                        NonFirstCharConstraints,
                        FirstCharConstraints,
                        String>(String(begin_, end_))
        {
        }
      };

      template <class Derived,
                bool AllowEmpty,
                class NonFirstCharConstraints,
                class FirstCharConstraints>
      class string<Derived,
                   AllowEmpty,
                   NonFirstCharConstraints,
                   FirstCharConstraints,
                   std::string> : public string_base<Derived,
                                                     AllowEmpty,
                                                     NonFirstCharConstraints,
                                                     FirstCharConstraints,
                                                     std::string>
      {
      public:
        explicit string(std::string value_ = std::string())
          : string_base<Derived,
                        AllowEmpty,
                        NonFirstCharConstraints,
                        FirstCharConstraints,
                        std::string>(std::move(value_))
        {
        }

        template <class InputIterator>
        string(InputIterator begin_, InputIterator end_)
          : string_base<Derived,
                        AllowEmpty,
                        NonFirstCharConstraints,
                        FirstCharConstraints,
                        std::string>(std::string(begin_, end_))
        {
        }
      };
    }

    template <class Derived,
              bool AllowEmpty = true,
              class NonFirstCharConstraints = constraint::any,
              class FirstCharConstraints = NonFirstCharConstraints,
              class String = std::string>
    class string : impl::string<Derived,
                                AllowEmpty,
                                NonFirstCharConstraints,
                                FirstCharConstraints,
                                String>
    {
    public:
      using impl::string<Derived,
                         AllowEmpty,
                         NonFirstCharConstraints,
                         FirstCharConstraints,
                         String>::string;

      string()
        : impl::string<Derived,
                       AllowEmpty,
                       NonFirstCharConstraints,
                       FirstCharConstraints,
                       String>()
      {
      }

      const String& value() const
      {
        return impl::string<Derived, AllowEmpty, NonFirstCharConstraints,
                            FirstCharConstraints, String>::_value;
      }

      friend auto c_str(const Derived& s_)
      {
        using impl::c_str;
        return c_str(s_.value());
      }

      friend auto empty(const Derived& s_)
      {
        using impl::empty;
        return empty(s_.value());
      }

      friend void clear(Derived& s_)
      {
        using impl::clear;
        clear(const_cast<String&>(s_.value()));
      }

      friend auto size(const Derived& s_)
      {
        using impl::size;
        return size(s_.value());
      }

      // requires requires { Derived(String()); };
      friend auto substr(const Derived& s_,
                         std::string::size_type pos_ = 0,
                         std::string::size_type len_ = std::string::npos)
      {
        using impl::substr;
        return Derived(substr(s_.value(), pos_, len_));
      }

      friend Derived& operator+=(Derived& lhs_, const Derived& rhs_)
      {
        const_cast<String&>(lhs_.value()) += rhs_.value();
        return lhs_;
      }

      template <class Container>
      friend typename std::enable_if<
          std::is_base_of<Derived, typename Container::value_type>::value,
          Derived>::type
      join(const Container& c_, const Derived& separator_)
      {
        Derived result;
        bool first = true;
        for (const Derived& s : c_)
        {
          if (first)
          {
            first = false;
          }
          else
          {
            result += separator_;
          }
          result += s;
        }
        return result;
      }

      friend std::string to_string(const Derived& s_)
      {
        using impl::to_string;
        return to_string(s_.value());
      }

      friend std::ostream& operator<<(std::ostream& o_, const Derived& s_)
      {
        return o_ << s_.value();
      }

      friend auto begin(const Derived& s_)
      {
        using std::begin;
        return begin(s_.value());
      }

      friend auto end(const Derived& s_)
      {
        using std::end;
        return end(s_.value());
      }

      friend bool operator==(const Derived& lhs_, const Derived& rhs_)
      {
        return lhs_.value() == rhs_.value();
      }

      friend bool operator<(const Derived& lhs_, const Derived& rhs_)
      {
        return lhs_.value() < rhs_.value();
      }

      friend bool operator!=(const Derived& lhs_, const Derived& rhs_)
      {
        return !(lhs_ == rhs_);
      }

      friend bool operator<=(const Derived& lhs_, const Derived& rhs_)
      {
        return !(rhs_ < lhs_);
      }

      friend bool operator>(const Derived& lhs_, const Derived& rhs_)
      {
        return rhs_ < lhs_;
      }

      friend bool operator>=(const Derived& lhs_, const Derived& rhs_)
      {
        return !(lhs_ < rhs_);
      }

      template <size_t Len>
      friend bool operator==(const char (&lhs_)[Len], const Derived& rhs_)
      {
        return lhs_ == rhs_.value();
      }

      template <size_t Len>
      friend bool operator==(const Derived& lhs_, const char (&rhs_)[Len])
      {
        return rhs_ == lhs_;
      }

      friend Derived operator+(const Derived& lhs_, const Derived& rhs_)
      {
        Derived lhs(lhs_);
        return lhs += rhs_;
      }

      friend std::string operator+(const Derived& lhs_, const std::string& rhs_)
      {
        return lhs_.value() + rhs_;
      }

      friend std::string operator+(const std::string& lhs_, const Derived& rhs_)
      {
        return lhs_ + rhs_.value();
      }

      friend bool starts_with(const Derived& input_, const Derived& test_)
      {
        using impl::size;
        using std::begin;

        const auto len = size(test_.value());
        return size(input_.value()) >= len &&
               std::equal(begin(input_.value()), begin(input_.value()) + len,
                          begin(test_.value()));
      }

      friend bool ends_with(const Derived& input_, const Derived& test_)
      {
        using impl::size;
        using std::begin;
        using std::end;

        const auto len = size(test_.value());
        return size(input_.value()) >= len &&
               std::equal(end(input_.value()) - len, end(input_.value()),
                          begin(test_.value()));
      }

      friend Derived trim_copy(const Derived& s_)
      {
        using std::begin;
        using std::end;

        const auto spc = [](auto c_) {
          using std::isspace;
          return isspace(c_);
        };

        auto e = end(s_.value());

        const auto b = std::find_if_not(begin(s_.value()), e, spc);
        if (b != e)
        {
          --e;
          while (e != b && spc(*e))
          {
            --e;
          }
          ++e;
        }

        return Derived(String(b, e));
      }

      template <class UnaryPredicate>
      friend auto count_if(const Derived& s_, UnaryPredicate p_)
      {
        using std::begin;
        using std::end;

        return std::count_if(begin(s_.value()), end(s_.value()), p_);
      }

      template <class T>
      friend auto count(const Derived& s_, T c_)
      {
        using std::begin;
        using std::end;

        return std::count(begin(s_.value()), end(s_.value()), c_);
      }
    };
  }
}

#endif
