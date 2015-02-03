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

#include <metashell/in_memory_environment.hpp>
#include <metashell/standard.hpp>
#include <metashell/config.hpp>
#include <metashell/path_builder.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>

#include <algorithm>
#include <iterator>
#include <cassert>
#include <sstream>

using namespace metashell;

namespace
{
  const std::vector<std::string> no_extra_arguments;

  template <class Cont>
  void add_with_prefix(
    const std::string& prefix_,
    const Cont& cont_,
    std::vector<std::string>& v_
  )
  {
    std::transform(
      cont_.begin(),
      cont_.end(),
      std::back_insert_iterator<std::vector<std::string> >(v_),
      [&prefix_] (const std::string& s_) { return prefix_ + s_; }
    );
  }

  std::string set_max_template_depth(int v_)
  {
    std::ostringstream s;
    s << "-ftemplate-depth=" << v_;
    return s.str();
  }

  std::string seq_formatter(const std::string& name_)
  {
    return
      "#include <boost/mpl/fold.hpp>\n"
      "#include <boost/mpl/" + name_ + ".hpp>\n"

      "namespace boost_"
      "{"
        "namespace mpl"
        "{"
          "template <class... Ts>"
          "struct " + name_ +
          "{"
            "typedef " + name_ + " type;"
          "};"
        "}"
      "}"

      "namespace metashell"
      "{"
        "namespace impl "
        "{ "
          "template <class C, class Item> "
          "struct " + name_ + "_builder;\n"

          "template <class... Ts, class Item>"
          "struct "
            + name_ + "_builder<::boost_::mpl::" + name_ + "<Ts...>, Item> : "
            "::boost_::mpl::" + name_ + "<"
              "Ts..., typename ::metashell::format<Item>::type"
            ">"
          "{};"
        "} "

        "template <> "
        "struct format_impl<::boost::mpl::" + name_ + "<>::tag> "
        "{ "
          "typedef format_impl type; "

          "template <class V> "
          "struct apply : "
            "::boost::mpl::fold<"
              "V,"
              "::boost_::mpl::" + name_ + "<>,"
              "::metashell::impl::" + name_ + "_builder<"
                "::boost::mpl::_1, "
                "::boost::mpl::_2"
              ">"
            ">"
          "{};"
        "};"
      "}"
      "\n"
      ;
  }

  std::string include_formatter(const std::string& name_)
  {
    using std::string;
    using metashell::path_builder;

    return
      "#include <"
        + string(path_builder() / "metashell" / "formatter" / (name_ + ".hpp"))
        + ">";
  }

  void add_internal_headers(data::headers& headers_)
  {
    using boost::algorithm::join;
    using boost::adaptors::transformed;
  
    using std::string;
  
    const path_builder internal_dir(headers_.internal_dir());
    const string hpp(".hpp");
  
    const char* formatters[] = {"vector", "list", "set", "map"};
  
    for (const char* f : formatters)
    {
      headers_.add(
        internal_dir / "metashell" / "formatter" / (f + hpp),
        seq_formatter(f)
      );
    }
  
    const string vector_formatter =
       string(path_builder() / "metashell" / "formatter" / "vector.hpp");
  
    headers_.add(
      internal_dir / "metashell" / "formatter" / "deque.hpp",
      "#include <" + vector_formatter + ">\n"
    );
  
    headers_.add(
      internal_dir / "metashell" / "formatter.hpp",
      join(formatters | transformed(include_formatter), "\n") + "\n"
    );
  
    headers_.add(
      internal_dir / "metashell" / "scalar.hpp",
      "#include <type_traits>\n"
  
      "#define SCALAR(...) "
        "std::integral_constant<"
          "std::remove_reference<"
            "std::remove_cv<"
              "std::remove_reference<"
                "decltype((__VA_ARGS__))"
              ">::type"
            ">::type"
          ">::type,"
          "(__VA_ARGS__)"
        ">\n"
    );
  }
}

in_memory_environment::in_memory_environment(
  const std::string& internal_dir_,
  const config& config_,
  const std::string& clang_extra_arg_,
  logger* logger_
) :
  _buffer(),
  _headers(internal_dir_),
  _clang_args(),
  _logger(logger_)
{
  assert(!internal_dir_.empty());

  add_internal_headers(_headers);

  _clang_args.push_back("-x");
  _clang_args.push_back("c++-header");
  _clang_args.push_back(clang_argument(config_.standard_to_use));
  _clang_args.push_back("-I" + internal_dir());
  _clang_args.push_back(set_max_template_depth(config_.max_template_depth));

  add_with_prefix("-I", config_.include_path, _clang_args);
  add_with_prefix("-D", config_.macros, _clang_args);

  if (!config_.warnings_enabled)
  {
    _clang_args.push_back("-w");
  }

  _clang_args.insert(
    _clang_args.end(),
    config_.extra_clang_args.begin(),
    config_.extra_clang_args.end()
  );

  if (!clang_extra_arg_.empty())
  {
    _clang_args.push_back(clang_extra_arg_);
  }
}

void in_memory_environment::append(const std::string& s_)
{
  _buffer = get_appended(s_);
}

std::string in_memory_environment::get() const
{
  return _buffer;
}

std::string in_memory_environment::get_appended(const std::string& s_) const
{
  return _buffer.empty() ? s_ : (_buffer + '\n' + s_);
}

std::vector<std::string>& in_memory_environment::clang_arguments()
{
  return _clang_args;
}

const std::vector<std::string>&
  in_memory_environment::clang_arguments() const
{
  return _clang_args;
}

std::string in_memory_environment::internal_dir() const
{
  return _headers.internal_dir();
}

const data::headers& in_memory_environment::get_headers() const
{
  return _headers;
}

void in_memory_environment::add_clang_arg(const std::string& arg_)
{
  _clang_args.push_back(arg_);
}

std::string in_memory_environment::get_all() const
{
  return _buffer;
}

logger* in_memory_environment::get_logger()
{
  return _logger;
}

