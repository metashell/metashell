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

#include <metashell/header_discoverer_clang.hpp>

#include <metashell/includes_cache.hpp>

#include <boost/xpressive/xpressive.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <just/lines.hpp>

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
}

namespace metashell
{
  header_discoverer_clang::header_discoverer_clang(clang_binary clang_binary_)
    : _clang_binary(clang_binary_), _includes(includes_cache(_clang_binary))
  {
  }

  std::vector<boost::filesystem::path>
  header_discoverer_clang::include_path(data::include_type type_)
  {
    return type_ == data::include_type::sys ? _includes->sys : _includes->quote;
  }

  std::set<boost::filesystem::path>
  header_discoverer_clang::files_included_by(const std::string& exp_)
  {
    const data::process_output output =
        run_clang(_clang_binary, {"-H", "-E"}, exp_);

    const boost::xpressive::sregex included_header =
        boost::xpressive::bos >> +boost::xpressive::as_xpr('.') >> ' ';

    const just::lines::view lines(output.standard_error);

    const auto result =
        lines |
        boost::adaptors::filtered([&included_header](const std::string& s_) {
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
}
