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

#include <metashell/data/token.hpp>

#include <cassert>

using namespace metashell::data;

token::token() : _type(token_type::unknown), _value() {}

token::token(cpp_code value_, token_type type_)
  : _type(type_), _value(std::move(value_))
{
}

token_type token::type() const { return _type; }

token_category token::category() const { return category_of_token(_type); }

const cpp_code& token::value() const { return _value; }

std::string metashell::data::string_literal_value(const token& token_)
{
  assert(token_.type() == token_type::string_literal);

  std::string value = token_.value().value();
  if (!value.empty() && value[0] == '"')
  {
    value.erase(0, 1);
  }
  if (!value.empty() && value[value.size() - 1] == '"')
  {
    value.erase(value.size() - 1);
  }
  return value;
}

bool metashell::data::operator==(const token& a_, const token& b_)
{
  return a_.value() == b_.value() && a_.type() == b_.type();
}

bool metashell::data::operator<(const token& a_, const token& b_)
{
  return a_.type() < b_.type() ||
         (a_.type() == b_.type() && a_.value() < b_.value());
}

std::ostream& metashell::data::operator<<(std::ostream& out_, const token& t_)
{
  return out_ << "token(\"" << t_.value() << "\", " << to_string(t_.type())
              << ")";
}

std::string metashell::data::format_token(const token& t_)
{
  switch (t_.type())
  {
  case token_type::unknown:
  case token_type::identifier:
  case token_type::character_literal:
  case token_type::floating_literal:
  case token_type::integer_literal:
  case token_type::string_literal:
  case token_type::bool_literal:
  case token_type::c_comment:
  case token_type::cpp_comment:
  case token_type::whitespace:
    return t_.value().value();

  case token_type::continue_line:
    return "\\\\\\n";
  case token_type::new_line:
    return "\\n";

  case token_type::keyword_asm:
    return "asm";
  case token_type::keyword_auto:
    return "auto";
  case token_type::keyword_bool:
    return "bool";
  case token_type::keyword_break:
    return "break";
  case token_type::keyword_case:
    return "case";
  case token_type::keyword_catch:
    return "catch";
  case token_type::keyword_char:
    return "char";
  case token_type::keyword_class:
    return "class";
  case token_type::keyword_const:
    return "const";
  case token_type::keyword_constexpr:
    return "constexpr";
  case token_type::keyword_const_cast:
    return "const_cast";
  case token_type::keyword_continue:
    return "continue";
  case token_type::keyword_default:
    return "default";
  case token_type::keyword_delete:
    return "delete";
  case token_type::keyword_do:
    return "do";
  case token_type::keyword_double:
    return "double";
  case token_type::keyword_dynamic_cast:
    return "dynamic_cast";
  case token_type::keyword_else:
    return "else";
  case token_type::keyword_enum:
    return "enum";
  case token_type::keyword_explicit:
    return "explicit";
  case token_type::keyword_export:
    return "export";
  case token_type::keyword_extern:
    return "extern";
  case token_type::keyword_float:
    return "float";
  case token_type::keyword_for:
    return "for";
  case token_type::keyword_friend:
    return "friend";
  case token_type::keyword_goto:
    return "goto";
  case token_type::keyword_if:
    return "if";
  case token_type::keyword_inline:
    return "inline";
  case token_type::keyword_int:
    return "int";
  case token_type::keyword_long:
    return "long";
  case token_type::keyword_mutable:
    return "mutable";
  case token_type::keyword_namespace:
    return "namespace";
  case token_type::keyword_new:
    return "new";
  case token_type::keyword_operator:
    return "operator";
  case token_type::keyword_private:
    return "private";
  case token_type::keyword_protected:
    return "protected";
  case token_type::keyword_public:
    return "public";
  case token_type::keyword_register:
    return "register";
  case token_type::keyword_reinterpret_cast:
    return "reinterpret_cast";
  case token_type::keyword_return:
    return "return";
  case token_type::keyword_short:
    return "short";
  case token_type::keyword_signed:
    return "signed";
  case token_type::keyword_sizeof:
    return "sizeof";
  case token_type::keyword_static:
    return "static";
  case token_type::keyword_static_cast:
    return "static_cast";
  case token_type::keyword_struct:
    return "struct";
  case token_type::keyword_switch:
    return "switch";
  case token_type::keyword_template:
    return "template";
  case token_type::keyword_this:
    return "this";
  case token_type::keyword_throw:
    return "throw";
  case token_type::keyword_try:
    return "try";
  case token_type::keyword_typedef:
    return "typedef";
  case token_type::keyword_typeid:
    return "typeid";
  case token_type::keyword_typename:
    return "typename";
  case token_type::keyword_union:
    return "union";
  case token_type::keyword_unsigned:
    return "unsigned";
  case token_type::keyword_using:
    return "using";
  case token_type::keyword_virtual:
    return "virtual";
  case token_type::keyword_void:
    return "void";
  case token_type::keyword_volatile:
    return "volatile";
  case token_type::keyword_wchar_t:
    return "wchar_t";
  case token_type::keyword_while:
    return "while";

  case token_type::operator_bitwise_and:
    return "&";
  case token_type::operator_logical_and:
    return "&&";
  case token_type::operator_assign:
    return "=";
  case token_type::operator_bitwise_and_assign:
    return "&=";
  case token_type::operator_bitwise_or:
    return "|";
  case token_type::operator_bitwise_or_assign:
    return "|=";
  case token_type::operator_bitwise_xor:
    return "^";
  case token_type::operator_bitwise_xor_assign:
    return "^=";
  case token_type::operator_comma:
    return ",";
  case token_type::operator_colon:
    return ":";
  case token_type::operator_divide:
    return "/";
  case token_type::operator_divide_assign:
    return "/=";
  case token_type::operator_dot:
    return ".";
  case token_type::operator_dotstar:
    return ".*";
  case token_type::operator_ellipsis:
    return "...";
  case token_type::operator_equal:
    return "==";
  case token_type::operator_greater:
    return ">";
  case token_type::operator_greater_equal:
    return ">=";
  case token_type::operator_left_brace:
    return "{";
  case token_type::operator_less:
    return "<";
  case token_type::operator_less_equal:
    return "<=";
  case token_type::operator_left_paren:
    return "(";
  case token_type::operator_left_bracket:
    return "[";
  case token_type::operator_minus:
    return "-";
  case token_type::operator_minus_assign:
    return "-=";
  case token_type::operator_minus_minus:
    return "--";
  case token_type::operator_modulo:
    return "%";
  case token_type::operator_modulo_assign:
    return "%=";
  case token_type::operator_logical_not:
    return "!";
  case token_type::operator_not_equal:
    return "!=";
  case token_type::operator_logical_or:
    return "|";
  case token_type::operator_plus:
    return "+";
  case token_type::operator_plus_assign:
    return "+=";
  case token_type::operator_plus_plus:
    return "++";
  case token_type::operator_arrow:
    return "->";
  case token_type::operator_arrow_star:
    return "->*";
  case token_type::operator_question_mark:
    return "?";
  case token_type::operator_right_brace:
    return "}";
  case token_type::operator_right_paren:
    return ")";
  case token_type::operator_right_bracket:
    return "]";
  case token_type::operator_colon_colon:
    return "::";
  case token_type::operator_semicolon:
    return ";";
  case token_type::operator_left_shift:
    return "<<";
  case token_type::operator_left_shift_assign:
    return "<<=";
  case token_type::operator_right_shift:
    return ">>";
  case token_type::operator_right_shift_assign:
    return ">>=";
  case token_type::operator_star:
    return "*";
  case token_type::operator_bitwise_not:
    return "~";
  case token_type::operator_star_assign:
    return "*=";
  case token_type::operator_pound_pound:
    return "##";
  case token_type::operator_pound:
    return "#";

  case token_type::p_define:
    return "#define";
  case token_type::p_if:
    return "#if";
  case token_type::p_ifdef:
    return "#ifdef";
  case token_type::p_ifndef:
    return "#ifndef";
  case token_type::p_else:
    return "#else";
  case token_type::p_elif:
    return "#elif";
  case token_type::p_endif:
    return "#endif";
  case token_type::p_error:
    return "#error";
  case token_type::p_line:
    return "#line";
  case token_type::p_pragma:
    return "#pragma";
  case token_type::p_undef:
    return "#undef";
  case token_type::p_warning:
    return "#warning";
  case token_type::p_include:
    return "#include";
  }
  assert(false);
  return std::string();
}
