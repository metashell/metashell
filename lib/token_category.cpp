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

#include <metashell/data/token_category.hpp>

using namespace metashell::data;

token_category metashell::data::category_of_token(token_type type_)
{
  switch (type_)
  {
  case token_type::character_literal:
    return token_category::character_literal;
  case token_type::floating_literal:
    return token_category::floating_literal;
  case token_type::identifier:
    return token_category::identifier;
  case token_type::integer_literal:
    return token_category::integer_literal;
  case token_type::string_literal:
    return token_category::string_literal;
  case token_type::bool_literal:
    return token_category::bool_literal;
  case token_type::whitespace:
  case token_type::continue_line:
  case token_type::new_line:
    return token_category::whitespace;
  case token_type::c_comment:
  case token_type::cpp_comment:
    return token_category::comment;
  case token_type::keyword_asm:
  case token_type::keyword_auto:
  case token_type::keyword_bool:
  case token_type::keyword_break:
  case token_type::keyword_case:
  case token_type::keyword_catch:
  case token_type::keyword_char:
  case token_type::keyword_class:
  case token_type::keyword_const:
  case token_type::keyword_constexpr:
  case token_type::keyword_const_cast:
  case token_type::keyword_continue:
  case token_type::keyword_default:
  case token_type::keyword_delete:
  case token_type::keyword_do:
  case token_type::keyword_double:
  case token_type::keyword_dynamic_cast:
  case token_type::keyword_else:
  case token_type::keyword_enum:
  case token_type::keyword_explicit:
  case token_type::keyword_export:
  case token_type::keyword_extern:
  case token_type::keyword_float:
  case token_type::keyword_for:
  case token_type::keyword_friend:
  case token_type::keyword_goto:
  case token_type::keyword_if:
  case token_type::keyword_inline:
  case token_type::keyword_int:
  case token_type::keyword_long:
  case token_type::keyword_mutable:
  case token_type::keyword_namespace:
  case token_type::keyword_new:
  case token_type::keyword_operator:
  case token_type::keyword_private:
  case token_type::keyword_protected:
  case token_type::keyword_public:
  case token_type::keyword_register:
  case token_type::keyword_reinterpret_cast:
  case token_type::keyword_return:
  case token_type::keyword_short:
  case token_type::keyword_signed:
  case token_type::keyword_sizeof:
  case token_type::keyword_static:
  case token_type::keyword_static_cast:
  case token_type::keyword_struct:
  case token_type::keyword_switch:
  case token_type::keyword_template:
  case token_type::keyword_this:
  case token_type::keyword_throw:
  case token_type::keyword_try:
  case token_type::keyword_typedef:
  case token_type::keyword_typeid:
  case token_type::keyword_typename:
  case token_type::keyword_union:
  case token_type::keyword_unsigned:
  case token_type::keyword_using:
  case token_type::keyword_virtual:
  case token_type::keyword_void:
  case token_type::keyword_volatile:
  case token_type::keyword_wchar_t:
  case token_type::keyword_while:
    return token_category::keyword;
  case token_type::operator_bitwise_and:
  case token_type::operator_logical_and:
  case token_type::operator_assign:
  case token_type::operator_bitwise_and_assign:
  case token_type::operator_bitwise_or:
  case token_type::operator_bitwise_or_assign:
  case token_type::operator_bitwise_xor:
  case token_type::operator_bitwise_xor_assign:
  case token_type::operator_comma:
  case token_type::operator_colon:
  case token_type::operator_divide:
  case token_type::operator_divide_assign:
  case token_type::operator_dot:
  case token_type::operator_dotstar:
  case token_type::operator_ellipsis:
  case token_type::operator_equal:
  case token_type::operator_greater:
  case token_type::operator_greater_equal:
  case token_type::operator_left_brace:
  case token_type::operator_less:
  case token_type::operator_less_equal:
  case token_type::operator_left_paren:
  case token_type::operator_left_bracket:
  case token_type::operator_minus:
  case token_type::operator_minus_assign:
  case token_type::operator_minus_minus:
  case token_type::operator_modulo:
  case token_type::operator_modulo_assign:
  case token_type::operator_logical_not:
  case token_type::operator_not_equal:
  case token_type::operator_logical_or:
  case token_type::operator_plus:
  case token_type::operator_plus_assign:
  case token_type::operator_plus_plus:
  case token_type::operator_arrow:
  case token_type::operator_arrow_star:
  case token_type::operator_question_mark:
  case token_type::operator_right_brace:
  case token_type::operator_right_paren:
  case token_type::operator_right_bracket:
  case token_type::operator_colon_colon:
  case token_type::operator_semicolon:
  case token_type::operator_left_shift:
  case token_type::operator_left_shift_assign:
  case token_type::operator_right_shift:
  case token_type::operator_right_shift_assign:
  case token_type::operator_star:
  case token_type::operator_bitwise_not:
  case token_type::operator_star_assign:
  case token_type::operator_pound_pound:
  case token_type::operator_pound:
    return token_category::operator_token;
  case token_type::p_define:
  case token_type::p_if:
  case token_type::p_ifdef:
  case token_type::p_ifndef:
  case token_type::p_else:
  case token_type::p_elif:
  case token_type::p_endif:
  case token_type::p_error:
  case token_type::p_line:
  case token_type::p_pragma:
  case token_type::p_undef:
  case token_type::p_warning:
  case token_type::p_include:
    return token_category::preprocessor;
  case token_type::unknown:
    return token_category::unknown;
  }
  // Avoid warning about not returning a value...
  return token_category::unknown;
}

