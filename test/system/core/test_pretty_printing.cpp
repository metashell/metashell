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

#include <metashell/system_test/prompt.hpp>
#include <metashell/system_test/type.hpp>

#include <metashell/system_test/metashell_instance.hpp>

#include <gtest/gtest.h>

using namespace metashell::system_test;

namespace
{
  json_string get_output(const std::string& input_,
                         const std::string& init_line_ = std::string())
  {
    metashell_instance mi;
    mi.command(init_line_);
    return mi.command(input_).front();
  }
} // namespace

TEST(pretty_printing, basic_type)
{
  ASSERT_EQ(type("void"), get_output("void"));
  ASSERT_EQ(type("bool"), get_output("bool"));
  ASSERT_EQ(type("char"), get_output("char"));
  ASSERT_EQ(type("unsigned char"), get_output("unsigned char"));
  ASSERT_EQ(type("char16_t"), get_output("char16_t"));
  ASSERT_EQ(type("char32_t"), get_output("char32_t"));
  ASSERT_EQ(type("unsigned short"), get_output("unsigned short"));
  ASSERT_EQ(type("unsigned int"), get_output("unsigned int"));
  ASSERT_EQ(type("unsigned long"), get_output("unsigned long"));
  ASSERT_EQ(type("unsigned long long"), get_output("unsigned long long"));
  ASSERT_EQ(type("signed char"), get_output("signed char"));
  ASSERT_EQ(type("wchar_t"), get_output("wchar_t"));
  ASSERT_EQ(type("short"), get_output("short"));
  ASSERT_EQ(type("int"), get_output("int"));
  ASSERT_EQ(type("long"), get_output("long"));
  ASSERT_EQ(type("long long"), get_output("long long"));
  ASSERT_EQ(type("float"), get_output("float"));
  ASSERT_EQ(type("double"), get_output("double"));
  ASSERT_EQ(type("long double"), get_output("long double"));
  ASSERT_EQ(
      type("nullptr_t"), get_output("std::nullptr_t", "#include <cstddef>"));
}

TEST(pretty_printing, qualified_type)
{
  ASSERT_EQ(type("const bool"), get_output("const bool"));
}

TEST(pretty_printing, pointer_type)
{
  ASSERT_EQ(type("int *"), get_output("int*"));
  ASSERT_EQ(type("const int *"), get_output("const int*"));
  ASSERT_EQ(type("int *const"), get_output("int * const"));
  ASSERT_EQ(type("int **"), get_output("int**"));
  ASSERT_EQ(type("const int **"), get_output("int const**"));
  ASSERT_EQ(type("int *const *"), get_output("int*const*"));
  ASSERT_EQ(type("int **const"), get_output("int** const"));
  ASSERT_EQ(type("int *const *const"), get_output("int* const* const"));
  ASSERT_EQ(type("volatile int *const *const"),
            get_output("int volatile* const* const"));
}

TEST(pretty_printing, type_alias)
{
  ASSERT_EQ(type("double"), get_output("foo", "typedef double foo;"));
}

TEST(pretty_printing, array_type)
{
  ASSERT_EQ(type("float [3]"), get_output("decltype(foo)", "float foo[3];"));
  ASSERT_EQ(
      type("float [3][4]"), get_output("decltype(foo)", "float foo[3][4];"));
  ASSERT_EQ(type("float [3][4][5]"),
            get_output("decltype(foo)", "float foo[3][4][5];"));
  ASSERT_EQ(type("float const[3]"),
            get_output("decltype(foo)", "const float foo[] = {1, 2, 3};"));

  ASSERT_EQ(type("float (*)[3]"),
            get_output("decltype(&foo)", "float foo[] = {1, 2, 3};"));

  ASSERT_EQ(type("float const (*)[3]"),
            get_output("decltype(&foo)", "const float foo[] = {1, 2, 3};"));

  ASSERT_EQ(type("void (*(*)[3])(int, char, double)"),
            get_output("decltype(&foo)",
                       "typedef void (*fp)(int, char, double);"
                       "fp foo[3];"));

  ASSERT_EQ(type("int const (*const (*)[3])[2]"),
            get_output("decltype(&foo)",
                       "const int bar[2] = {0, 0};"
                       "const decltype(&bar) foo[3] = {0, 0, 0};"));

  ASSERT_EQ(type("void (*(*(*)[2])[3])(int, char, double)"),
            get_output("decltype(&foo)",
                       "typedef void (*fp)(int, char, double);"
                       "fp bar[3];"
                       "decltype(&bar) foo[2];"));
}

TEST(pretty_printing, fun_ptr)
{
  ASSERT_EQ(type("bool (*)(long, double, char)"),
            get_output("foo", "typedef bool (*foo)(long, double, char);"));

  ASSERT_EQ(
      type("bool (*const)(long, double, char)"),
      get_output("foo", "typedef bool (* const foo)(long, double, char);"));

  ASSERT_EQ(type("bool (**)(long, double, char)"),
            get_output("decltype(&f)",
                       "typedef bool (*foo)(long, double, char);"
                       "foo f;"));
}

TEST(pretty_printing, record)
{
  ASSERT_EQ(type("foo"), get_output("foo", "struct foo {};"));
  ASSERT_EQ(type("foo"), get_output("foo", "class foo {};"));

  ASSERT_EQ(type("ns1::ns2::foo"), get_output("ns1::ns2::foo",
                                              "namespace ns1"
                                              "{"
                                              "namespace ns2"
                                              "{"
                                              "struct foo {};"
                                              "}"
                                              "}"));
}

TEST(pretty_printing, enums)
{
  ASSERT_EQ(type("foo"), get_output("foo", "enum foo {a, b, c};"));

  ASSERT_EQ(type("ns1::ns2::foo"), get_output("ns1::ns2::foo",
                                              "namespace ns1"
                                              "{"
                                              "namespace ns2"
                                              "{"
                                              "enum foo {a, b, c};"
                                              "}"
                                              "}"));
}

TEST(pretty_printing, template_instance)
{
  ASSERT_EQ(type("foo<char, double>"),
            get_output("foo<char, double>",
                       "template <class A, class B> struct foo {};"));

  ASSERT_EQ(type("foo<13>"),
            get_output("foo<11 + 2>", "template <int N> struct foo {};"));
}

TEST(pretty_printing, reference_type)
{
  ASSERT_EQ(type("int &"), get_output("int&"));
  ASSERT_EQ(type("int &&"), get_output("int&&"));
}
