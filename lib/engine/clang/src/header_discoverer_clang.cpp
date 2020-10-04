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

#include <metashell/engine/clang/header_discoverer.hpp>
#include <metashell/engine/clang/includes_cache.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <just/lines.hpp>

#include <cassert>
#include <regex>

namespace metashell
{
  namespace engine
  {
    namespace clang
    {
      namespace
      {
        template <class InputIt>
        std::string remove_double_backslashes(InputIt begin_, InputIt end_)
        {
          std::string result(end_ - begin_, 'X');
          std::string::iterator j = result.begin();
          bool wasBackslash = false;
          for (InputIt i = begin_; i != end_; ++i)
          {
            if (!wasBackslash || *i != '\\')
            {
              *j = *i;
              ++j;
            }
            wasBackslash = !wasBackslash && (*i == '\\');
          }
          result.erase(j, result.end());
          return result;
        }
      } // namespace

      header_discoverer::header_discoverer(binary binary_)
        : _binary(binary_),
          _all_includes(
              includes_cache(_binary, data::standard_headers_allowed::all)),
          _c_includes(
              includes_cache(_binary, data::standard_headers_allowed::c)),
          _cpp_includes(
              includes_cache(_binary, data::standard_headers_allowed::cpp))
      {
      }

      std::vector<boost::filesystem::path>
      header_discoverer::include_path(data::include_type type_,
                                      data::standard_headers_allowed allowed_)
      {
        switch (allowed_)
        {
        case data::standard_headers_allowed::all:
          return get(type_, *_all_includes);
        case data::standard_headers_allowed::c:
          return get(type_, *_c_includes);
        case data::standard_headers_allowed::cpp:
          return get(type_, *_cpp_includes);
        case data::standard_headers_allowed::none:
          return {};
        }
        assert(!"Invalid standard_headers_allowed");
        return {};
      }

      std::set<boost::filesystem::path>
      header_discoverer::files_included_by(const data::cpp_code& exp_)
      {
        const data::process_output output =
            run_clang(_binary, {"-H", "-E"}, exp_);

        const std::regex included_header("^\\.+ ");

        const just::lines::view lines(output.standard_error);

        const auto result =
            lines |
            boost::adaptors::filtered(
                [&included_header](const std::string& s_) {
                  return regex_search(s_, included_header);
                }) |
            boost::adaptors::transformed([](const std::string& s_) {
              const auto i = std::find(s_.begin(), s_.end(), ' ');

              return i == s_.end() ?
                         boost::filesystem::path() :
                         boost::filesystem::path(
                             remove_double_backslashes(i + 1, s_.end()));
            });

        return std::set<boost::filesystem::path>(result.begin(), result.end());
      }
    } // namespace clang
  } // namespace engine
} // namespace metashell
