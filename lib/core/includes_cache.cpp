// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2016, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/clang_binary.hpp>
#include <metashell/includes_cache.hpp>

#include <metashell/data/include_type.hpp>
#include <metashell/data/process_output.hpp>

#include <just/lines.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <algorithm>
#include <string>

namespace
{
  template <class ForwardIt>
  ForwardIt next(ForwardIt i_, ForwardIt end_)
  {
    if (i_ != end_)
    {
      ++i_;
    }
    return i_;
  }

  template <metashell::data::include_type... Types, class ForwardIt>
  ForwardIt beginning_of_includes(ForwardIt begin_, ForwardIt end_)
  {
    using boost::algorithm::starts_with;

    const std::vector<std::string> prefixes{
        metashell::data::include_dotdotdot<Types>()...};

    return std::find_if(begin_, end_, [&prefixes](const std::string& line_) {
      return std::find_if(prefixes.begin(), prefixes.end(),
                          [&line_](const std::string& prefix_) {
                            return starts_with(line_, prefix_);
                          }) != end(prefixes);
    });
  }

  template <metashell::data::include_type Type, class ForwardIt>
  std::vector<boost::filesystem::path>
  include_path_of_type(ForwardIt lines_begin_, ForwardIt lines_end_)
  {
    using boost::algorithm::starts_with;
    using boost::algorithm::trim_copy;

    std::vector<boost::filesystem::path> result;
    const auto includes_begin =
        next(beginning_of_includes<Type>(lines_begin_, lines_end_), lines_end_);

    transform(includes_begin, find_if(includes_begin, lines_end_,
                                      [](const std::string& line_) {
                                        return !starts_with(line_, " ");
                                      }),
              back_inserter(result),
              [](const std::string& s_) { return trim_copy(s_); });

    return result;
  }

  template <class LineView>
  metashell::data::includes determine_clang_includes(const LineView& lines_)
  {
    using metashell::data::include_type;

    const auto begin =
        beginning_of_includes<include_type::sys, include_type::quote>(
            lines_.begin(), lines_.end());
    metashell::data::includes result{
        include_path_of_type<include_type::sys>(begin, lines_.end()),
        include_path_of_type<include_type::quote>(begin, lines_.end())};

    result.quote.insert(
        result.quote.end(), result.sys.begin(), result.sys.end());
    return result;
  }

  metashell::data::includes
  determine_includes(const metashell::iface::executable& clang_binary_)
  {
    const metashell::data::process_output o = metashell::run_clang(
        clang_binary_, {"-v", "-xc++", "-E"}, metashell::data::cpp_code());

    const std::string s = o.standard_output + o.standard_error;
    return determine_clang_includes(just::lines::view_of(s));
  }
}

namespace metashell
{
  cached<data::includes> includes_cache(clang_binary clang_binary_)
  {
    return cached<data::includes>(
        [clang_binary_]() { return determine_includes(clang_binary_); });
  }
}
