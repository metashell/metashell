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

#include "headers.hpp"

#include <metashell/shell.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/foreach.hpp>

using namespace metashell;

namespace
{
  CXUnsavedFile create_entry(const std::pair<std::string, std::string>& header_)
  {
    CXUnsavedFile entry;
    entry.Filename = header_.first.c_str();
    entry.Contents = header_.second.c_str();
    entry.Length = header_.second.size();
    return entry;
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
            "::boost_::mpl::" + name_ + "<Ts..., Item>"
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
}

headers::headers(const std::string& src_) :
  _internal_dir("__metashell_internal"),
  _headers()
{
  using boost::algorithm::join;
  using boost::adaptors::transformed;
  using boost::phoenix::arg_names::arg1;

  const char* formatters[] = {"vector", "list", "set", "map"};

  BOOST_FOREACH(const char* f, formatters)
  {
    add(_internal_dir + "/metashell/formatter/" + f + ".hpp", seq_formatter(f));
  }
  add(
    _internal_dir + "/metashell/formatter/deque.hpp",
    "#include <metashell/formatter/vector.hpp>\n"
  );

  add(
    _internal_dir + "/metashell/formatter.hpp",
    join(
      formatters |
        transformed(
          std::string("#include <metashell/formatter/")
          + arg1 + std::string(".hpp>")
        ),
      "\n"
    ) + "\n"
  );

  add(
    _internal_dir + "/metashell/scalar.hpp",
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

  add(shell::input_filename(), src_);
}

void headers::add(const std::string& filename_, const std::string& content_)
{
  _headers.push_back(header(filename_, content_));
}

headers::iterator headers::begin() const
{
  return iterator(_headers.begin(), create_entry);
}

headers::iterator headers::end() const
{
  return iterator(_headers.end(), create_entry);
}

const std::string& headers::internal_dir() const
{
  return _internal_dir;
}

std::string headers::operator[](const std::string& filename_) const
{
  BOOST_FOREACH(const header& h, _headers)
  {
    if (h.first == filename_)
    {
      return h.second;
    }
  }
  return std::string();
}


