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

#include <metashell/wave_tokeniser.hpp>

#include <just/test.hpp>

using namespace metashell;

namespace
{
  void test_type_of_a_token(const std::string& token_,
                            data::token_type expected_type_)
  {
    const auto t = create_wave_tokeniser(token_);

    JUST_ASSERT(t->has_further_tokens());
    JUST_ASSERT(expected_type_ == t->current_token().type());
  }

  void test_category_of_a_token(const std::string& token_,
                                data::token_category expected_category_)
  {
    const auto t = create_wave_tokeniser(token_);

    JUST_ASSERT(t->has_further_tokens());
    JUST_ASSERT(expected_category_ == t->current_token().category());
  }
}

JUST_TEST_CASE(test_empty_code_is_empty_token_sequence)
{
  const auto t = create_wave_tokeniser("");

  JUST_ASSERT(!t->has_further_tokens());
}

JUST_TEST_CASE(test_code_with_one_token_has_token)
{
  const auto t = create_wave_tokeniser("foo");

  JUST_ASSERT(t->has_further_tokens());
}

JUST_TEST_CASE(test_code_with_one_token_has_one_token)
{
  const auto t = create_wave_tokeniser("foo");

  t->move_to_next_token();

  JUST_ASSERT(!t->has_further_tokens());
}

JUST_TEST_CASE(test_value_of_token)
{
  const auto t = create_wave_tokeniser("foo ");

  JUST_ASSERT_EQUAL("foo", t->current_token().value());

  t->move_to_next_token();

  JUST_ASSERT_EQUAL(" ", t->current_token().value());
}

JUST_TEST_CASE(test_category_of_a_character_literal)
{
  test_category_of_a_token("'a'", data::token_category::character_literal);
}

JUST_TEST_CASE(test_category_of_a_floating_literal)
{
  test_category_of_a_token("11.13", data::token_category::floating_literal);
}

JUST_TEST_CASE(test_category_of_an_integer_literal)
{
  test_category_of_a_token("13", data::token_category::integer_literal);
}

JUST_TEST_CASE(test_category_of_a_string_literal)
{
  test_category_of_a_token("\"foo bar\"", data::token_category::string_literal);
}

JUST_TEST_CASE(test_category_of_a_bool_literal)
{
  test_category_of_a_token("true", data::token_category::bool_literal);
}

JUST_TEST_CASE(test_category_of_an_identifier)
{
  test_category_of_a_token("foo", data::token_category::identifier);
  test_category_of_a_token("std", data::token_category::identifier);
}

JUST_TEST_CASE(test_category_of_a_whitespace)
{
  test_category_of_a_token(" ", data::token_category::whitespace);
  test_category_of_a_token("\n", data::token_category::whitespace);
}

JUST_TEST_CASE(test_category_of_a_comment)
{
  test_category_of_a_token("/* c comment */", data::token_category::comment);
  test_category_of_a_token("// c++ comment\n", data::token_category::comment);
}

JUST_TEST_CASE(test_category_of_a_keyword)
{
  test_category_of_a_token("for", data::token_category::keyword);
}

JUST_TEST_CASE(test_category_of_an_operator)
{
  test_category_of_a_token("+=", data::token_category::operator_token);
}

JUST_TEST_CASE(test_category_of_a_preprocessor_token)
{
  test_category_of_a_token("#define", data::token_category::preprocessor);
}

JUST_TEST_CASE(test_type_of_identifier_token)
{
  test_type_of_a_token("foo", data::token_type::identifier);
}

JUST_TEST_CASE(test_type_of_character_literal_token)
{
  test_type_of_a_token("'a'", data::token_type::character_literal);
}

JUST_TEST_CASE(test_type_of_floating_literal_token)
{
  test_type_of_a_token("11.13", data::token_type::floating_literal);
}

JUST_TEST_CASE(test_type_of_integer_literal_token)
{
  test_type_of_a_token("015", data::token_type::integer_literal);
  test_type_of_a_token("0xd", data::token_type::integer_literal);
  test_type_of_a_token("13", data::token_type::integer_literal);
  test_type_of_a_token("13LL", data::token_type::integer_literal);
}

JUST_TEST_CASE(test_type_of_string_literal_token)
{
  test_type_of_a_token("\"foo bar\"", data::token_type::string_literal);
}

JUST_TEST_CASE(test_type_of_bool_literal_token)
{
  test_type_of_a_token("true", data::token_type::bool_literal);
  test_type_of_a_token("false", data::token_type::bool_literal);
}

JUST_TEST_CASE(test_type_of_comment_token)
{
  test_type_of_a_token("/* foo bar */", data::token_type::c_comment);
  test_type_of_a_token("// foo bar\n", data::token_type::cpp_comment);
}

JUST_TEST_CASE(test_type_of_whitespace_token)
{
  test_type_of_a_token(" ", data::token_type::whitespace);
  test_type_of_a_token("\f", data::token_type::whitespace);
}

