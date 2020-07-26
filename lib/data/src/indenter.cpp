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

#include <metashell/data/indenter.hpp>

#include <algorithm>
#include <cassert>
#include <vector>

namespace metashell
{
  namespace data
  {
    namespace
    {
      bool is_whitespace(char c_)
      {
        return c_ == ' ' || c_ == '\t' || c_ == '\v' || c_ == '\r' ||
               c_ == '\n';
      }

      template <class It>
      It pos_after_last_non_whitespace(It begin_, It end_)
      {
        if (begin_ == end_)
        {
          return end_;
        }
        else
        {
          for (It i = end_ - 1; i != begin_; --i)
          {
            if (!is_whitespace(*i))
            {
              return i + 1;
            }
          }

          return is_whitespace(*begin_) ? end_ : begin_;
        }
      }

      template <class It>
      It first_non_whitespace(It begin_, It end_)
      {
        return std::find_if(
            begin_, end_, [](char c_) { return !is_whitespace(c_); });
      }

      template <class It>
      void
      word_wrap(It begin_, It end_, int width_, std::vector<std::string>& out_)
      {
        // Some environments report 0 width
        if (width_ <= 0)
        {
          width_ = 80;
        }

        for (auto i = begin_; i != end_;)
        {
          const auto max = i + std::min<int>(end_ - i, width_);
          auto break_at = i;
          auto last_whitespace = end_;
          for (; break_at != max && *break_at != '\n'; ++break_at)
          {
            if (is_whitespace(*break_at))
            {
              last_whitespace = break_at;
            }
          }

          const auto bp = (static_cast<int>(break_at - i) == width_ &&
                           last_whitespace != end_) ?
                              last_whitespace :
                              break_at;

          out_.push_back(std::string(i, pos_after_last_non_whitespace(i, bp)));

          const auto new_i = (bp == end_ || *bp != '\n') ?
                                 first_non_whitespace(bp, end_) :
                                 bp + 1;
          assert(i < new_i);
          i = new_i;
        }
      }
    }

    indenter::indenter(int width_, std::string line_sep_)
      : _width{width_}, _line_sep{std::move(line_sep_)}
    {
    }

    std::string indenter::str() const { return _buff.str(); }

    indenter& indenter::raw(const std::string& s_)
    {
      _buff << s_ << _line_sep;
      return *this;
    }

    indenter& indenter::left_align(const std::string& s_,
                                   const std::string& line_prefix_,
                                   const std::string& first_line_prefix_)
    {
      assert(first_line_prefix_.length() == line_prefix_.length());

      std::vector<std::string> lines;
      word_wrap(s_.begin(), s_.end(), _width - line_prefix_.length(), lines);
      bool first = true;
      for (const std::string& line : lines)
      {
        _buff << (first ? first_line_prefix_ : line_prefix_) << line
              << _line_sep;
        first = false;
      }
      return *this;
    }
  }
}
