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

#include <metashell/data/command.hpp>
#include <metashell/exception.hpp>
#include <metashell/clang_binary.hpp>

#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <fstream>
#include <memory>

using namespace metashell;

namespace
{
  bool has_typedef(
    const data::command::iterator& begin_,
    const data::command::iterator& end_
  )
  {
    return
      std::find_if(
        begin_,
        end_,
        [](const data::token& t_)
        {
          return t_.type() == data::token_type::keyword_typedef;
        }
      ) != end_;
  }

  data::token_type last_non_whitespace_token_type(
    data::command::iterator begin_,
    const data::command::iterator& end_
  )
  {
    data::token_type t;
    for (; begin_ != end_; ++begin_)
    {
      const data::token_category c = begin_->category();
      if (
        c != data::token_category::whitespace
        && c != data::token_category::comment
      )
      {
        t = begin_->type();
      }
    }
    return t;
  }

}

std::string metashell::repair_type_string(const std::string& type) {
  boost::regex bool_regex(
      "(^|[^A-Za-z0-9_])_Bool([^A-Za-z0-9_]|$)");
  boost::regex type_regex(
      "(^|[^A-Za-z0-9_])(?:class |struct |union |enum )");

  auto tmp = boost::regex_replace(type, bool_regex, "\\1bool\\2",
      boost::match_default | boost::format_all);

  return boost::regex_replace(tmp, type_regex, "\\1",
      boost::match_default | boost::format_all);
}

std::string metashell::get_type_from_ast_string(const std::string& ast) {
  // This algorithm is very ugly, but it basically iterates on the
  // lines of the ast dump from the end until it finds the interesting line.

  std::size_t end_index = std::string::npos;
  std::size_t start_index = ast.find_last_of('\n');

  boost::regex reg(
    ".*':'struct metashell::impl::wrap<?(.*)>' "
    "'void \\(void\\)"
    "(?: __attribute__\\(\\(thiscall\\)\\)|(?:))(?: noexcept|(?:))'.*");

  std::string line;
  while (true) {
    end_index = start_index;
    start_index = ast.find_last_of('\n', end_index - 1);

    if (start_index == std::string::npos || end_index == std::string::npos) {
      throw exception("No suitable ast line in dump");
    }

    line = ast.substr(start_index + 1, end_index-start_index-1);

    boost::smatch match;
    if (boost::regex_match(line, match, reg)) {
      return repair_type_string(boost::trim_copy(std::string(match[1])));
    }
  }
}

result metashell::validate_code(
  const std::string& src_,
  const config& config_,
  const iface::environment& env_,
  const std::string& input_filename_,
  logger* logger_,
  iface::libclang& libclang_
)
{
  METASHELL_LOG(logger_, "Validating code " + src_);

  try
  {
    const data::unsaved_file src(input_filename_, env_.get_appended(src_));
    std::unique_ptr<iface::cxindex>
      index = libclang_.create_index(env_, logger_);
    std::unique_ptr<iface::cxtranslationunit> tu = index->parse_code(src);

    const std::string error_string = tu->get_error_string();
    return
      result(error_string.empty(),
        "",
        error_string,
        config_.verbose ? src.content() : ""
      );
  }
  catch (const std::exception& e)
  {
    return result(false, "", e.what(), "");
  }
}

result metashell::run_clang(
  const std::string& clang_path_,
  std::vector<std::string> clang_args_,
  const std::string& input_,
  logger* logger_
)
{
  clang_args_.push_back("-"); //Compile from stdin

  const just::process::output output =
    clang_binary(clang_path_, logger_).run(clang_args_, input_);

  if (output.exit_code() != 0) {
    return result{false, "", output.standard_error(), ""};
  }

  return result{
    true, get_type_from_ast_string(output.standard_output()), "", ""};
}

result metashell::eval_tmp_formatted(
  const iface::environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  logger* logger_
)
{
  using std::string;
  using std::pair;

  METASHELL_LOG(
    logger_,
    "Checking if metaprogram can be evaluated without metashell::format: "
    + tmp_exp_
  );

  result simple = eval_tmp(env_, tmp_exp_, config_, logger_);

  METASHELL_LOG(
    logger_,
    !simple.successful ?
      "Errors occured during metaprogram evaluation. Displaying errors coming"
      " from the metaprogram without metashell::format" :
      "No errors occured during metaprogram evaluation. Re-evaluating it with"
      " metashell::format"
  );

  result final_result = !simple.successful ? std::move(simple) :
    eval_tmp(
      env_,
      "::metashell::format<" + tmp_exp_ + ">::type",
      config_,
      logger_);

  return final_result;
}

result metashell::eval_tmp(
  const iface::environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  logger* logger_)
{
  return run_clang(
    config_.clang_path, env_.clang_arguments(),
    env_.get_appended(
      "::metashell::impl::wrap< " + tmp_exp_ + " > __metashell_v;\n"),
    logger_);
}

namespace
{
  std::pair<std::string, std::string> find_completion_start(
    const std::string& s_
  )
  {
    typedef std::pair<std::string, std::string> string_pair;

    const data::command cmd(s_);

    std::ostringstream o;
    data::token last_token;
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
        last_token.category() == data::token_category::identifier
        || last_token.category() == data::token_category::keyword
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
  const iface::environment& env_,
  const std::string& src_,
  const std::string& input_filename_,
  std::set<std::string>& out_,
  logger* logger_,
  iface::libclang& libclang_
)
{
  using boost::starts_with;

  using std::pair;
  using std::string;
  using std::set;

  METASHELL_LOG(logger_, "Code completion of " + src_);

  const pair<string, string> completion_start = find_completion_start(src_);

  const data::unsaved_file src(
    input_filename_,
    // code completion doesn't seem to work without that extra space at the end
    env_.get_appended(completion_start.first + " ")
  );

  set<string> c;
  libclang_.create_index(env_, logger_)->parse_code(src)->code_complete(c);

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
  data::command::iterator begin_,
  const data::command::iterator& end_
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
      const data::token t = *begin_;
      switch (t.category())
      {
      case data::token_category::keyword:
        switch (t.type())
        {
        case data::token_type::keyword_bool:
        case data::token_type::keyword_char:
        case data::token_type::keyword_const:
        case data::token_type::keyword_double:
        case data::token_type::keyword_float:
        case data::token_type::keyword_int:
        case data::token_type::keyword_long:
        case data::token_type::keyword_short:
        case data::token_type::keyword_signed:
        case data::token_type::keyword_unsigned:
        case data::token_type::keyword_void:
        case data::token_type::keyword_volatile:
        case data::token_type::keyword_wchar_t:
          if (has_typedef(begin_, end_))
          {
            return true;
          }
          else
          {
            const data::token_type
              lt = last_non_whitespace_token_type(begin_, end_);
            return
              lt == data::token_type::operator_semicolon
              || lt == data::token_type::operator_right_brace;
          }
        case data::token_type::keyword_sizeof:
        case data::token_type::keyword_const_cast:
        case data::token_type::keyword_static_cast:
        case data::token_type::keyword_dynamic_cast:
        case data::token_type::keyword_reinterpret_cast:
          return false;
        default:
          return true;
        }
        assert(false);
      case data::token_category::identifier:
        return
          has_typedef(begin_, end_) ||
            last_non_whitespace_token_type(begin_, end_)
            == data::token_type::operator_semicolon;
      case data::token_category::preprocessor:
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

