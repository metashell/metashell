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

#include <metashell/data/config.hpp>
#include <metashell/data/exception.hpp>
#include <metashell/data/headers.hpp>

#include <metashell/core/header_file_environment.hpp>
#include <metashell/core/headers.hpp>
#include <metashell/core/unsaved_file.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <boost/range/adaptors.hpp>

#include <fstream>
#include <sstream>
#include <vector>

namespace metashell
{
  namespace core
  {
    namespace
    {
      std::string seq_formatter(const std::string& name_)
      {
        // clang-format off
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
        // clang-format on
      }

      std::string include_formatter(const std::string& name_)
      {
        using std::string;
        using boost::filesystem::path;

        return "#include <" +
               (path("metashell") / "formatter" / (name_ + ".hpp")).string() +
               ">";
      }

      void add_internal_headers(data::headers& headers_)
      {
        using boost::algorithm::join;
        using boost::adaptors::transformed;
        using boost::filesystem::path;

        using std::string;

        const path internal_dir(headers_.internal_dir());
        const string hpp(".hpp");

        const char* formatters[] = {"vector", "list", "set", "map"};

        for (const char* f : formatters)
        {
          headers_.add(internal_dir / "metashell" / "formatter" / (f + hpp),
                       seq_formatter(f));
        }

        const path vector_formatter =
            path("metashell") / "formatter" / "vector.hpp";

        headers_.add(internal_dir / "metashell" / "formatter" / "deque.hpp",
                     "#include <" + vector_formatter.string() + ">\n");

        headers_.add(
            internal_dir / "metashell" / "formatter.hpp",
            join(formatters | transformed(include_formatter), "\n") + "\n");

        headers_.add(internal_dir / "metashell" / "scalar.hpp",
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
                     ">\n");

        headers_.add(internal_dir / "metashell" / "instantiate_expression.hpp",
                     "namespace metashell\n"
                     "{\n"
                     "  template <bool> struct expression_instantiated;\n"
                     "}\n"

                     "#define METASHELL_INSTANTIATE_EXPRESSION(...) \\\n"
                     "  ::metashell::expression_instantiated<true ? true : "
                     "((__VA_ARGS__), false)>\n");
      }
    }

    header_file_environment::header_file_environment(
        iface::type_shell* type_shell_,
        const data::shell_config_data& config_,
        const boost::filesystem::path& internal_dir_,
        const boost::filesystem::path& env_filename_)
      : _internal_dir(internal_dir_),
        _env_filename(env_filename_),
        _buffer(),
        _headers(internal_dir_),
        _use_precompiled_headers(config_.use_precompiled_headers),
        _type_shell(type_shell_)
    {
      add_internal_headers(_headers);

      save();
      generate(_headers);
    }

    void header_file_environment::append(const data::cpp_code& s_)
    {
      const data::cpp_code new_line("\n");
      assert(_buffer.empty() || ends_with(_buffer, new_line));

      if (!s_.empty())
      {
        _buffer += s_;
        if (!ends_with(_buffer, new_line))
        {
          _buffer += new_line;
        }
      }

      save();
    }

    data::cpp_code header_file_environment::get() const
    {
      return data::cpp_code(
          _use_precompiled_headers ?
              std::string() : // The -include directive includes the header
              "#include <" + _env_filename.string() + ">\n");
    }

    data::cpp_code
    header_file_environment::get_appended(const data::cpp_code& s_) const
    {
      return get() + s_;
    }

    void header_file_environment::save()
    {
      const boost::filesystem::path fn =
          _headers.internal_dir() / _env_filename;
      if (!_internal_dir.empty())
      {
        const std::string filename = fn.string();
        std::ofstream f(filename.c_str());
        if (f)
        {
          f << _buffer;
        }
        else
        {
          throw data::exception("Error saving environment to " + filename);
        }
      }

      if (_use_precompiled_headers && _type_shell)
      {
        _type_shell->generate_precompiled_header(fn);
      }
    }

    const data::headers& header_file_environment::get_headers() const
    {
      return _headers;
    }

    data::cpp_code header_file_environment::get_all() const { return _buffer; }
  }
}
