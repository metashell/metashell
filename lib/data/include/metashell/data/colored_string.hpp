#ifndef METASHELL_COLORED_STRING_HPP
#define METASHELL_COLORED_STRING_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <metashell/data/color.hpp>
#include <metashell/data/visual_cpp_workaround.hpp>

#include <boost/operators.hpp>

#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace metashell
{
  namespace data
  {

    class colored_string : boost::addable<colored_string>,
                           boost::equality_comparable<colored_string>
    {
    public:
      typedef std::optional<color> color_t;
      typedef std::vector<color_t> colors_t;

      typedef std::string::size_type size_type;

      colored_string() = default;
      colored_string(const char* string, const color_t& color = std::nullopt);
      colored_string(const std::string& string,
                     const color_t& color = std::nullopt);

      colored_string& operator+=(const char* rhs);
      colored_string& operator+=(const std::string& rhs);
      colored_string& operator+=(const colored_string& rhs);

      size_type size() const;

      colored_string substr(size_type pos_, size_type len_) const;

      const std::string& get_string() const;
      const colors_t& get_colors() const;

      void clear();

    private:
      template <class CharIt, class ColorIt>
      colored_string(CharIt sbegin_,
                     CharIt send_,
                     ColorIt cbegin_,
                     ColorIt cend_)
        : string(sbegin_, send_), colors(cbegin_, cend_)
      {
      }

      std::string string;
      colors_t colors;
    };

    void print_to_cout(const colored_string& s_);

    bool operator==(const colored_string& a_, const colored_string& b_);

    inline colored_string operator+(const char* rhs, const colored_string& lhs)
    {
      return colored_string(rhs) + lhs;
    }

    inline colored_string operator+(const std::string& rhs,
                                    const colored_string& lhs)
    {
      return colored_string(rhs) + lhs;
    }

    inline colored_string operator+(colored_string rhs, const char* lhs)
    {
      return rhs += lhs;
    }

    inline colored_string operator+(colored_string rhs, const std::string& lhs)
    {
      return rhs += lhs;
    }

    std::ostream& operator<<(std::ostream& os, const colored_string& cs);
  }
}

#endif
