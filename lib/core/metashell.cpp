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
  const char* var = "__metashell_v";

  std::pair<std::unique_ptr<iface::cxtranslationunit>, std::string> parse_expr(
    iface::cxindex& index_,
    const std::string& input_filename_,
    const iface::environment& env_,
    const std::string& tmp_exp_
  )
  {
    using std::make_pair;

    const data::unsaved_file
      code(
        input_filename_,
        env_.get_appended(
          "::metashell::impl::wrap< " + tmp_exp_ + " > " + var + ";\n"
        )
      );
    return make_pair(index_.parse_code(code), code.content());
  }

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

  std::string unwrap(
    const std::string& prefix_,
    const std::string& s_,
    const std::string& suffix_
  )
  {
    if (
      s_.size() < prefix_.size()
      || std::string(s_.begin(), s_.begin() + prefix_.size()) != prefix_
    )
    {
      throw
        metashell::exception(
          "\"" + prefix_ + "\" is not a prefix of \"" + s_ + "\""
        );
    }
    else if (
      s_.size() < suffix_.size()
      || std::string(s_.end() - suffix_.size(), s_.end()) != suffix_
    )
    {
      throw
        metashell::exception(
          "\"" + suffix_ + "\" is not a suffix of \"" + s_ + "\""
        );
    }
    else
    {
      return
        std::string(s_.begin() + prefix_.size(), s_.end() - suffix_.size());
    }
  }

  std::function<void(const iface::cxcursor&)> get_type_of_variable(
    std::string name_,
    std::string& result_
  )
  {
    return
      [name_, &result_] (const iface::cxcursor& cursor_)
      {
        if (cursor_.variable_declaration() && cursor_.spelling() == name_)
        {
          try
          {
            result_ =
              unwrap(
                "wrap<",
                cursor_.type()->canonical_type()->spelling(),
                ">"
              );
          }
          catch (const exception&)
          {
            return;
          }
          boost::algorithm::trim(result_);
        }
      };
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

result metashell::eval_tmp_formatted(
  const iface::environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  const std::string& input_filename_,
  logger* logger_,
  iface::libclang& libclang_
)
{
  using std::string;
  using std::pair;

  typedef std::unique_ptr<iface::cxtranslationunit> tup;

  METASHELL_LOG(
    logger_,
    "Checking if metaprogram can be evaluated without metashell::format: "
    + tmp_exp_
  );

  std::unique_ptr<iface::cxindex> index = libclang_.create_index(env_, logger_);

  pair<tup, string>
    simple = parse_expr(*index, input_filename_, env_, tmp_exp_);

  std::string error_string = simple.first->get_error_string();
  METASHELL_LOG(
    logger_,
    !error_string.empty() ?
      "Errors occured during metaprogram evaluation. Displaying errors coming"
      " from the metaprogram without metashell::format" :
      "No errors occured during metaprogram evaluation. Re-evaluating it with"
      " metashell::format"
  );

  const pair<tup, string> final_pair =
    !error_string.empty() ?
      std::move(simple) :
      parse_expr(
        *index,
        input_filename_,
        env_,
        "::metashell::format<" + tmp_exp_ + ">::type"
      );

  std::string v;
  final_pair.first->visit_nodes(get_type_of_variable(var, v));

  error_string = final_pair.first->get_error_string();

  return
    result(error_string.empty(),
      v,
      error_string,
      config_.verbose ? final_pair.second : ""
    );
}

result metashell::eval_tmp(
  const iface::environment& env_,
  const std::string& tmp_exp_,
  const config& config_,
  logger* logger_)
{
  //lot of hacking and duplication just to make things work. TODO refactor

  auto clang_args = env_.clang_arguments();
  clang_args.push_back("-"); //Compile from stdin

  const just::process::output output =
    clang_binary(config_.clang_path, logger_).run(
        clang_args,
        env_.get_appended(
          "::metashell::impl::wrap< " + tmp_exp_ + " > " + var + ";\n"));

  if (output.exit_code() != 0) {
    return result{false, "", output.standard_error(), ""};
  }

  const std::string& standard_output = output.standard_output();
  std::string last_line = standard_output.substr(
      standard_output.find_last_of('\n', standard_output.size() - 2) + 1);

  boost::regex reg(
      ".*':'struct metashell::impl::wrap<(?:class |struct |union )?(.*)>' "
      "'void \\(void\\) noexcept'.*");

  boost::smatch match;
  if (!boost::regex_match(last_line, match, reg)) {
    throw exception("Unexpected ast format");
  }

  return result{true, boost::trim_copy(std::string(match[1])), "", ""};
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
        return has_typedef(begin_, end_);
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

