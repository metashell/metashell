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

#include <metashell/headers.hpp>
#include <metashell/shell.hpp>
#include <metashell/path_builder.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptors.hpp>

using namespace metashell;

namespace
{
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

    return
      "#include <"
        + string(path_builder() / "metashell" / "formatter/" / (name_ + ".hpp"))
        + ">";
  }
}

headers::headers(const std::string& internal_dir_, bool empty_) :
  _headers(),
  _internal_dir(internal_dir_)
{
  if (!empty_)
  {
    using boost::algorithm::join;
    using boost::adaptors::transformed;

    using std::string;
    
    const path_builder internal_dir(_internal_dir);
    const string hpp(".hpp");

    const char* formatters[] = {"vector", "list", "set", "map"};

    for (const char* f : formatters)
    {
      add(
        internal_dir / "metashell" / "formatter" / (f + hpp),
        seq_formatter(f)
      );
    }

    const string vector_formatter =
       string(path_builder() / "metashell" / "formatter" / "vector.hpp");

    add(
      internal_dir / "metashell" / "formatter" / "deque.hpp",
      "#include <" + vector_formatter + ">\n"
    );

    add(
      internal_dir / "metashell" / "formatter.hpp",
      join(formatters | transformed(include_formatter), "\n") + "\n"
    );

    add(
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

void headers::add(const std::string& filename_, const std::string& content_)
{
  _headers.push_back(unsaved_file(filename_, content_));
}

headers::iterator headers::begin() const
{
  return _headers.begin();
}

headers::iterator headers::end() const
{
  return _headers.end();
}

void headers::generate() const
{
  for (const unsaved_file& h : _headers)
  {
    h.generate();
  }
}

headers::size_type headers::size() const
{
  return _headers.size();
}

const std::string& headers::internal_dir() const
{
  return _internal_dir;
}

