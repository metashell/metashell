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

result metashell::validate_code(const std::string& src_, const config& config_)
{
  cxindex index;
  boost::shared_ptr<cxtranslationunit> tu = index.parse_code(src_, config_);
  return result("", tu->errors_begin(), tu->errors_end());
}

result metashell::eval_tmp(
  const std::string& buffer_,
  const std::string& tmp_exp_,
  const config& config_
)
{
  const char* var = "__metashell_v";

  cxindex index;

  boost::shared_ptr<cxtranslationunit> tu =
    index.parse_code(
      append_to_buffer(
        buffer_,
        " typedef " + tmp_exp_ + " __metashell_t; "
        "__metashell_t* " + var + ";\n"
      ),
      config_
    );

  get_type_of_variable v(var);
  tu->visit_nodes(boost::ref(v));

  return result(v.result(), tu->errors_begin(), tu->errors_end());
}

std::string metashell::append_to_buffer(
  const std::string& buffer_,
  const std::string& s_
)
{
  return buffer_.empty() ? s_ : (buffer_ + '\n' + s_);
}


