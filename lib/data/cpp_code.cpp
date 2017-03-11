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

#include <metashell/data/cpp_code.hpp>

#include <ostream>
#include <stdexcept>

namespace
{
  template <class Str, class Wrapper>
  auto wrap(const Str& s_, const Wrapper& wrapper_)
      -> decltype(wrapper_ + s_ + wrapper_)
  {
    return wrapper_ + s_ + wrapper_;
  }

  std::string marker(bool process_directives_)
  {
    return wrap("* __METASHELL_PP_MARKER *",
                std::string(process_directives_ ? "\n" : ""));
  }
}

namespace metashell
{
  namespace data
  {
    cpp_code::cpp_code(std::string value_) : _value(move(value_)) {}

    const std::string& cpp_code::value() const { return _value; }

    bool cpp_code::empty() const { return _value.empty(); }

    cpp_code::size_type cpp_code::size() const { return _value.size(); }

    char cpp_code::operator[](size_type pos_) const { return _value[pos_]; }

    cpp_code cpp_code::substr(size_type pos_, size_type len_) const
    {
      return cpp_code(_value.substr(pos_, len_));
    }

    cpp_code::iterator cpp_code::begin() { return _value.begin(); }
    cpp_code::iterator cpp_code::end() { return _value.end(); }

    cpp_code::const_iterator cpp_code::begin() const { return _value.begin(); }
    cpp_code::const_iterator cpp_code::end() const { return _value.end(); }

    cpp_code& cpp_code::operator+=(const cpp_code& code_)
    {
      return *this += code_.value();
    }

    cpp_code& cpp_code::operator+=(const std::string& code_)
    {
      _value += code_;
      return *this;
    }

    std::string to_string(const cpp_code& c_) { return c_.value(); }

    std::ostream& operator<<(std::ostream& o_, const cpp_code& c_)
    {
      return o_ << c_.value();
    }

    bool operator==(const cpp_code& a_, const cpp_code& b_)
    {
      return a_ == b_.value();
    }

    bool operator<(const cpp_code& a_, const cpp_code& b_)
    {
      return a_.value() < b_.value();
    }

    bool operator==(const cpp_code& a_, const std::string& b_)
    {
      return a_.value() == b_;
    }

    cpp_code operator+(cpp_code code_, const std::string& s_)
    {
      return code_ += s_;
    }

    cpp_code operator+(std::string s_, const cpp_code& code_)
    {
      return cpp_code(s_) += code_;
    }

    cpp_code add_markers(const cpp_code& code_, bool process_directives_)
    {
      return wrap(code_, marker(process_directives_));
    }

    cpp_code remove_markers(const cpp_code& code_, bool process_directives_)
    {
      const std::string code = code_.value();
      const std::string mrk = marker(process_directives_);

      const auto p1 = code.find(mrk);
      if (p1 == std::string::npos)
      {
        throw std::runtime_error(
            "Marker (" + mrk +
            ") not found in preprocessed output."
            " Does it contain a macro that has been defined?");
      }
      else
      {
        const auto m_len = mrk.size();
        const auto p2 = code.find(mrk, p1 + m_len);
        if (p2 == std::string::npos)
        {
          throw std::runtime_error("Marker (" + mrk +
                                   ") found only once in preprocessed output.");
        }
        else if (code.find(mrk, p2 + m_len) == std::string::npos)
        {
          return cpp_code(code.substr(p1 + m_len, p2 - p1 - m_len));
        }
        else
        {
          throw std::runtime_error(
              "Marker (" + mrk +
              ") found more than two times in preprocessed output.");
        }
      }
    }
  }
} // namespace metashell:data
