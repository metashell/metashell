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

#include "metashell.hpp"
#include "get_type_of_variable.hpp"
#include "cxindex.hpp"

#include <metashell/token_iterator.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/foreach.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <fstream>

using namespace metashell;

namespace
{
  const char* var = "__metashell_v";

  std::pair<boost::shared_ptr<cxtranslationunit>, std::string> parse_expr(
    cxindex& index_,
    const config& config_,
    const std::string& input_filename_,
    const environment& env_,
    const std::string& tmp_exp_
  )
  {
    using std::make_pair;

    const unsaved_file
      code(
        input_filename_,
        env_.get_appended(
          "::metashell::impl::wrap< " + tmp_exp_ + " > " + var + ";\n"
        )
      );
    return make_pair(index_.parse_code(code, config_, env_), code.content());
  }
}

result metashell::validate_code(
  const std::string& src_,
  const config& config_,
  const environment& env_,
  const std::string& input_filename_
)
{
  const unsaved_file src(input_filename_, env_.get_appended(src_));
  cxindex index;
  boost::shared_ptr<cxtranslationunit>
    tu = index.parse_code(src, config_, env_);
  return
    result(
      "",
      tu->errors_begin(),
      tu->errors_end(),
      config_.verbose ? src.content() : ""
    );
}

result metashell::eval_tmp(
  const environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  const std::string& input_filename_
)
{
  using std::string;
  using std::pair;

  typedef boost::shared_ptr<cxtranslationunit> tup;

  cxindex index;

  const pair<tup, string> simple =
    parse_expr(index, config_, input_filename_, env_, tmp_exp_);

  const pair<tup, string> final =
    simple.first->has_errors() ?
      simple :
      parse_expr(
        index,
        config_,
        input_filename_,
        env_,
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

namespace
{
  std::pair<std::string, std::string> find_completion_start(
    const std::string& s_,
    const std::string& input_filename_
  )
  {
    typedef std::pair<std::string, std::string> string_pair;

    std::ostringstream o;
    token_iterator::value_type last_token;
    bool first = true;
    for (token_iterator i = begin_tokens(s_, input_filename_), e; i != e; ++i)
    {
      if (!IS_CATEGORY(*i, boost::wave::EOFTokenType))
      {
        if (first)
        {
          first = false;
        }
        else
        {
          o << last_token.get_value();
        }
        last_token = *i;
      }
    }

    if (first) // no token
    {
      return string_pair("", "");
    }
    else
    {
      if (
        IS_CATEGORY(last_token, boost::wave::IdentifierTokenType)
        || IS_CATEGORY(last_token, boost::wave::KeywordTokenType)
      )
      {
        const token_iterator::value_type::string_type
          t = last_token.get_value();
        return string_pair(o.str(), std::string(t.begin(), t.end()));
      }
      else
      {
        o << last_token.get_value();
        return string_pair(o.str(), "");
      }
    }
  }
}

void metashell::code_complete(
  const environment& env_,
  const std::string& src_,
  const config& config_,
  const std::string& input_filename_,
  std::set<std::string>& out_
)
{
  using boost::starts_with;

  using std::pair;
  using std::string;
  using std::set;

  const pair<string, string> completion_start =
    find_completion_start(src_, input_filename_);

  const unsaved_file src(
    input_filename_,
    // code completion doesn't seem to work without that extra space at the end
    env_.get_appended(completion_start.first + " ")
  );

  set<string> c;
  cxindex().parse_code(src, config_, env_)->code_complete(c);
  
  out_.clear();
  const int prefix_len = completion_start.second.length();
  BOOST_FOREACH(const string& s, c)
  {
    if (starts_with(s, completion_start.second) && s != completion_start.second)
    {
      out_.insert(string(s.begin() + prefix_len, s.end()));
    }
  }
}

bool metashell::file_exists(const std::string& fn_)
{
  std::ifstream f(fn_.c_str());
  return f;
}

