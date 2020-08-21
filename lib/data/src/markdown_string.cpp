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

#include <metashell/data/markdown_string.hpp>

#include <boost/algorithm/string/replace.hpp>

#include <boost/range/combine.hpp>

#include <boost/tuple/tuple.hpp>

#include <iostream>
#include <regex>

namespace metashell
{
  namespace data
  {
    namespace
    {
      template <class Size>
      void adopt_widths(const std::vector<markdown_string>& row_,
                        std::vector<Size>& widths_)
      {
        assert(widths_.empty() || widths_.size() == row_.size());
        widths_.resize(row_.size(), 0);

        for (auto p : boost::combine(widths_, row_))
        {
          Size& width = boost::get<0>(p);
          const markdown_string& cell = boost::get<1>(p);

          width = std::max(width, cell.size());
        }
      }

      template <class Size>
      void format_row(const std::vector<markdown_string>& row_,
                      const std::vector<Size>& widths_,
                      std::ostream& out_)
      {
        assert(row_.size() == widths_.size());

        for (auto p : boost::combine(widths_, row_))
        {
          const Size& width = boost::get<0>(p);
          const markdown_string& cell = boost::get<1>(p);

          assert(cell.size() <= width);

          out_ << '|' << cell << std::string(width - cell.size(), ' ');
        }
        out_ << "|\n";
      }

      template <class Size>
      void format_header_separator(const std::vector<Size>& widths_,
                                   std::ostream& out_)
      {
        bool first_column = true;
        for (int width : widths_)
        {
          if (first_column)
          {
            assert(width >= 1);
            out_ << "|:" << std::string(width - 1, '-');
            first_column = false;
          }
          else
          {
            assert(width >= 2);
            out_ << "|:" << std::string(width - 2, '-') << ":";
          }
        }
        out_ << "|\n";
      }
    }

    markdown_string italics(const markdown_string& md_)
    {
      return markdown_string("_" + md_.value() + "_");
    }

    markdown_string make_id(const std::string& value_)
    {
      return markdown_string(
          boost::algorithm::replace_all_copy(value_, " ", "_"));
    }

    markdown_string self_reference(const std::string& value_)
    {
      return markdown_string("<a href=\"#") + make_id(value_) +
             markdown_string("\">" + value_ + "</a>");
    }

    markdown_string self_id(const std::string& value_)
    {
      return markdown_string("<strong id=\"") + make_id(value_) +
             markdown_string("\">" + value_ + "</strong>");
    }

    void format_table(const std::vector<markdown_string>& header_,
                      const std::vector<std::vector<markdown_string>>& table_,
                      std::ostream& out_)
    {
      std::vector<std::vector<std::string>::size_type> widths;
      adopt_widths(header_, widths);
      for (const auto& row : table_)
      {
        adopt_widths(row, widths);
      }

      format_row(header_, widths, out_);
      format_header_separator(widths, out_);
      for (const auto& row : table_)
      {
        format_row(row, widths, out_);
      }
    }

    std::string unformat(const markdown_string& s_)
    {
      std::string s(s_.value());

      boost::replace_all(s, "`", "");
      boost::replace_all(s, "<br />", "\n");
      boost::replace_all(s, "&nbsp;", " ");

      return std::regex_replace(
          s, std::regex("\\[([^\\]]*)\\]\\(([^)]*)\\)"), "$01 (see $02)");
    }

    markdown_string auto_engine_description()
    {
      return metashell::data::markdown_string(
          "Chooses an engine based on the compiler "
          "specified in the arguments.");
    }
  }
}
