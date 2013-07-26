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
    "namespace metashell { "
      "namespace impl { "
        "template <class T> "
        "typename T::tag tag_of(T*, T*); "
        
        "template <class T> "
        "void tag_of(T*, void*); "
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
          "decltype(::metashell::impl::tag_of((T*)0, (T*)0))"
        ">::template apply<T>"
        "{}; "

      ""
    "}"
    "\n";

  const char* var = "__metashell_v";

  boost::shared_ptr<cxtranslationunit> parse_expr(
    cxindex& index_,
    const config& config_,
    const std::string& buffer_,
    const std::string& tmp_exp_
  )
  {
    return
      index_.parse_code(
        prefix
        + append_to_buffer(
          buffer_,
          " typedef " + tmp_exp_ + " __metashell_t; "
          "__metashell_t* " + var + ";\n"
        ),
        config_
      );
  }
}

result metashell::validate_code(const std::string& src_, const config& config_)
{
  cxindex index;
  boost::shared_ptr<cxtranslationunit>
    tu = index.parse_code(prefix + src_, config_);
  return result("", tu->errors_begin(), tu->errors_end());
}

result metashell::eval_tmp(
  const std::string& buffer_,
  const std::string& tmp_exp_,
  const config& config_
)
{
  cxindex index;

  boost::shared_ptr<cxtranslationunit>
    simple = parse_expr(index, config_, buffer_, tmp_exp_);

  boost::shared_ptr<cxtranslationunit> final =
    simple->has_errors() ?
      simple :
      parse_expr(
        index,
        config_,
        buffer_,
        "::metashell::format<" + tmp_exp_ + ">::type"
      );

  get_type_of_variable v(var);
  final->visit_nodes(boost::ref(v));

  return result(v.result(), final->errors_begin(), final->errors_end());
}

std::string metashell::append_to_buffer(
  const std::string& buffer_,
  const std::string& s_
)
{
  return buffer_.empty() ? s_ : (buffer_ + '\n' + s_);
}


