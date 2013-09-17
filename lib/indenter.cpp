// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2013, Abel Sinkovics (abel@sinkovics.hu)
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

#include "indenter.hpp"

#include <boost/foreach.hpp>

#include <vector>
#include <cassert>

namespace
{
  bool is_whitespace(char c_)
  {
    return c_ == ' ' || c_ == '\t' || c_ == '\v' || c_ == '\r' || c_ == '\n';
  }

  void word_wrap(
    const std::string& s_,
    unsigned int width_,
    std::vector<std::string>& out_
  )
  {
    for (std::string s(s_); !s.empty();)
    {
      if (s.length() > width_)
      {
        int i = width_;
        while (i != -1 && !is_whitespace(s[i]))
        {
          --i;
        }

        if (i == -1)
        {
          out_.push_back(std::string(s.begin(), s.begin() + width_));
          s = std::string(s.begin() + width_, s.end());
        }
        else
        {
          out_.push_back(std::string(s.begin(), s.begin() + i));
          s = std::string(s.begin() + (i + 1), s.end());
        }
      }
      else
      {
        out_.push_back(s);
        s = std::string();
      }
    }
  }
}

indenter::indenter(unsigned int width_, const std::string& default_prefix_) :
  _width(width_),
  _default_prefix(default_prefix_)
{}

std::string indenter::str() const
{
  return _buff.str();
}

indenter& indenter::raw(const std::string& s_)
{
  _buff << s_ << "\n";
  return *this;
}

indenter& indenter::empty_line()
{
  return raw(_default_prefix);
}

indenter& indenter::left_align(const std::string& s_)
{
  return left_align(s_, _default_prefix, _default_prefix);
}

indenter& indenter::left_align(
  const std::string& s_,
  const std::string& line_prefix_
)
{
  return left_align(s_, line_prefix_, line_prefix_);
}

indenter& indenter::left_align(
  const std::string& s_,
  const std::string& line_prefix_,
  const std::string& first_line_prefix_
)
{
  assert(first_line_prefix_.length() == line_prefix_.length());

  std::vector<std::string> lines;
  word_wrap(s_, _width - line_prefix_.length(), lines);
  bool first = true;
  BOOST_FOREACH(const std::string& line, lines)
  {
    _buff << (first ? first_line_prefix_ : line_prefix_) << line << "\n";
    first = false;
  }
  return *this;
}


