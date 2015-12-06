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

#include <metashell/header_file_environment.hpp>
#include <metashell/data/headers.hpp>
#include <metashell/config.hpp>
#include <metashell/clang_binary.hpp>
#include <metashell/exception.hpp>
#include <metashell/unsaved_file.hpp>
#include <metashell/headers.hpp>
#include <metashell/path_builder.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/join.hpp>

#include <boost/range/adaptors.hpp>

#include <fstream>
#include <vector>

using namespace metashell;

namespace
{
  const char env_fn[] = "metashell_environment.hpp";

  void extend_to_find_headers_in_local_dir(std::vector<std::string>& v_)
  {
    v_.push_back("-iquote");
    v_.push_back(".");
  }

  void precompile(
    const std::string& clang_path_,
    const std::vector<std::string>& clang_args_,
    const std::string& fn_,
    logger* logger_
  )
  {
    using boost::algorithm::trim_copy;

    METASHELL_LOG(logger_, "Generating percompiled header for " + fn_);

    std::vector<std::string> args(clang_args_);
    extend_to_find_headers_in_local_dir(args);
    args.push_back("-w");
    args.push_back("-o");
    args.push_back(fn_ + ".pch");
    args.push_back(fn_);

    const data::process_output
      o = clang_binary(clang_path_, logger_).run(args, "");
    const std::string err = o.standard_output() + o.standard_error();
    if (
      !err.empty()
      // clang displays this even when "-w" is used. This can be ignored
      && trim_copy(err) !=
        "warning: precompiled header used __DATE__ or __TIME__."
    )
    {
      throw exception("Error precompiling header " + fn_ + ": " + err);
    }
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

    headers_.add(
      internal_dir / "metashell" / "instantiate_expression.hpp",
      "namespace metashell\n"
      "{\n"
      "  template <bool> struct expression_instantiated;\n"
      "}\n"

      "#define METASHELL_INSTANTIATE_EXPRESSION(...) \\\n"
      "  ::metashell::expression_instantiated<true ? true : ((__VA_ARGS__), false)>\n"
    );
  }

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

  std::vector<std::string> base_clang_args(
    const data::config& config_,
    const std::string& internal_dir_
  )
  {
    std::vector<std::string>
      clang_args{
        "-x",
        "c++-header",
        clang_argument(config_.standard_to_use),
        "-I" + internal_dir_,
        set_max_template_depth(config_.max_template_depth)
      };

    add_with_prefix("-I", config_.include_path, clang_args);
    add_with_prefix("-D", config_.macros, clang_args);

    if (!config_.warnings_enabled)
    {
      clang_args.push_back("-w");
    }

    clang_args.insert(
      clang_args.end(),
      config_.extra_clang_args.begin(),
      config_.extra_clang_args.end()
    );

    return clang_args;
  }
}

header_file_environment::header_file_environment(
  const data::config& config_,
  logger* logger_
) :
  _dir(),
  _buffer(),
  _base_clang_args(base_clang_args(config_, _dir.path())),
  _clang_args(_base_clang_args),
  _headers(_dir.path()),
  _use_precompiled_headers(config_.use_precompiled_headers),
  _clang_path(config_.clang_path),
  _logger(logger_)
{
  add_internal_headers(_headers);

  if (_use_precompiled_headers)
  {
    _clang_args.push_back("-include");
    _clang_args.push_back(env_filename());
  }

  _clang_args.push_back("-Xclang");
  _clang_args.push_back("-ast-dump");

  _clang_args.push_back("-Wfatal-errors");

  extend_to_find_headers_in_local_dir(_clang_args);

  save();
  generate(_headers);
}

void header_file_environment::append(const std::string& s_)
{
  if (_buffer.empty())
  {
    _buffer = s_;
  }
  else
  {
    _buffer += '\n' + s_;
  }

  save();
}

std::string header_file_environment::get() const
{
  return
    _use_precompiled_headers ?
      std::string() : // The -include directive includes the header
      "#include <" + std::string(env_fn) + ">\n";
}

std::string header_file_environment::get_appended(const std::string& s_) const
{
  return get() + s_;
}

std::vector<std::string>& header_file_environment::clang_arguments()
{
  return _clang_args;
}

const std::vector<std::string>&
  header_file_environment::clang_arguments() const
{
  return _clang_args;
}

std::string header_file_environment::env_filename() const
{
  return _headers.internal_dir() + "/" + env_fn;
}

void header_file_environment::save()
{
  const std::string fn = env_filename();
  {
    std::ofstream f(fn.c_str());
    if (f)
    {
      f << _buffer;
    }
    else
    {
      throw exception("Error saving environment to " + fn);
    }
  }

  if (_use_precompiled_headers)
  {
    precompile(_clang_path, _base_clang_args, fn, _logger);
  }
}

std::string header_file_environment::internal_dir() const
{
  return _dir.path();
}

const data::headers& header_file_environment::get_headers() const
{
  return _headers;
}

std::string header_file_environment::get_all() const
{
  return _buffer;
}

