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

#include <metashell/indenter.hpp>

#include <vector>
#include <cassert>

using namespace metashell;

namespace
{
  bool is_whitespace(char c_)
  {
    return c_ == ' ' || c_ == '\t' || c_ == '\v' || c_ == '\r' || c_ == '\n';
  }

  void word_wrap(
    std::string s_,
    int width_,
    std::vector<std::string>& out_
  )
  {
    while (!s_.empty())
    {
      if (static_cast<int>(s_.length()) > width_)
      {
        int i = width_;
        while (i != -1 && !is_whitespace(s_[i]))
        {
          --i;
        }

        if (i == -1)
        {
          out_.push_back(s_.substr(0, width_));
          s_ = s_.substr(width_);
        }
        else
        {
          out_.push_back(s_.substr(0, i));
          s_ = s_.substr(i + 1);
        }
      }
      else
      {
        out_.push_back(s_);
        s_.clear();
      }
    }
  }
}

indenter::indenter(int width_) :
  _width(width_)
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
  for (const std::string& line : lines)
  {
    _buff << (first ? first_line_prefix_ : line_prefix_) << line << "\n";
    first = false;
  }
  return *this;
}


