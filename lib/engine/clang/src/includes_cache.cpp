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

#include <metashell/engine/clang/includes_cache.hpp>

#include <metashell/data/include_type.hpp>
#include <metashell/data/process_output.hpp>

#include <just/lines.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <algorithm>
#include <string>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
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

        template <data::include_type... Types, class ForwardIt>
        ForwardIt beginning_of_includes(ForwardIt begin_, ForwardIt end_)
        {
          const std::vector<std::string> prefixes{
              data::include_dotdotdot<Types>()...};

          return std::find_if(
              begin_, end_, [&prefixes](const std::string& line_) {
                return std::find_if(prefixes.begin(), prefixes.end(),
                                    [&line_](const std::string& prefix_) {
                                      return line_.find(prefix_) !=
                                             std::string::npos;
                                    }) != end(prefixes);
              });
        }

        template <data::include_type Type, class ForwardIt>
        std::vector<boost::filesystem::path>
        include_path_of_type(ForwardIt lines_begin_, ForwardIt lines_end_)
        {
          using boost::algorithm::starts_with;
          using boost::algorithm::trim_copy;

          std::vector<boost::filesystem::path> result;
          const auto includes_begin =
              next(beginning_of_includes<Type>(lines_begin_, lines_end_),
                   lines_end_);

          transform(includes_begin,
                    find_if(includes_begin, lines_end_,
                            [](const std::string& line_) {
                              return !starts_with(line_, " ");
                            }),
                    back_inserter(result),
                    [](const std::string& s_) { return trim_copy(s_); });

          return result;
        }

        template <class LineView>
        data::includes determine_clang_includes(const LineView& lines_)
        {
          using data::include_type;

          const auto begin =
              beginning_of_includes<include_type::sys, include_type::quote>(
                  lines_.begin(), lines_.end());
          data::includes result{
              include_path_of_type<include_type::sys>(begin, lines_.end()),
              include_path_of_type<include_type::quote>(begin, lines_.end())};

          result.quote.insert(
              result.quote.end(), result.sys.begin(), result.sys.end());
          return result;
        }

        data::includes
        determine_includes(const iface::executable& clang_binary_,
                           data::standard_headers_allowed allowed_)
        {
          data::command_line_argument_list args{"-v", "-xc++", "-E"};
          switch (allowed_)
          {
          case data::standard_headers_allowed::all:
            break;
          case data::standard_headers_allowed::c:
            args.push_back("-nostdinc++");
            break;
          case data::standard_headers_allowed::cpp:
            args.push_back("-nostdinc");
            break;
          case data::standard_headers_allowed::none:
            args.push_back("-nostdinc");
            args.push_back("-nostdinc++");
            break;
          }

          const data::process_output o =
              run_clang(clang_binary_, args, data::cpp_code());

          const std::string s = o.standard_output + o.standard_error;
          return determine_clang_includes(just::lines::view_of(s));
        }
      } // namespace

      core::cached<data::includes>
      includes_cache(binary binary_, data::standard_headers_allowed allowed_)
      {
        return core::cached<data::includes>([binary_, allowed_]() {
          return determine_includes(binary_, allowed_);
        });
      }
    } // namespace clang
  } // namespace engine
} // namespace metashell
