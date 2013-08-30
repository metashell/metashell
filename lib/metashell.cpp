// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metashell.hpp"
#include "get_type_of_variable.hpp"
#include "cxindex.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>

using namespace metashell;

namespace
{
  const char* prefix =
    "#define __METASHELL 1\n"

    "namespace metashell { "
      "namespace impl { "
        "template <class T> "
        "struct wrap {}; "

        "template <class T> "
        "typename T::tag tag_of(::metashell::impl::wrap<T>); "
        
        "void tag_of(...); "
      "} "
      
      "template <class Tag> "
      "struct format_impl "
      "{ "
        "typedef format_impl type; "
        
        "template <class T> "
        "struct apply { typedef T type; }; "
      "}; "

      "template <class T> "
      "struct format : "
        "::metashell::format_impl<"
          "decltype(::metashell::impl::tag_of(::metashell::impl::wrap<T>()))"
        ">::template apply<T>"
        "{}; "

      ""
    "}"
    "\n";

  const char* var = "__metashell_v";

  std::pair<boost::shared_ptr<cxtranslationunit>, std::string> parse_expr(
    cxindex& index_,
    const config& config_,
    const std::string& buffer_,
    const std::string& tmp_exp_
  )
  {
    const std::string code =
      prefix
      + append_to_buffer(
        buffer_,
        "::metashell::impl::wrap< " + tmp_exp_ + " > " + var + ";\n"
      );
    return std::make_pair(index_.parse_code(code, config_), code);
  }
}

result metashell::validate_code(const std::string& src_, const config& config_)
{
  const std::string code = prefix + src_;
  cxindex index;
  boost::shared_ptr<cxtranslationunit> tu = index.parse_code(code, config_);
  return
    result(
      "",
      tu->errors_begin(),
      tu->errors_end(),
      config_.verbose ? code : ""
    );
}

result metashell::eval_tmp(
  const std::string& buffer_,
  const std::string& tmp_exp_,
  const config& config_
)
{
  using std::string;
  using std::pair;

  typedef boost::shared_ptr<cxtranslationunit> tup;

  cxindex index;

  const pair<tup, string> simple =
    parse_expr(index, config_, buffer_, tmp_exp_);

  const pair<tup, string> final =
    simple.first->has_errors() ?
      simple :
      parse_expr(
        index,
        config_,
        buffer_,
        "::metashell::format<" + tmp_exp_ + ">::type"
      );

  get_type_of_variable v(var);
  final.first->visit_nodes(boost::ref(v));

  return
    result(
      v.result(),
      final.first->errors_begin(),
      final.first->errors_end(),
      config_.verbose ? final.second : ""
    );
}

std::string metashell::append_to_buffer(
  const std::string& buffer_,
  const std::string& s_
)
{
  return buffer_.empty() ? s_ : (buffer_ + '\n' + s_);
}


