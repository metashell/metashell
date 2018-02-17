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

#include <metashell/header_discoverer_vc.hpp>

#include <metashell/includes_cache.hpp>

#include <boost/xpressive/xpressive.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>

#include <boost/optional.hpp>

#include <just/environment.hpp>
#include <just/lines.hpp>

#include <algorithm>

namespace
{
  boost::optional<std::string> include_path_addition(const std::string& arg_)
  {
    using boost::algorithm::starts_with;
    using boost::algorithm::ends_with;

    if ((starts_with(arg_, "\"/I") || starts_with(arg_, "/I\"")) &&
        ends_with(arg_, "\""))
    {
      return arg_.substr(3, arg_.size() - 4);
    }
    else if (starts_with(arg_, "/I"))
    {
      return arg_.substr(2);
    }

    return boost::none;
  }

  metashell::data::includes
  determine_includes(const std::vector<std::string>& cl_args_)
  {
    const std::string include = just::environment::get("INCLUDE");

    metashell::data::includes result{{}, {"."}};

    boost::algorithm::split(
        result.sys, include, boost::algorithm::is_any_of(";"));

    result.sys.erase(std::remove_if(result.sys.begin(), result.sys.end(),
                                    [](const boost::filesystem::path& p_) {
                                      return p_.empty();
                                    }),
                     result.sys.end());

    for (const std::string& arg : cl_args_)
    {
      if (const boost::optional<std::string> path = include_path_addition(arg))
      {
        result.sys.push_back(*path);
      }
    }

    result.quote.insert(
        result.quote.end(), result.sys.begin(), result.sys.end());

    return result;
  }
}

namespace metashell
{
  header_discoverer_vc::header_discoverer_vc(vc_binary vc_binary_)
    : _vc_binary(vc_binary_),
      _includes(determine_includes(vc_binary_.base_args()))
  {
  }

  std::vector<boost::filesystem::path>
  header_discoverer_vc::include_path(data::include_type type_)
  {
    return get(type_, _includes);
  }

  std::set<boost::filesystem::path>
  header_discoverer_vc::files_included_by(const data::cpp_code& exp_)
  {
    using boost::xpressive::bos;
    using boost::xpressive::as_xpr;
    using boost::xpressive::set;

    const data::process_output output =
        run_vc(_vc_binary, {"/c", "/showIncludes"}, exp_);

    const boost::xpressive::sregex included_header =
        bos >> +~(set = ':') >> ": " >> +~(set = ':') >> ":" >> *as_xpr(' ');

    std::set<boost::filesystem::path> result;

    for (const std::string line : just::lines::view(output.standard_output))
    {
      boost::xpressive::smatch what;
      if (regex_search(line, what, included_header))
      {
        result.insert(what.suffix().str());
      }
    }

    return result;
  }
}
