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
#include <iosfwd>
#include <string>
#include <type_traits>

namespace metashell
{
  namespace data
  {
    namespace impl
    {
      std::string c_escape(int);

      std::string to_string(const std::string&);

      void throw_(const std::string&);

      bool isspace(int);
    }

    template <class Derived,
              bool AllowEmpty = true,
              class NonFirstCharConstraints = constraint::any,
              class FirstCharConstraints = NonFirstCharConstraints,
              class String = std::string>
    class string
    {
    public:
      string() : _value{}
      {
        static_assert(AllowEmpty, "Empty values are disabled.");

        check_invariant(_value);
      }

      template <class Str,
                class = std::enable_if_t<std::is_constructible_v<String, Str>>>
      explicit string(Str value_) : _value{std::move(value_)}
      {
        check_invariant(_value);
      }

      template <class InputIterator>
      string(InputIterator begin_, InputIterator end_) : _value{begin_, end_}
      {
        check_invariant(_value);
      }

      const String& value() const { return _value; }

      auto c_str() const { return _value.c_str(); }

      bool empty() const { return _value.empty(); }

      void clear()
      {
        static_assert(AllowEmpty);
        _value.clear();
        check_invariant(_value);
      }

      auto size() const { return _value.size(); }

      template <class... Args>
      auto find(Args&&... args_) const
      {
        return _value.find(std::forward<Args>(args_)...);
      }

      auto substr(std::string::size_type pos_ = 0,
                  std::string::size_type len_ = std::string::npos) const
      {
        return Derived{_value.substr(pos_, len_)};
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

      auto begin() const { return _value.begin(); }

      auto end() const { return _value.end(); }

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
        const auto len = test_.size();
        return input_.size() >= len &&
               std::equal(input_.begin(), input_.begin() + len, test_.begin());
      }

      friend bool ends_with(const Derived& input_, const Derived& test_)
      {
        const auto len = test_.size();
        return input_.size() >= len &&
               std::equal(input_.end() - len, input_.end(), test_.begin());
      }

      friend Derived trim_copy(const Derived& s_)
      {
        const auto spc = [](auto c_) {
          using impl::isspace;
          return isspace(c_);
        };

        auto e = s_.end();

        const auto b = std::find_if_not(s_.begin(), e, spc);
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
        return std::count_if(s_.begin(), s_.end(), p_);
      }

      template <class T>
      friend auto count(const Derived& s_, T c_)
      {
        return std::count(s_.begin(), s_.end(), c_);
      }

    private:
      String _value;

      static void check_invariant(const String& s_)
      {
        if (!AllowEmpty && s_.empty())
        {
          impl::throw_("Empty " + std::string{Derived::name_of_type()} +
                       " value");
        }

        bool first = true;
        for (auto c : s_)
        {
          if ((first && !FirstCharConstraints::allowed_char(c)) ||
              (!first && !NonFirstCharConstraints::allowed_char(c)))
          {
            impl::throw_("Invalid character in " +
                         std::string{Derived::name_of_type()} + ": " +
                         impl::c_escape(c));
          }
          first = false;
        }
      }
    };
  }
}

#endif
