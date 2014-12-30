// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Abel Sinkovics (abel@sinkovics.hu)
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

#include <boost/algorithm/string/trim_all.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <boost/assign/list_of.hpp>

#include <algorithm>

using namespace metashell;

namespace
{
  std::string quote_argument(const std::string& arg_)
  {
#ifdef _WIN32
    return "\"" + arg_ + "\"";
#else
    return arg_;
#endif
  }

  bool is_new_line(char c_)
  {
    return c_ == '\n';
  }
}

clang_binary::clang_binary(const std::string& path_) :
  _path(path_)
{}

just::process::output clang_binary::run(
  const std::vector<std::string>& args_,
  const std::string& stdin
) const
{
  std::vector<std::string> cmd(args_.size() + 1);

  std::vector<std::string>::iterator i = cmd.begin();
  *i = quote_argument(_path);
  ++i;
  std::transform(args_.begin(), args_.end(), i, quote_argument);

  return just::process::run(cmd, stdin);
}

std::vector<std::string> metashell::default_sysinclude(
  const clang_binary& clang_
)
{
  using boost::algorithm::trim_all_copy;
  using boost::algorithm::split;
  using boost::starts_with;
  using boost::assign::list_of;

  using std::vector;
  using std::string;

  const just::process::output o =
    clang_.run(list_of<string>("-v")("-xc++")("-"));

  const string s = o.standard_output() + o.standard_error();

  vector<string> lines;
  split(lines, s, is_new_line);

  vector<string> result;
  bool in_sysinclude = false;
  for (const string& line : lines)
  {
    if (in_sysinclude)
    {
      if (starts_with(line, " "))
      {
        result.push_back(trim_all_copy(line));
      }
      else
      {
        return result;
      }
    }
    else if (starts_with(line, "#include <"))
    {
      in_sysinclude = true;
    }
  }

  return result;
}

