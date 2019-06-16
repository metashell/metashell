// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2019, Abel Sinkovics (abel@sinkovics.hu)
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

#ifndef METASHELL_TOKEN_TYPE
#error METASHELL_TOKEN_TYPE not defined
#endif

#ifdef TOKEN_TYPE_SEP
#error TOKEN_TYPE_SEP already defined
#endif
#define TOKEN_TYPE_SEP ,

// clang-format off

METASHELL_TOKEN_TYPE(unknown,                     unknown,           TOKEN_VALUE)

METASHELL_TOKEN_TYPE(identifier,                  identifier,        TOKEN_VALUE)

METASHELL_TOKEN_TYPE(character_literal,           character_literal, TOKEN_VALUE)
METASHELL_TOKEN_TYPE(floating_literal,            floating_literal,  TOKEN_VALUE)
METASHELL_TOKEN_TYPE(integer_literal,             integer_literal,   TOKEN_VALUE)
METASHELL_TOKEN_TYPE(string_literal,              string_literal,    TOKEN_VALUE)
METASHELL_TOKEN_TYPE(bool_literal,                bool_literal,      TOKEN_VALUE)

METASHELL_TOKEN_TYPE(c_comment,                   comment,           TOKEN_VALUE)
METASHELL_TOKEN_TYPE(cpp_comment,                 comment,           TOKEN_VALUE)
METASHELL_TOKEN_TYPE(whitespace,                  whitespace,        TOKEN_VALUE)
METASHELL_TOKEN_TYPE(continue_line,               whitespace,        "\\\\\\n")
METASHELL_TOKEN_TYPE(new_line,                    whitespace,        "\\n")

METASHELL_TOKEN_TYPE(keyword_asm,                 keyword,           "asm")
METASHELL_TOKEN_TYPE(keyword_auto,                keyword,           "auto")
METASHELL_TOKEN_TYPE(keyword_bool,                keyword,           "bool")
METASHELL_TOKEN_TYPE(keyword_break,               keyword,           "break")
METASHELL_TOKEN_TYPE(keyword_case,                keyword,           "case")
METASHELL_TOKEN_TYPE(keyword_catch,               keyword,           "catch")
METASHELL_TOKEN_TYPE(keyword_char,                keyword,           "char")
METASHELL_TOKEN_TYPE(keyword_class,               keyword,           "class")
METASHELL_TOKEN_TYPE(keyword_const,               keyword,           "const")
METASHELL_TOKEN_TYPE(keyword_constexpr,           keyword,           "constexpr")
METASHELL_TOKEN_TYPE(keyword_const_cast,          keyword,           "const_cast")
METASHELL_TOKEN_TYPE(keyword_continue,            keyword,           "continue")
METASHELL_TOKEN_TYPE(keyword_default,             keyword,           "default")
METASHELL_TOKEN_TYPE(keyword_delete,              keyword,           "delete")
METASHELL_TOKEN_TYPE(keyword_do,                  keyword,           "do")
METASHELL_TOKEN_TYPE(keyword_double,              keyword,           "double")
METASHELL_TOKEN_TYPE(keyword_dynamic_cast,        keyword,           "dynamic_cast")
METASHELL_TOKEN_TYPE(keyword_else,                keyword,           "else")
METASHELL_TOKEN_TYPE(keyword_enum,                keyword,           "enum")
METASHELL_TOKEN_TYPE(keyword_explicit,            keyword,           "explicit")
METASHELL_TOKEN_TYPE(keyword_export,              keyword,           "export")
METASHELL_TOKEN_TYPE(keyword_extern,              keyword,           "extern")
METASHELL_TOKEN_TYPE(keyword_float,               keyword,           "float")
METASHELL_TOKEN_TYPE(keyword_for,                 keyword,           "for")
METASHELL_TOKEN_TYPE(keyword_friend,              keyword,           "friend")
METASHELL_TOKEN_TYPE(keyword_goto,                keyword,           "goto")
METASHELL_TOKEN_TYPE(keyword_if,                  keyword,           "if")
METASHELL_TOKEN_TYPE(keyword_inline,              keyword,           "inline")
METASHELL_TOKEN_TYPE(keyword_int,                 keyword,           "int")
METASHELL_TOKEN_TYPE(keyword_long,                keyword,           "long")
METASHELL_TOKEN_TYPE(keyword_mutable,             keyword,           "mutable")
METASHELL_TOKEN_TYPE(keyword_namespace,           keyword,           "namespace")
METASHELL_TOKEN_TYPE(keyword_new,                 keyword,           "new")
METASHELL_TOKEN_TYPE(keyword_operator,            keyword,           "operator")
METASHELL_TOKEN_TYPE(keyword_private,             keyword,           "private")
METASHELL_TOKEN_TYPE(keyword_protected,           keyword,           "protected")
METASHELL_TOKEN_TYPE(keyword_public,              keyword,           "public")
METASHELL_TOKEN_TYPE(keyword_register,            keyword,           "register")
METASHELL_TOKEN_TYPE(keyword_reinterpret_cast,    keyword,           "reinterpret_cast")
METASHELL_TOKEN_TYPE(keyword_return,              keyword,           "return")
METASHELL_TOKEN_TYPE(keyword_short,               keyword,           "short")
METASHELL_TOKEN_TYPE(keyword_signed,              keyword,           "signed")
METASHELL_TOKEN_TYPE(keyword_sizeof,              keyword,           "sizeof")
METASHELL_TOKEN_TYPE(keyword_static,              keyword,           "static")
METASHELL_TOKEN_TYPE(keyword_static_cast,         keyword,           "static_cast")
METASHELL_TOKEN_TYPE(keyword_struct,              keyword,           "struct")
METASHELL_TOKEN_TYPE(keyword_switch,              keyword,           "switch")
METASHELL_TOKEN_TYPE(keyword_template,            keyword,           "template")
METASHELL_TOKEN_TYPE(keyword_this,                keyword,           "this")
METASHELL_TOKEN_TYPE(keyword_throw,               keyword,           "throw")
METASHELL_TOKEN_TYPE(keyword_try,                 keyword,           "try")
METASHELL_TOKEN_TYPE(keyword_typedef,             keyword,           "typedef")
METASHELL_TOKEN_TYPE(keyword_typeid,              keyword,           "typeid")
METASHELL_TOKEN_TYPE(keyword_typename,            keyword,           "typename")
METASHELL_TOKEN_TYPE(keyword_union,               keyword,           "union")
METASHELL_TOKEN_TYPE(keyword_unsigned,            keyword,           "unsigned")
METASHELL_TOKEN_TYPE(keyword_using,               keyword,           "using")
METASHELL_TOKEN_TYPE(keyword_virtual,             keyword,           "virtual")
METASHELL_TOKEN_TYPE(keyword_void,                keyword,           "void")
METASHELL_TOKEN_TYPE(keyword_volatile,            keyword,           "volatile")
METASHELL_TOKEN_TYPE(keyword_wchar_t,             keyword,           "wchar_t")
METASHELL_TOKEN_TYPE(keyword_while,               keyword,           "while")

