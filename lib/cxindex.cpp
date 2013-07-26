// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxindex.hpp"

#include <clang-c/Index.h>

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/assign/list_of.hpp>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

#include <string>
#include <algorithm>
#include <iterator>

using namespace metashell;

namespace
{
  std::string seq_formatter(
    const std::string& name_,
    const std::string& limit_
  )
  {
    return
      "namespace impl "
      "{ " 
        "template <class C, class Item> "
        "struct " + name_ + "_builder;\n"

        "#ifdef METASHELL_TYPE_BUILDER\n"
          "#error METASHELL_TYPE_BUILDER already defined\n"
        "#endif\n"
        "#define METASHELL_TYPE_BUILDER(z, n, unused) "
          "template <"
            "BOOST_PP_ENUM_PARAMS(n, class T) BOOST_PP_COMMA_IF(n) class Item"
          "> "
          "struct "
            + name_ + "_builder<"
              "::boost::mpl::" + name_ + "<"
                "BOOST_PP_ENUM_PARAMS(n, T) "
                  "BOOST_PP_COMMA_IF("
                    "BOOST_PP_MUL(n, BOOST_PP_SUB(" + limit_ + ", n))"
                  ")"
                "BOOST_PP_ENUM("
                  "BOOST_PP_SUB(" + limit_ + ", n), "
                  "mpl_::na BOOST_PP_TUPLE_EAT(3), "
                  "~"
                ")"
              ">,"
              "Item"
            "> "
          "{ "
              "typedef "
                "::boost::mpl::" + name_ + "<"
                  "BOOST_PP_ENUM_PARAMS(n, T) BOOST_PP_COMMA_IF(n) Item"
                ">"
                "type; "
          "};"
          "\n"

        "BOOST_PP_REPEAT(" + limit_ + ", METASHELL_TYPE_BUILDER, ~)\n"

        "#undef METASHELL_TYPE_BUILDER\n"
      "} "

      "template <> "
      "struct format_impl<::boost::mpl::" + name_ + "<>::tag> "
      "{ "
        "typedef format_impl type; "

        "template <class V> "
        "struct apply : "
          "::boost::mpl::copy<"
            "V,"
            "::boost::mpl::inserter<"
              "::boost::mpl::" + name_ + "<>, "
              "::metashell::impl::" + name_ + "_builder<"
                "::boost::mpl::_1, "
                "::boost::mpl::_2"
              ">"
            ">"
          "> "
          "{};"
      "}; "
      ;
  }

  const std::string mpl_formatter =
    "#include <boost/mpl/copy.hpp>\n"
    "#include <boost/mpl/inserter.hpp>\n"
    "#include <boost/mpl/push_back.hpp>\n"

    "#include <boost/preprocessor/arithmetic/sub.hpp>\n"
    "#include <boost/preprocessor/arithmetic/mul.hpp>\n"
    "#include <boost/preprocessor/cat.hpp>\n"
    "#include <boost/preprocessor/punctuation/comma_if.hpp>\n"
    "#include <boost/preprocessor/repetition/enum.hpp>\n"
    "#include <boost/preprocessor/repetition/enum_params.hpp>\n"
    "#include <boost/preprocessor/repetition/repeat.hpp>\n"
    "#include <boost/preprocessor/tuple/eat.hpp>\n"

    "#include <boost/mpl/vector.hpp>\n"
    "#include <boost/mpl/list.hpp>\n"
    "#include <boost/mpl/set.hpp>\n"
    "#include <boost/mpl/map.hpp>\n"

    "namespace metashell "
    "{ "
      + seq_formatter("vector", "BOOST_MPL_LIMIT_VECTOR_SIZE") // covers: deque
      + seq_formatter("list", "BOOST_MPL_LIMIT_LIST_SIZE")
      + seq_formatter("set", "BOOST_MPL_LIMIT_SET_SIZE")
      + seq_formatter("map", "BOOST_MPL_LIMIT_MAP_SIZE")
      +
    "} "
    "\n";
}

cxindex::cxindex() : _index(clang_createIndex(0, 0)) {}

cxindex::~cxindex()
{
  clang_disposeIndex(_index);
}

boost::shared_ptr<cxtranslationunit> cxindex::parse_code(
  const std::string& src_,
  const config& config_
)
{
  using boost::bind;
  using boost::transform_iterator;
  using boost::function;
  using boost::assign::list_of;

  using boost::phoenix::val;
  using boost::phoenix::arg_names::arg1;

  using std::string;
  using std::vector;

  typedef
    transform_iterator<
      function<const char*(string)>,
      vector<string>::const_iterator
    >
    c_str_it;

  const char fn[] = "__metashell_source_file";
  const std::string internal_dir = "__metashell_internal";

  const std::string& mpl_formatter_fn = internal_dir + "/mpl_formatter.hpp";

  vector<string> args = list_of<string>
    ("-x")("c++")("-std=c++1y")("-I" + internal_dir);
  std::transform(
    config_.include_path.begin(),
    config_.include_path.end(),
    std::back_insert_iterator<vector<string> >(args),
    "-I" + arg1
  );

  const vector<const char*> argv(
    c_str_it(args.begin(), bind(&string::c_str, _1)),
    c_str_it(args.end())
  );

  CXUnsavedFile uf[2];
  uf[0].Filename = fn;
  uf[0].Contents = src_.c_str();
  uf[0].Length = src_.size();
  uf[1].Filename = mpl_formatter_fn.c_str();
  uf[1].Contents = mpl_formatter.c_str();
  uf[1].Length = mpl_formatter.size();
  return
    boost::make_shared<cxtranslationunit>(
      clang_parseTranslationUnit(
        _index,
        fn,
        &argv[0],
        argv.size(),
        uf,
        sizeof(uf) / sizeof(uf[0]),
        CXTranslationUnit_None 
      )
    );
}


