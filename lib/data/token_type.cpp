// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2017, Abel Sinkovics (abel@sinkovics.hu)
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

#include <metashell/data/token_type.hpp>

#include <cassert>

namespace metashell
{
  namespace data
  {
    std::string to_string(token_type t_)
    {
      switch (t_)
      {
      case token_type::unknown:
        return "unknown";

      case token_type::identifier:
        return "identifier";

      case token_type::character_literal:
        return "character_literal";
      case token_type::floating_literal:
        return "floating_literal";
      case token_type::integer_literal:
        return "integer_literal";
      case token_type::string_literal:
        return "string_literal";
      case token_type::bool_literal:
        return "bool_literal";

      case token_type::c_comment:
        return "c_comment";
      case token_type::cpp_comment:
        return "cpp_comment";
      case token_type::whitespace:
        return "whitespace";
      case token_type::continue_line:
        return "continue_line";
      case token_type::new_line:
        return "new_line";

      case token_type::keyword_asm:
        return "keyword_asm";
      case token_type::keyword_auto:
        return "keyword_auto";
      case token_type::keyword_bool:
        return "keyword_bool";
      case token_type::keyword_break:
        return "keyword_break";
      case token_type::keyword_case:
        return "keyword_case";
      case token_type::keyword_catch:
        return "keyword_catch";
      case token_type::keyword_char:
        return "keyword_char";
      case token_type::keyword_class:
        return "keyword_class";
      case token_type::keyword_const:
        return "keyword_const";
      case token_type::keyword_constexpr:
        return "keyword_constexpr";
      case token_type::keyword_const_cast:
        return "keyword_const_cast";
      case token_type::keyword_continue:
        return "keyword_continue";
      case token_type::keyword_default:
        return "keyword_default";
      case token_type::keyword_delete:
        return "keyword_delete";
      case token_type::keyword_do:
        return "keyword_do";
      case token_type::keyword_double:
        return "keyword_double";
      case token_type::keyword_dynamic_cast:
        return "keyword_dynamic_cast";
      case token_type::keyword_else:
        return "keyword_else";
      case token_type::keyword_enum:
        return "keyword_enum";
      case token_type::keyword_explicit:
        return "keyword_explicit";
      case token_type::keyword_export:
        return "keyword_export";
      case token_type::keyword_extern:
        return "keyword_extern";
      case token_type::keyword_float:
        return "keyword_float";
      case token_type::keyword_for:
        return "keyword_for";
      case token_type::keyword_friend:
        return "keyword_friend";
      case token_type::keyword_goto:
        return "keyword_goto";
      case token_type::keyword_if:
        return "keyword_if";
      case token_type::keyword_inline:
        return "keyword_inline";
      case token_type::keyword_int:
        return "keyword_int";
      case token_type::keyword_long:
        return "keyword_long";
      case token_type::keyword_mutable:
        return "keyword_mutable";
      case token_type::keyword_namespace:
        return "keyword_namespace";
      case token_type::keyword_new:
        return "keyword_new";
      case token_type::keyword_operator:
        return "keyword_operator";
      case token_type::keyword_private:
        return "keyword_private";
      case token_type::keyword_protected:
        return "keyword_protected";
      case token_type::keyword_public:
        return "keyword_public";
      case token_type::keyword_register:
        return "keyword_register";
      case token_type::keyword_reinterpret_cast:
        return "keyword_reinterpret_cast";
      case token_type::keyword_return:
        return "keyword_return";
      case token_type::keyword_short:
        return "keyword_short";
      case token_type::keyword_signed:
        return "keyword_signed";
      case token_type::keyword_sizeof:
        return "keyword_sizeof";
      case token_type::keyword_static:
        return "keyword_static";
      case token_type::keyword_static_cast:
        return "keyword_static_cast";
      case token_type::keyword_struct:
        return "keyword_struct";
      case token_type::keyword_switch:
        return "keyword_switch";
      case token_type::keyword_template:
        return "keyword_template";
      case token_type::keyword_this:
        return "keyword_this";
      case token_type::keyword_throw:
        return "keyword_throw";
      case token_type::keyword_try:
        return "keyword_try";
      case token_type::keyword_typedef:
        return "keyword_typedef";
      case token_type::keyword_typeid:
        return "keyword_typeid";
      case token_type::keyword_typename:
        return "keyword_typename";
      case token_type::keyword_union:
        return "keyword_union";
      case token_type::keyword_unsigned:
        return "keyword_unsigned";
      case token_type::keyword_using:
        return "keyword_using";
      case token_type::keyword_virtual:
        return "keyword_virtual";
      case token_type::keyword_void:
        return "keyword_void";
      case token_type::keyword_volatile:
        return "keyword_volatile";
      case token_type::keyword_wchar_t:
        return "keyword_wchar_t";
      case token_type::keyword_while:
        return "keyword_while";

      case token_type::operator_bitwise_and:
        return "operator_bitwise_and";
      case token_type::operator_logical_and:
        return "operator_logical_and";
      case token_type::operator_assign:
        return "operator_assign";
      case token_type::operator_bitwise_and_assign:
        return "operator_bitwise_and_assign";
      case token_type::operator_bitwise_or:
        return "operator_bitwise_or";
      case token_type::operator_bitwise_or_assign:
        return "operator_bitwise_or_assign";
      case token_type::operator_bitwise_xor:
        return "operator_bitwise_xor";
      case token_type::operator_bitwise_xor_assign:
        return "operator_bitwise_xor_assign";
      case token_type::operator_comma:
        return "operator_comma";
      case token_type::operator_colon:
        return "operator_colon";
      case token_type::operator_divide:
        return "operator_divide";
      case token_type::operator_divide_assign:
        return "operator_divide_assign";
      case token_type::operator_dot:
        return "operator_dot";
      case token_type::operator_dotstar:
        return "operator_dotstar";
      case token_type::operator_ellipsis:
        return "operator_ellipsis";
      case token_type::operator_equal:
        return "operator_equal";
      case token_type::operator_greater:
        return "operator_greater";
      case token_type::operator_greater_equal:
        return "operator_greater_equal";
      case token_type::operator_left_brace:
        return "operator_left_brace";
      case token_type::operator_less:
        return "operator_less";
      case token_type::operator_less_equal:
        return "operator_less_equal";
      case token_type::operator_left_paren:
        return "operator_left_paren";
      case token_type::operator_left_bracket:
        return "operator_left_bracket";
      case token_type::operator_minus:
        return "operator_minus";
      case token_type::operator_minus_assign:
        return "operator_minus_assign";
      case token_type::operator_minus_minus:
        return "operator_minus_minus";
      case token_type::operator_modulo:
        return "operator_modulo";
      case token_type::operator_modulo_assign:
        return "operator_modulo_assign";
      case token_type::operator_logical_not:
        return "operator_logical_not";
      case token_type::operator_not_equal:
        return "operator_not_equal";
      case token_type::operator_logical_or:
        return "operator_logical_or";
      case token_type::operator_plus:
        return "operator_plus";
      case token_type::operator_plus_assign:
        return "operator_plus_assign";
      case token_type::operator_plus_plus:
        return "operator_plus_plus";
      case token_type::operator_arrow:
        return "operator_arrow";
      case token_type::operator_arrow_star:
        return "operator_arrow_star";
      case token_type::operator_question_mark:
        return "operator_question_mark";
      case token_type::operator_right_brace:
        return "operator_right_brace";
      case token_type::operator_right_paren:
        return "operator_right_paren";
      case token_type::operator_right_bracket:
        return "operator_right_bracket";
      case token_type::operator_colon_colon:
        return "operator_colon_colon";
      case token_type::operator_semicolon:
        return "operator_semicolon";
      case token_type::operator_left_shift:
        return "operator_left_shift";
      case token_type::operator_left_shift_assign:
        return "operator_left_shift_assign";
      case token_type::operator_right_shift:
        return "operator_right_shift";
      case token_type::operator_right_shift_assign:
        return "operator_right_shift_assign";
      case token_type::operator_star:
        return "operator_star";
      case token_type::operator_bitwise_not:
        return "operator_bitwise_not";
      case token_type::operator_star_assign:
        return "operator_star_assign";
      case token_type::operator_pound_pound:
        return "operator_pound_pound";
      case token_type::operator_pound:
        return "operator_pound";

      case token_type::p_define:
        return "p_define";
      case token_type::p_if:
        return "p_if";
      case token_type::p_ifdef:
        return "p_ifdef";
      case token_type::p_ifndef:
        return "p_ifndef";
      case token_type::p_else:
        return "p_else";
      case token_type::p_elif:
        return "p_elif";
      case token_type::p_endif:
        return "p_endif";
      case token_type::p_error:
        return "p_error";
      case token_type::p_line:
        return "p_line";
      case token_type::p_pragma:
        return "p_pragma";
      case token_type::p_undef:
        return "p_undef";
      case token_type::p_warning:
        return "p_warning";
      case token_type::p_include:
        return "p_include";
      }
      assert(false);
      return "";
    }
  }
}
