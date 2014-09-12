#ifndef METASHELL_TOKEN_TYPE_HPP
#define METASHELL_TOKEN_TYPE_HPP

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

namespace metashell
{
  enum class token_type
  {
    unknown,

    identifier,

    character_literal,
    floating_literal,
    integer_literal,
    string_literal,
    bool_literal,

    c_comment,
    cpp_comment,
    whitespace,
    continue_line, // "\\\n"
    new_line,

    keyword_asm,
    keyword_auto,
    keyword_bool,
    keyword_break,
    keyword_case,
    keyword_catch,
    keyword_char,
    keyword_class,
    keyword_const,
    keyword_constexpr,
    keyword_const_cast,
    keyword_continue,
    keyword_default,
    keyword_delete,
    keyword_do,
    keyword_double,
    keyword_dynamic_cast,
    keyword_else,
    keyword_enum,
    keyword_explicit,
    keyword_export,
    keyword_extern,
    keyword_float,
    keyword_for,
    keyword_friend,
    keyword_goto,
    keyword_if,
    keyword_inline,
    keyword_int,
    keyword_long,
    keyword_mutable,
    keyword_namespace,
    keyword_new,
    keyword_operator,
    keyword_private,
    keyword_protected,
    keyword_public,
    keyword_register,
    keyword_reinterpret_cast,
    keyword_return,
    keyword_short,
    keyword_signed,
    keyword_sizeof,
    keyword_static,
    keyword_static_cast,
    keyword_struct,
    keyword_switch,
    keyword_template,
    keyword_this,
    keyword_throw,
    keyword_try,
    keyword_typedef,
    keyword_typeid,
    keyword_typename,
    keyword_union,
    keyword_unsigned,
    keyword_using,
    keyword_virtual,
    keyword_void,
    keyword_volatile,
    keyword_wchar_t,
    keyword_while,

    operator_bitwise_and, // &
    operator_logical_and, // &&
    operator_assign, // =
    operator_bitwise_and_assign, // &=
    operator_bitwise_or, // |
    operator_bitwise_or_assign, // |=
    operator_bitwise_xor, // ^
    operator_bitwise_xor_assign, // ^=
    operator_comma, // ,
    operator_colon, // :
    operator_divide, // /
    operator_divide_assign, // /=
    operator_dot, // .
    operator_dotstar, // .*
    operator_ellipsis, // ...
    operator_equal, // ==
    operator_greater, // >
    operator_greater_equal, // >=
    operator_left_brace, // {
    operator_less, // <
    operator_less_equal, // <=
    operator_left_paren, // (
    operator_left_bracket, // [
    operator_minus, // -
    operator_minus_assign, // -=
    operator_minus_minus, // --
    operator_modulo, // %
    operator_modulo_assign, // %=
    operator_logical_not, // !
    operator_not_equal, // !=
    operator_logical_or, // |
    operator_plus, // +
    operator_plus_assign, // +=
    operator_plus_plus, // ++
    operator_arrow, // ->
    operator_arrow_star, // ->*
    operator_question_mark, // ?
    operator_right_brace, // }
    operator_right_paren, // )
    operator_right_bracket, // ]
    operator_colon_colon, // ::
    operator_semicolon, // ;
    operator_left_shift, // <<
    operator_left_shift_assign, // <<=
    operator_right_shift, // >>
    operator_right_shift_assign, // >>=
    operator_star, // *
    operator_bitwise_not, // ~
    operator_star_assign, // *=
    operator_pound_pound, // ##
    operator_pound, // #

    p_define, // #define
    p_if, // #if
    p_ifdef, // #ifdef
    p_ifndef, // #ifndef
    p_else, // #else
    p_elif, // #elif
    p_endif, // #endif
    p_error, // #error
    p_line, // #line
    p_pragma, // #pragma
    p_undef, // #undef
    p_warning, // #warning
    p_include // #include
  };
}

#endif

