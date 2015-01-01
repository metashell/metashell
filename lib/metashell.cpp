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

#include <metashell/metashell.hpp>
#include "get_type_of_variable.hpp"
#include "cxindex.hpp"

#include <metashell/command.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <fstream>
#include <memory>

using namespace metashell;

namespace
{
  const char* var = "__metashell_v";

  std::pair<std::unique_ptr<cxtranslationunit>, std::string> parse_expr(
    cxindex& index_,
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
    return make_pair(index_.parse_code(code, env_), code.content());
  }

  bool has_typedef(
    const command::iterator& begin_,
    const command::iterator& end_
  )
  {
    return
      std::find_if(
        begin_,
        end_,
        [](const token& t_) { return t_.type() == token_type::keyword_typedef; }
      ) != end_;
  }

  token_type last_non_whitespace_token_type(
    command::iterator begin_,
    const command::iterator& end_
  )
  {
    token_type t;
    for (; begin_ != end_; ++begin_)
    {
      const token_category c = begin_->category();
      if (c != token_category::whitespace && c != token_category::comment)
      {
        t = begin_->type();
      }
    }
    return t;
  }
}

result metashell::validate_code(
  const std::string& src_,
  const config& config_,
  const environment& env_,
  const std::string& input_filename_,
  logger* logger_
)
{
  METASHELL_LOG(logger_, "Validating code " + src_);

  try
  {
    const unsaved_file src(input_filename_, env_.get_appended(src_));
    cxindex index(logger_);
    std::unique_ptr<cxtranslationunit> tu = index.parse_code(src, env_);
    return
      result(
        "",
        tu->errors_begin(),
        tu->errors_end(),
        config_.verbose ? src.content() : ""
      );
  }
  catch (const std::exception& e)
  {
    const std::string es[] = { e.what() };
    return result("", es, es + sizeof(es) / sizeof(es[0]), "");
  }
}

result metashell::eval_tmp_formatted(
  const environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  const std::string& input_filename_,
  logger* logger_
)
{
  using std::string;
  using std::pair;

  typedef std::unique_ptr<cxtranslationunit> tup;

  METASHELL_LOG(
    logger_,
    "Checking if metaprogram can be evaluated without metashell::format: "
    + tmp_exp_
  );

  cxindex index(logger_);

  pair<tup, string> simple = parse_expr(index, input_filename_, env_, tmp_exp_);

  METASHELL_LOG(
    logger_,
    simple.first->has_errors() ?
      "Errors occured during metaprogram evaluation. Displaying errors coming"
      " from the metaprogram without metashell::format" :
      "No errors occured during metaprogram evaluation. Re-evaluating it with"
      " metashell::format"
  );

  const pair<tup, string> final_pair =
    simple.first->has_errors() ?
      std::move(simple) :
      parse_expr(
        index,
        input_filename_,
        env_,
        "::metashell::format<" + tmp_exp_ + ">::type"
      );

  get_type_of_variable v(var);
  final_pair.first->visit_nodes(
    [&v](cxcursor cursor_, cxcursor) { v(cursor_); }
  );

  return
    result(
      v.result(),
      final_pair.first->errors_begin(),
      final_pair.first->errors_end(),
      config_.verbose ? final_pair.second : ""
    );
}

result metashell::eval_tmp_unformatted(
  const environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  const std::string& input_filename_,
  logger* logger_
)
{
  using std::string;
  using std::pair;

  typedef std::unique_ptr<cxtranslationunit> tup;

  METASHELL_LOG(
    logger_,
    "Evaluating template metaprogram without metashell:format: " + tmp_exp_
  );

  cxindex index(logger_);

  pair<tup, string> final_pair =
    parse_expr(index, input_filename_, env_, tmp_exp_);

  get_type_of_variable v(var);
  final_pair.first->visit_nodes(
    [&v](cxcursor cursor_, cxcursor) { v(cursor_); }
  );

  return
    result(
      v.result(),
      final_pair.first->errors_begin(),
      final_pair.first->errors_end(),
      config_.verbose ? final_pair.second : ""
    );
}


namespace
{
  std::pair<std::string, std::string> find_completion_start(
    const std::string& s_
  )
  {
    typedef std::pair<std::string, std::string> string_pair;

    const command cmd(s_);

    std::ostringstream o;
    token last_token;
    bool first = true;
    for (auto i = cmd.begin(), e = cmd.end(); i != e; ++i)
    {
      if (first)
      {
        first = false;
      }
      else
      {
        o << last_token.value();
      }
      last_token = *i;
    }

    if (first) // no token
    {
      return string_pair("", "");
    }
    else
    {
      if (
        last_token.category() == token_category::identifier
        || last_token.category() == token_category::keyword
      )
      {
        return string_pair(o.str(), last_token.value());
      }
      else
      {
        o << last_token.value();
        return string_pair(o.str(), "");
      }
    }
  }
}

void metashell::code_complete(
  const environment& env_,
  const std::string& src_,
  const std::string& input_filename_,
  std::set<std::string>& out_,
  logger* logger_
)
{
  using boost::starts_with;

  using std::pair;
  using std::string;
  using std::set;

  METASHELL_LOG(logger_, "Code completion of " + src_);

  const pair<string, string> completion_start = find_completion_start(src_);

  const unsaved_file src(
    input_filename_,
    // code completion doesn't seem to work without that extra space at the end
    env_.get_appended(completion_start.first + " ")
  );

  set<string> c;
  cxindex(logger_).parse_code(src, env_)->code_complete(c);

  out_.clear();
  const int prefix_len = completion_start.second.length();
  for (const string& s : c)
  {
    if (starts_with(s, completion_start.second) && s != completion_start.second)
    {
      out_.insert(string(s.begin() + prefix_len, s.end()));
    }
  }
}

bool metashell::is_environment_setup_command(
  command::iterator begin_,
  const command::iterator& end_
)
{
  try
  {
    begin_ = skip_whitespace(begin_, end_);

    if (begin_ == end_)
    {
      // empty input is not a query
      return true;
    }
    else
    {
      const token t = *begin_;
      switch (t.category())
      {
      case token_category::keyword:
        switch (t.type())
        {
        case token_type::keyword_bool:
        case token_type::keyword_char:
        case token_type::keyword_const:
        case token_type::keyword_double:
        case token_type::keyword_float:
        case token_type::keyword_int:
        case token_type::keyword_long:
        case token_type::keyword_short:
        case token_type::keyword_signed:
        case token_type::keyword_unsigned:
        case token_type::keyword_void:
        case token_type::keyword_volatile:
        case token_type::keyword_wchar_t:
          if (has_typedef(begin_, end_))
          {
            return true;
          }
          else
          {
            const token_type lt = last_non_whitespace_token_type(begin_, end_);
            return
              lt == token_type::operator_semicolon
              || lt == token_type::operator_right_brace;
          }
        case token_type::keyword_sizeof:
        case token_type::keyword_const_cast:
        case token_type::keyword_static_cast:
        case token_type::keyword_dynamic_cast:
        case token_type::keyword_reinterpret_cast:
          return false;
        default:
          return true;
        }
        assert(false);
      case token_category::identifier:
        return has_typedef(begin_, end_);
      case token_category::preprocessor:
        return true;
      default:
        return false;
      }
    }
  }
  catch (...)
  {
    return false;
  }
}