METASHELL_TOKEN_TYPE(operator_bitwise_and,        operator_token,    "&")
METASHELL_TOKEN_TYPE(operator_logical_and,        operator_token,    "&&")
METASHELL_TOKEN_TYPE(operator_assign,             operator_token,    "=")
METASHELL_TOKEN_TYPE(operator_bitwise_and_assign, operator_token,    "&=")
METASHELL_TOKEN_TYPE(operator_bitwise_or,         operator_token,    "|")
METASHELL_TOKEN_TYPE(operator_bitwise_or_assign,  operator_token,    "|=")
METASHELL_TOKEN_TYPE(operator_bitwise_xor,        operator_token,    "^")
METASHELL_TOKEN_TYPE(operator_bitwise_xor_assign, operator_token,    "^=")
METASHELL_TOKEN_TYPE(operator_comma,              operator_token,    ",")
METASHELL_TOKEN_TYPE(operator_colon,              operator_token,    ":")
METASHELL_TOKEN_TYPE(operator_divide,             operator_token,    "/")
METASHELL_TOKEN_TYPE(operator_divide_assign,      operator_token,    "/=")
METASHELL_TOKEN_TYPE(operator_dot,                operator_token,    ".")
METASHELL_TOKEN_TYPE(operator_dotstar,            operator_token,    ".*")
METASHELL_TOKEN_TYPE(operator_ellipsis,           operator_token,    "...")
METASHELL_TOKEN_TYPE(operator_equal,              operator_token,    "==")
METASHELL_TOKEN_TYPE(operator_greater,            operator_token,    ">")
METASHELL_TOKEN_TYPE(operator_greater_equal,      operator_token,    ">=")
METASHELL_TOKEN_TYPE(operator_left_brace,         operator_token,    "{")
METASHELL_TOKEN_TYPE(operator_less,               operator_token,    "<")
METASHELL_TOKEN_TYPE(operator_less_equal,         operator_token,    "<=")
METASHELL_TOKEN_TYPE(operator_left_paren,         operator_token,    "(")
METASHELL_TOKEN_TYPE(operator_left_bracket,       operator_token,    "[")
METASHELL_TOKEN_TYPE(operator_minus,              operator_token,    "-")
METASHELL_TOKEN_TYPE(operator_minus_assign,       operator_token,    "-=")
METASHELL_TOKEN_TYPE(operator_minus_minus,        operator_token,    "--")
METASHELL_TOKEN_TYPE(operator_modulo,             operator_token,    "%")
METASHELL_TOKEN_TYPE(operator_modulo_assign,      operator_token,    "%=")
METASHELL_TOKEN_TYPE(operator_logical_not,        operator_token,    "!")
METASHELL_TOKEN_TYPE(operator_not_equal,          operator_token,    "!=")
METASHELL_TOKEN_TYPE(operator_logical_or,         operator_token,    "|")
METASHELL_TOKEN_TYPE(operator_plus,               operator_token,    "+")
METASHELL_TOKEN_TYPE(operator_plus_assign,        operator_token,    "+=")
METASHELL_TOKEN_TYPE(operator_plus_plus,          operator_token,    "++")
METASHELL_TOKEN_TYPE(operator_arrow,              operator_token,    "->")
METASHELL_TOKEN_TYPE(operator_arrow_star,         operator_token,    "->*")
METASHELL_TOKEN_TYPE(operator_question_mark,      operator_token,    "?")
METASHELL_TOKEN_TYPE(operator_right_brace,        operator_token,    "}")
METASHELL_TOKEN_TYPE(operator_right_paren,        operator_token,    ")")
METASHELL_TOKEN_TYPE(operator_right_bracket,      operator_token,    "]")
METASHELL_TOKEN_TYPE(operator_colon_colon,        operator_token,    "::")
METASHELL_TOKEN_TYPE(operator_semicolon,          operator_token,    ";")
METASHELL_TOKEN_TYPE(operator_left_shift,         operator_token,    "<<")
METASHELL_TOKEN_TYPE(operator_left_shift_assign,  operator_token,    "<<=")
METASHELL_TOKEN_TYPE(operator_right_shift,        operator_token,    ">>")
METASHELL_TOKEN_TYPE(operator_right_shift_assign, operator_token,    ">>=")
METASHELL_TOKEN_TYPE(operator_star,               operator_token,    "*")
METASHELL_TOKEN_TYPE(operator_bitwise_not,        operator_token,    "~")
METASHELL_TOKEN_TYPE(operator_star_assign,        operator_token,    "*=")
METASHELL_TOKEN_TYPE(operator_pound_pound,        operator_token,    "##")
METASHELL_TOKEN_TYPE(operator_pound,              operator_token,    "#")

