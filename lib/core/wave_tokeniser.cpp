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

// Disable warnings coming from Boost.Wave
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include <metashell/wave_tokeniser.hpp>

#include <boost/wave.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp>

#include <cassert>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace metashell;

namespace
{
  template <char C>
  std::string remove(const std::string& s_)
  {
    std::vector<char> v;
    v.reserve(s_.size());
    std::copy_if(s_.begin(), s_.end(), std::back_inserter(v), [](char c_)
                 {
                   return c_ != C;
                 });
    return std::string(v.begin(), v.end());
  }

  template <class Token>
  data::token_type type_of_token(const Token& t_)
  {
    switch (t_)
    {
    case boost::wave::T_IDENTIFIER:
      return t_.get_value() == "constexpr" ?
                 data::token_type::keyword_constexpr :
                 data::token_type::identifier;
    case boost::wave::T_CHARLIT:
      return data::token_type::character_literal;
    case boost::wave::T_FLOATLIT:
      return data::token_type::floating_literal;
    case boost::wave::T_OCTALINT:
    case boost::wave::T_DECIMALINT:
    case boost::wave::T_HEXAINT:
    case boost::wave::T_LONGINTLIT:
    case boost::wave::T_INTLIT:
      return data::token_type::integer_literal;
    case boost::wave::T_STRINGLIT:
      return data::token_type::string_literal;
    case boost::wave::T_TRUE:
    case boost::wave::T_FALSE:
      return data::token_type::bool_literal;
    case boost::wave::T_CCOMMENT:
      return data::token_type::c_comment;
    case boost::wave::T_CPPCOMMENT:
      return data::token_type::cpp_comment;
    case boost::wave::T_SPACE:
    case boost::wave::T_SPACE2:
      return data::token_type::whitespace;
    case boost::wave::T_CONTLINE:
      return data::token_type::continue_line;
    case boost::wave::T_NEWLINE:
      return data::token_type::new_line;
    case boost::wave::T_ASM:
      return data::token_type::keyword_asm;
    case boost::wave::T_AUTO:
      return data::token_type::keyword_auto;
    case boost::wave::T_BOOL:
      return data::token_type::keyword_bool;
    case boost::wave::T_BREAK:
      return data::token_type::keyword_break;
    case boost::wave::T_CASE:
      return data::token_type::keyword_case;
    case boost::wave::T_CATCH:
      return data::token_type::keyword_catch;
    case boost::wave::T_CHAR:
      return data::token_type::keyword_char;
    case boost::wave::T_CLASS:
      return data::token_type::keyword_class;
    case boost::wave::T_CONST:
      return data::token_type::keyword_const;
    case boost::wave::T_CONSTCAST:
      return data::token_type::keyword_const_cast;
    case boost::wave::T_CONTINUE:
      return data::token_type::keyword_continue;
    case boost::wave::T_DEFAULT:
      return data::token_type::keyword_default;
    case boost::wave::T_DELETE:
      return data::token_type::keyword_delete;
    case boost::wave::T_DO:
      return data::token_type::keyword_do;
    case boost::wave::T_DOUBLE:
      return data::token_type::keyword_double;
    case boost::wave::T_DYNAMICCAST:
      return data::token_type::keyword_dynamic_cast;
    case boost::wave::T_ELSE:
      return data::token_type::keyword_else;
    case boost::wave::T_ENUM:
      return data::token_type::keyword_enum;
    case boost::wave::T_EXPLICIT:
      return data::token_type::keyword_explicit;
    case boost::wave::T_EXPORT:
      return data::token_type::keyword_export;
    case boost::wave::T_EXTERN:
      return data::token_type::keyword_extern;
    case boost::wave::T_FLOAT:
      return data::token_type::keyword_float;
    case boost::wave::T_FOR:
      return data::token_type::keyword_for;
    case boost::wave::T_FRIEND:
      return data::token_type::keyword_friend;
    case boost::wave::T_GOTO:
      return data::token_type::keyword_goto;
    case boost::wave::T_IF:
      return data::token_type::keyword_if;
    case boost::wave::T_INLINE:
      return data::token_type::keyword_inline;
    case boost::wave::T_INT:
      return data::token_type::keyword_int;
    case boost::wave::T_LONG:
      return data::token_type::keyword_long;
    case boost::wave::T_MUTABLE:
      return data::token_type::keyword_mutable;
    case boost::wave::T_NAMESPACE:
      return data::token_type::keyword_namespace;
    case boost::wave::T_NEW:
      return data::token_type::keyword_new;
    case boost::wave::T_OPERATOR:
      return data::token_type::keyword_operator;
    case boost::wave::T_PRIVATE:
      return data::token_type::keyword_private;
    case boost::wave::T_PROTECTED:
      return data::token_type::keyword_protected;
    case boost::wave::T_PUBLIC:
      return data::token_type::keyword_public;
    case boost::wave::T_REGISTER:
      return data::token_type::keyword_register;
    case boost::wave::T_REINTERPRETCAST:
      return data::token_type::keyword_reinterpret_cast;
    case boost::wave::T_RETURN:
      return data::token_type::keyword_return;
    case boost::wave::T_SHORT:
      return data::token_type::keyword_short;
    case boost::wave::T_SIGNED:
      return data::token_type::keyword_signed;
    case boost::wave::T_SIZEOF:
      return data::token_type::keyword_sizeof;
    case boost::wave::T_STATIC:
      return data::token_type::keyword_static;
    case boost::wave::T_STATICCAST:
      return data::token_type::keyword_static_cast;
    case boost::wave::T_STRUCT:
      return data::token_type::keyword_struct;
    case boost::wave::T_SWITCH:
      return data::token_type::keyword_switch;
    case boost::wave::T_TEMPLATE:
      return data::token_type::keyword_template;
    case boost::wave::T_THIS:
      return data::token_type::keyword_this;
    case boost::wave::T_THROW:
      return data::token_type::keyword_throw;
    case boost::wave::T_TRY:
      return data::token_type::keyword_try;
    case boost::wave::T_TYPEDEF:
      return data::token_type::keyword_typedef;
    case boost::wave::T_TYPEID:
      return data::token_type::keyword_typeid;
    case boost::wave::T_TYPENAME:
      return data::token_type::keyword_typename;
    case boost::wave::T_UNION:
      return data::token_type::keyword_union;
    case boost::wave::T_UNSIGNED:
      return data::token_type::keyword_unsigned;
    case boost::wave::T_USING:
      return data::token_type::keyword_using;
    case boost::wave::T_VIRTUAL:
      return data::token_type::keyword_virtual;
    case boost::wave::T_VOID:
      return data::token_type::keyword_void;
    case boost::wave::T_VOLATILE:
      return data::token_type::keyword_volatile;
    case boost::wave::T_WCHART:
      return data::token_type::keyword_wchar_t;
    case boost::wave::T_WHILE:
      return data::token_type::keyword_while;
    case boost::wave::T_AND:
    case boost::wave::T_AND_ALT:
      return data::token_type::operator_bitwise_and;
    case boost::wave::T_ANDAND:
    case boost::wave::T_ANDAND_ALT:
      return data::token_type::operator_logical_and;
    case boost::wave::T_ASSIGN:
      return data::token_type::operator_assign;
    case boost::wave::T_ANDASSIGN:
    case boost::wave::T_ANDASSIGN_ALT:
      return data::token_type::operator_bitwise_and_assign;
    case boost::wave::T_OR:
    case boost::wave::T_OR_ALT:
      return data::token_type::operator_bitwise_or;
    case boost::wave::T_ORASSIGN:
    case boost::wave::T_ORASSIGN_ALT:
      return data::token_type::operator_bitwise_or_assign;
    case boost::wave::T_XOR:
    case boost::wave::T_XOR_ALT:
      return data::token_type::operator_bitwise_xor;
    case boost::wave::T_XORASSIGN:
    case boost::wave::T_XORASSIGN_ALT:
      return data::token_type::operator_bitwise_xor_assign;
    case boost::wave::T_COMMA:
      return data::token_type::operator_comma;
    case boost::wave::T_COLON:
      return data::token_type::operator_colon;
    case boost::wave::T_DIVIDE:
      return data::token_type::operator_divide;
    case boost::wave::T_DIVIDEASSIGN:
      return data::token_type::operator_divide_assign;
    case boost::wave::T_DOT:
      return data::token_type::operator_dot;
    case boost::wave::T_DOTSTAR:
      return data::token_type::operator_dotstar;
    case boost::wave::T_ELLIPSIS:
      return data::token_type::operator_ellipsis;
    case boost::wave::T_EQUAL:
      return data::token_type::operator_equal;
    case boost::wave::T_GREATER:
      return data::token_type::operator_greater;
    case boost::wave::T_GREATEREQUAL:
      return data::token_type::operator_greater_equal;
    case boost::wave::T_LEFTBRACE:
    case boost::wave::T_LEFTBRACE_ALT:
    case boost::wave::T_LEFTBRACE_TRIGRAPH:
      return data::token_type::operator_left_brace;
    case boost::wave::T_LESS:
      return data::token_type::operator_less;
    case boost::wave::T_LESSEQUAL:
      return data::token_type::operator_less_equal;
    case boost::wave::T_LEFTPAREN:
      return data::token_type::operator_left_paren;
    case boost::wave::T_LEFTBRACKET:
    case boost::wave::T_LEFTBRACKET_ALT:
    case boost::wave::T_LEFTBRACKET_TRIGRAPH:
      return data::token_type::operator_left_bracket;
    case boost::wave::T_MINUS:
      return data::token_type::operator_minus;
    case boost::wave::T_MINUSASSIGN:
      return data::token_type::operator_minus_assign;
    case boost::wave::T_MINUSMINUS:
      return data::token_type::operator_minus_minus;
    case boost::wave::T_PERCENT:
      return data::token_type::operator_modulo;
    case boost::wave::T_PERCENTASSIGN:
      return data::token_type::operator_modulo_assign;
    case boost::wave::T_NOT:
    case boost::wave::T_NOT_ALT:
      return data::token_type::operator_logical_not;
    case boost::wave::T_NOTEQUAL:
    case boost::wave::T_NOTEQUAL_ALT:
      return data::token_type::operator_not_equal;
    case boost::wave::T_OROR:
    case boost::wave::T_OROR_ALT:
      return data::token_type::operator_logical_or;
    case boost::wave::T_PLUS:
      return data::token_type::operator_plus;
    case boost::wave::T_PLUSASSIGN:
      return data::token_type::operator_plus_assign;
    case boost::wave::T_PLUSPLUS:
      return data::token_type::operator_plus_plus;
    case boost::wave::T_ARROW:
      return data::token_type::operator_arrow;
    case boost::wave::T_ARROWSTAR:
      return data::token_type::operator_arrow_star;
    case boost::wave::T_QUESTION_MARK:
      return data::token_type::operator_question_mark;
    case boost::wave::T_RIGHTBRACE:
    case boost::wave::T_RIGHTBRACE_ALT:
    case boost::wave::T_RIGHTBRACE_TRIGRAPH:
      return data::token_type::operator_right_brace;
    case boost::wave::T_RIGHTPAREN:
      return data::token_type::operator_right_paren;
    case boost::wave::T_RIGHTBRACKET:
    case boost::wave::T_RIGHTBRACKET_ALT:
    case boost::wave::T_RIGHTBRACKET_TRIGRAPH:
      return data::token_type::operator_right_bracket;
    case boost::wave::T_COLON_COLON:
      return data::token_type::operator_colon_colon;
    case boost::wave::T_SEMICOLON:
      return data::token_type::operator_semicolon;
    case boost::wave::T_SHIFTLEFT:
      return data::token_type::operator_left_shift;
    case boost::wave::T_SHIFTLEFTASSIGN:
      return data::token_type::operator_left_shift_assign;
    case boost::wave::T_SHIFTRIGHT:
      return data::token_type::operator_right_shift;
    case boost::wave::T_SHIFTRIGHTASSIGN:
      return data::token_type::operator_right_shift_assign;
    case boost::wave::T_STAR:
      return data::token_type::operator_star;
    case boost::wave::T_COMPL:
    case boost::wave::T_COMPL_ALT:
      return data::token_type::operator_bitwise_not;
    case boost::wave::T_STARASSIGN:
      return data::token_type::operator_star_assign;
    case boost::wave::T_POUND_POUND:
    case boost::wave::T_POUND_POUND_ALT:
    case boost::wave::T_POUND_POUND_TRIGRAPH:
      return data::token_type::operator_pound_pound;
    case boost::wave::T_POUND:
    case boost::wave::T_POUND_ALT:
    case boost::wave::T_POUND_TRIGRAPH:
      return data::token_type::operator_pound;
    case boost::wave::T_PP_DEFINE:
      return data::token_type::p_define;
    case boost::wave::T_PP_IF:
      return data::token_type::p_if;
    case boost::wave::T_PP_IFDEF:
      return data::token_type::p_ifdef;
    case boost::wave::T_PP_IFNDEF:
      return data::token_type::p_ifndef;
    case boost::wave::T_PP_ELSE:
      return data::token_type::p_else;
    case boost::wave::T_PP_ELIF:
      return data::token_type::p_elif;
    case boost::wave::T_PP_ENDIF:
      return data::token_type::p_endif;
    case boost::wave::T_PP_ERROR:
      return data::token_type::p_error;
    case boost::wave::T_PP_LINE:
      return data::token_type::p_line;
    case boost::wave::T_PP_PRAGMA:
      return data::token_type::p_pragma;
    case boost::wave::T_PP_UNDEF:
      return data::token_type::p_undef;
    case boost::wave::T_PP_WARNING:
      return data::token_type::p_warning;
    case boost::wave::T_PP_INCLUDE:
    case boost::wave::T_PP_QHEADER:
    case boost::wave::T_PP_HHEADER:
      return data::token_type::p_include;
    default:
      return data::token_type::unknown;
    }
  }

