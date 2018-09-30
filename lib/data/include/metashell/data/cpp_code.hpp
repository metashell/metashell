#ifndef METASHELL_CPP_CODE_HPP
#define METASHELL_CPP_CODE_HPP

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

#include <boost/operators.hpp>

#include <iosfwd>
#include <string>

namespace metashell
{
  namespace data
  {
    class cpp_code : boost::totally_ordered<cpp_code>,
                     boost::equality_comparable<cpp_code, std::string>,
                     boost::addable<cpp_code>
    {
    public:
      typedef std::string::iterator iterator;
      typedef std::string::const_iterator const_iterator;
      typedef std::string::size_type size_type;

      explicit cpp_code(std::string value_ = std::string());

      template <class InputIterator>
      cpp_code(InputIterator begin_, InputIterator end_) : _value(begin_, end_)
      {
      }

      const std::string& value() const;

      bool empty() const;
      size_type size() const;
      char operator[](size_type pos_) const;

      cpp_code substr(size_type pos_ = 0,
                      size_type len_ = std::string::npos) const;

      iterator begin();
      iterator end();

      const_iterator begin() const;
      const_iterator end() const;

      cpp_code& operator+=(const cpp_code& code_);
      cpp_code& operator+=(const std::string& code_);

      template <class InputIterator>
      void insert(iterator at_, InputIterator first_, InputIterator last_)
      {
        _value.insert(at_, first_, last_);
      }

    private:
      std::string _value;
    };

    std::string to_string(const cpp_code& c_);

    std::ostream& operator<<(std::ostream& o_, const cpp_code& c_);

    bool operator==(const cpp_code& a_, const cpp_code& b_);
    bool operator<(const cpp_code& a_, const cpp_code& b_);

    bool operator==(const cpp_code& a_, const std::string& b_);

    cpp_code operator+(cpp_code code_, const std::string& s_);
    cpp_code operator+(std::string s_, const cpp_code& code_);

    cpp_code add_markers(const cpp_code& code_, bool process_directives_);
    cpp_code remove_markers(const cpp_code& code_, bool process_directives_);

    int lines_in(const cpp_code& code_);
  }
}

#endif