METASHELL_TOKEN_TYPE(p_define,                    preprocessor,      "#define")
METASHELL_TOKEN_TYPE(p_if,                        preprocessor,      "#if")
METASHELL_TOKEN_TYPE(p_ifdef,                     preprocessor,      "#ifdef")
METASHELL_TOKEN_TYPE(p_ifndef,                    preprocessor,      "#ifndef")
METASHELL_TOKEN_TYPE(p_else,                      preprocessor,      "#else")
METASHELL_TOKEN_TYPE(p_elif,                      preprocessor,      "#elif")
METASHELL_TOKEN_TYPE(p_endif,                     preprocessor,      "#endif")
METASHELL_TOKEN_TYPE(p_error,                     preprocessor,      "#error")
METASHELL_TOKEN_TYPE(p_line,                      preprocessor,      "#line")
METASHELL_TOKEN_TYPE(p_pragma,                    preprocessor,      "#pragma")
METASHELL_TOKEN_TYPE(p_undef,                     preprocessor,      "#undef")
METASHELL_TOKEN_TYPE(p_warning,                   preprocessor,      "#warning")

#undef TOKEN_TYPE_SEP
#define TOKEN_TYPE_SEP
METASHELL_TOKEN_TYPE(p_include,                   preprocessor,      "#include")

// clang-format on

#undef TOKEN_TYPE_SEP