  class wave_tokeniser : public iface::tokeniser
  {
  public:
    typedef boost::wave::cpplexer::lex_iterator<
        boost::wave::cpplexer::lex_token<>> token_iterator;

    std::string src;
    std::string input_filename;
    // Wave's operator!= doesn't seem to accept const object as argument
    mutable token_iterator it;
    mutable bool error_flag; // has_further_tokens might set it
    data::token current_tok;

    wave_tokeniser(std::string src_, std::string input_filename_)
      : src(std::move(src_)),
        input_filename(std::move(input_filename_)),
        it(),
        error_flag(false)
    {
      try
      {
        it = token_iterator(
            src.begin(), src.end(),
            token_iterator::value_type::position_type(input_filename.c_str()),
            boost::wave::language_support(
                boost::wave::support_cpp |
                boost::wave::support_option_long_long));
        update_current_token();
      }
      catch (const boost::wave::cpplexer::lexing_exception&)
      {
        error_flag = true;
        it = token_iterator();
      }
    }

    virtual bool has_further_tokens() const override
    {
      try
      {
        if (it != token_iterator())
        {
          const bool b = *it != boost::wave::T_EOF;
          return b;
        }
        else
        {
          return false;
        }
      }
      catch (const boost::wave::cpplexer::lexing_exception&)
      {
        error_flag = true;
        it = token_iterator();
        return false;
      }
    }

    virtual data::token current_token() const override { return current_tok; }

    virtual void move_to_next_token() override
    {
      try
      {
        // has_further_tokens seems to return different results when called
        // twice in some situations for invalid code
        if (has_further_tokens())
        {
          ++it;
          update_current_token();
        }
      }
      catch (const boost::wave::cpplexer::lexing_exception&)
      {
        error_flag = true;
        it = token_iterator();
      }
    }

    virtual bool was_error() const override { return error_flag; }

    void update_current_token()
    {
      using std::string;

      if (has_further_tokens())
      {
        const auto v = it->get_value();
        current_tok =
            data::token(string(v.begin(), v.end()), type_of_token(*it));
      }
    }
  };
}

std::unique_ptr<iface::tokeniser>
metashell::create_wave_tokeniser(std::string src_, std::string input_filename_)
{
  return std::unique_ptr<iface::tokeniser>(
      new wave_tokeniser(std::move(src_), std::move(input_filename_)));
}

std::string metashell::wave_version()
{
  return remove<'"'>(
      boost::wave::context<
          const char*,
          boost::wave::cpplexer::lex_iterator<
              boost::wave::cpplexer::lex_token<>>>::get_version_string());
}