JUST_TEST_CASE(test_type_of_new_line_token)
{
  test_type_of_a_token("\n", data::token_type::new_line);
}

#ifdef TEST_KEYWORD
#error TEST_KEYWORD already defined
#endif
#define TEST_KEYWORD(name)                                                     \
  JUST_TEST_CASE(test_type_of_##name##_keyword_token)                          \
  {                                                                            \
    test_type_of_a_token(#name, data::token_type::keyword_##name);             \
  }

TEST_KEYWORD(asm)
TEST_KEYWORD(auto)
TEST_KEYWORD(bool)
TEST_KEYWORD(break)
TEST_KEYWORD(case)
TEST_KEYWORD(catch)
TEST_KEYWORD(char)
TEST_KEYWORD(class)
TEST_KEYWORD(const)
TEST_KEYWORD(constexpr)
TEST_KEYWORD(const_cast)
TEST_KEYWORD(continue)
TEST_KEYWORD(default)
TEST_KEYWORD(delete)
TEST_KEYWORD(do)
TEST_KEYWORD(double)
TEST_KEYWORD(dynamic_cast)
TEST_KEYWORD(else)
TEST_KEYWORD(enum)
TEST_KEYWORD(explicit)
TEST_KEYWORD(export)
TEST_KEYWORD(extern)
TEST_KEYWORD(float)
TEST_KEYWORD(for)
TEST_KEYWORD(friend)
TEST_KEYWORD(goto)
TEST_KEYWORD(if)
TEST_KEYWORD(inline)
TEST_KEYWORD(int)
TEST_KEYWORD(long)
TEST_KEYWORD(mutable)
TEST_KEYWORD(namespace)
TEST_KEYWORD(new)
TEST_KEYWORD(operator)
TEST_KEYWORD(private)
TEST_KEYWORD(protected)
TEST_KEYWORD(public)
TEST_KEYWORD(register)
TEST_KEYWORD(reinterpret_cast)
TEST_KEYWORD(return )
TEST_KEYWORD(short)
TEST_KEYWORD(signed)
TEST_KEYWORD(sizeof)
TEST_KEYWORD(static)
TEST_KEYWORD(static_cast)
TEST_KEYWORD(struct)
TEST_KEYWORD(switch)
TEST_KEYWORD(template)
TEST_KEYWORD(this)
TEST_KEYWORD(throw)
TEST_KEYWORD(try)
TEST_KEYWORD(typedef)
TEST_KEYWORD(typeid)
TEST_KEYWORD(typename)
TEST_KEYWORD(union)
TEST_KEYWORD(unsigned)
TEST_KEYWORD(using)
TEST_KEYWORD(virtual)
TEST_KEYWORD(void)
TEST_KEYWORD(volatile)
TEST_KEYWORD(wchar_t)
TEST_KEYWORD(while)

#undef TEST_KEYWORD

#ifdef TEST_OPERATOR
#error TEST_OPERATOR already defined
#endif
#define TEST_OPERATOR(name, code)                                              \
  JUST_TEST_CASE(test_type_of_##name##_operator_token)                         \
  {                                                                            \
    test_type_of_a_token(code, data::token_type::operator_##name);             \
  }

#ifdef TEST_OPERATOR_ALT
#error TEST_OPERATOR_ALT already defined
#endif
#define TEST_OPERATOR_ALT(name, code)                                          \
  JUST_TEST_CASE(test_type_of_##name##_alt_operator_token)                     \
  {                                                                            \
    test_type_of_a_token(code, data::token_type::operator_##name);             \
  }

#ifdef TEST_OPERATOR_TRIGRAPH
#error TEST_OPERATOR_TRIGRAPH already defined
#endif
#define TEST_OPERATOR_TRIGRAPH(name, code)                                     \
  JUST_TEST_CASE(test_type_of_##name##_trigraph_operator_token)                \
  {                                                                            \
    test_type_of_a_token(code, data::token_type::operator_##name);             \
  }

TEST_OPERATOR(bitwise_and, "&")
TEST_OPERATOR_ALT(bitwise_and, "bitand")
TEST_OPERATOR(logical_and, "&&")
TEST_OPERATOR_ALT(logical_and, "and")
TEST_OPERATOR(assign, "=")
TEST_OPERATOR(bitwise_and_assign, "&=")
TEST_OPERATOR_ALT(bitwise_and_assign, "and_eq")
TEST_OPERATOR(bitwise_or, "|")
TEST_OPERATOR_ALT(bitwise_or, "bitor")
TEST_OPERATOR(bitwise_or_assign, "|=")
TEST_OPERATOR_ALT(bitwise_or_assign, "or_eq")
TEST_OPERATOR(bitwise_xor, "^")
TEST_OPERATOR_ALT(bitwise_xor, "xor")
TEST_OPERATOR(bitwise_xor_assign, "^=")
TEST_OPERATOR_ALT(bitwise_xor_assign, "xor_eq")
TEST_OPERATOR(comma, ",")
TEST_OPERATOR(colon, ":")
TEST_OPERATOR(divide, "/")
TEST_OPERATOR(divide_assign, "/=")
TEST_OPERATOR(dot, ".")
TEST_OPERATOR(dotstar, ".*")
TEST_OPERATOR(ellipsis, "...")
TEST_OPERATOR(equal, "==")
TEST_OPERATOR(greater, ">")
TEST_OPERATOR(greater_equal, ">=")
TEST_OPERATOR(left_brace, "{")
TEST_OPERATOR_ALT(left_brace, "<%")
TEST_OPERATOR_TRIGRAPH(left_brace, "\?\?<")
TEST_OPERATOR(less, "<")
TEST_OPERATOR(less_equal, "<=")
TEST_OPERATOR(left_paren, "(")
TEST_OPERATOR(left_bracket, "[")
TEST_OPERATOR_ALT(left_bracket, "<:")
TEST_OPERATOR_TRIGRAPH(left_bracket, "\?\?(")
TEST_OPERATOR(minus, "-")
TEST_OPERATOR(minus_assign, "-=")
TEST_OPERATOR(minus_minus, "--")
TEST_OPERATOR(modulo, "%")
TEST_OPERATOR(modulo_assign, "%=")
TEST_OPERATOR(logical_not, "!")
TEST_OPERATOR_ALT(logical_not, "not")
TEST_OPERATOR(not_equal, "!=")
TEST_OPERATOR_ALT(not_equal, "not_eq")
TEST_OPERATOR(logical_or, "||")
TEST_OPERATOR_ALT(logical_or, "or")
TEST_OPERATOR(plus, "+")
TEST_OPERATOR(plus_assign, "+=")
TEST_OPERATOR(plus_plus, "++")
TEST_OPERATOR(arrow, "->")
TEST_OPERATOR(arrow_star, "->*")
TEST_OPERATOR(question_mark, "?")
TEST_OPERATOR(right_brace, "}")
TEST_OPERATOR_ALT(right_brace, "%>")
TEST_OPERATOR_TRIGRAPH(right_brace, "\?\?>")
TEST_OPERATOR(right_paren, ")")
TEST_OPERATOR(right_bracket, "]")
TEST_OPERATOR_ALT(right_bracket, ":>")
TEST_OPERATOR_TRIGRAPH(right_bracket, "\?\?)")
TEST_OPERATOR(colon_colon, "::")
TEST_OPERATOR(semicolon, ";")
TEST_OPERATOR(left_shift, "<<")
TEST_OPERATOR(left_shift_assign, "<<=")
TEST_OPERATOR(right_shift, ">>")
TEST_OPERATOR(right_shift_assign, ">>=")
TEST_OPERATOR(star, "*")
TEST_OPERATOR(bitwise_not, "~")
TEST_OPERATOR_ALT(bitwise_not, "compl")
TEST_OPERATOR(star_assign, "*=")
TEST_OPERATOR(pound_pound, "##")
TEST_OPERATOR_ALT(pound_pound, "%:%:")
TEST_OPERATOR_TRIGRAPH(pound_pound, "\?\?=\?\?=")
TEST_OPERATOR(pound, "#")
TEST_OPERATOR_ALT(pound, "%:")
TEST_OPERATOR_TRIGRAPH(pound, "\?\?=")

#undef TEST_OPERATOR_TRIGRAPH
#undef TEST_OPERATOR_ALT
#undef TEST_OPERATOR

#ifdef TEST_PP_TOKEN
#error TEST_PP_TOKEN already defined
#endif
#define TEST_PP_TOKEN(name)                                                    \
  JUST_TEST_CASE(test_type_of_p_##name##_token)                                \
  {                                                                            \
    test_type_of_a_token("#" #name, data::token_type::p_##name);               \
    test_type_of_a_token("#  " #name, data::token_type::p_##name);             \
  }

TEST_PP_TOKEN(define)
TEST_PP_TOKEN(if)
TEST_PP_TOKEN(ifdef)
TEST_PP_TOKEN(ifndef)
TEST_PP_TOKEN(else)
TEST_PP_TOKEN(elif)
TEST_PP_TOKEN(endif)
TEST_PP_TOKEN(error)
TEST_PP_TOKEN(line)
TEST_PP_TOKEN(pragma)
TEST_PP_TOKEN(undef)
TEST_PP_TOKEN(warning)
TEST_PP_TOKEN(include)

#undef TEST_PP_TOKEN

JUST_TEST_CASE(test_error_flag_is_set)
{
  std::unique_ptr<iface::tokeniser> t =
      create_wave_tokeniser("/* unclosed comment");

  while (t->has_further_tokens())
  {
    t->move_to_next_token();
  }

  JUST_ASSERT(t->was_error());
}

JUST_TEST_CASE(test_constexpr_is_in_keyword_category)
{
  test_category_of_a_token("constexpr", data::token_category::keyword);
}
