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

#include "util.hpp"

#include <just/test.hpp>

using namespace metashell;

JUST_TEST_CASE(test_basic_type_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("void"), get_output("void"));
  JUST_ASSERT_EQUAL(type("bool"), get_output("bool"));
  JUST_ASSERT_EQUAL(type("char"), get_output("char"));
  JUST_ASSERT_EQUAL(type("unsigned char"), get_output("unsigned char"));
  JUST_ASSERT_EQUAL(type("char16_t"), get_output("char16_t"));
  JUST_ASSERT_EQUAL(type("char32_t"), get_output("char32_t"));
  JUST_ASSERT_EQUAL(type("unsigned short"), get_output("unsigned short"));
  JUST_ASSERT_EQUAL(type("unsigned int"), get_output("unsigned int"));
  JUST_ASSERT_EQUAL(type("unsigned long"), get_output("unsigned long"));
  JUST_ASSERT_EQUAL(
    type("unsigned long long"),
    get_output("unsigned long long")
  );
  JUST_ASSERT_EQUAL(type("signed char"), get_output("signed char"));
  JUST_ASSERT_EQUAL(type("wchar_t"), get_output("wchar_t"));
  JUST_ASSERT_EQUAL(type("short"), get_output("short"));
  JUST_ASSERT_EQUAL(type("int"), get_output("int"));
  JUST_ASSERT_EQUAL(type("long"), get_output("long"));
  JUST_ASSERT_EQUAL(type("long long"), get_output("long long"));
  JUST_ASSERT_EQUAL(type("float"), get_output("float"));
  JUST_ASSERT_EQUAL(type("double"), get_output("double"));
  JUST_ASSERT_EQUAL(type("long double"), get_output("long double"));
  JUST_ASSERT_EQUAL(
    type("nullptr_t"),
    get_output("std::nullptr_t", "#include <cstddef>")
  );
}

JUST_TEST_CASE(test_qualified_type_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("const bool"), get_output("const bool"));
}

JUST_TEST_CASE(test_pointer_type_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("int *"), get_output("int*"));
  JUST_ASSERT_EQUAL(type("const int *"), get_output("const int*"));
  JUST_ASSERT_EQUAL(type("int *const"), get_output("int * const"));
  JUST_ASSERT_EQUAL(type("int **"), get_output("int**"));
  JUST_ASSERT_EQUAL(type("const int **"), get_output("int const**"));
  JUST_ASSERT_EQUAL(type("int *const *"), get_output("int*const*"));
  JUST_ASSERT_EQUAL(type("int **const"), get_output("int** const"));
  JUST_ASSERT_EQUAL(type("int *const *const"), get_output("int* const* const"));
  JUST_ASSERT_EQUAL(
    type("volatile int *const *const"),
    get_output("int volatile* const* const")
  );
}

JUST_TEST_CASE(test_type_alias_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("double"), get_output("foo", "typedef double foo;"));
}

JUST_TEST_CASE(test_array_type_pretty_printing)
{
  JUST_ASSERT_EQUAL(
    type("float [3]"),
    get_output("decltype(foo)", "float foo[3];")
  );
  JUST_ASSERT_EQUAL(
    type("float [3][4]"),
    get_output("decltype(foo)", "float foo[3][4];")
  );
  JUST_ASSERT_EQUAL(
    type("float [3][4][5]"),
    get_output("decltype(foo)", "float foo[3][4][5];")
  );
  JUST_ASSERT_EQUAL(
    type("float const[3]"),
    get_output("decltype(foo)", "const float foo[] = {1, 2, 3};")
  );

  JUST_ASSERT_EQUAL(
    type("float (*)[3]"),
    get_output("decltype(&foo)", "float foo[] = {1, 2, 3};")
  );

  JUST_ASSERT_EQUAL(
    type("float const (*)[3]"),
    get_output("decltype(&foo)", "const float foo[] = {1, 2, 3};")
  );

  JUST_ASSERT_EQUAL(
    type("void (*(*)[3])(int, char, double)"),
    get_output(
      "decltype(&foo)",
      "typedef void (*fp)(int, char, double);"
      "fp foo[3];"
    )
  );

  JUST_ASSERT_EQUAL(
    type("int const (*const (*)[3])[2]"),
    get_output(
      "decltype(&foo)",
      "const int bar[2] = {0, 0};"
      "const decltype(&bar) foo[3] = {0, 0, 0};"
    )
  );

  JUST_ASSERT_EQUAL(
    type("void (*(*(*)[2])[3])(int, char, double)"),
    get_output(
      "decltype(&foo)",
      "typedef void (*fp)(int, char, double);"
      "fp bar[3];"
      "decltype(&bar) foo[2];"
    )
  );

}

JUST_TEST_CASE(test_fun_ptr_pretty_printing)
{
  JUST_ASSERT_EQUAL(
    type("bool (*)(long, double, char)"),
    get_output("foo", "typedef bool (*foo)(long, double, char);")
  );

  JUST_ASSERT_EQUAL(
    type("bool (*const)(long, double, char)"),
    get_output("foo", "typedef bool (* const foo)(long, double, char);")
  );

  JUST_ASSERT_EQUAL(
    type("bool (**)(long, double, char)"),
    get_output(
      "decltype(&f)",
      "typedef bool (*foo)(long, double, char);"
      "foo f;"
    )
  );
}

JUST_TEST_CASE(test_record_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("foo"), get_output("foo", "struct foo {};"));
  JUST_ASSERT_EQUAL(type("foo"), get_output("foo", "class foo {};"));

  JUST_ASSERT_EQUAL(
    type("ns1::ns2::foo"),
    get_output(
      "ns1::ns2::foo",
      "namespace ns1"
      "{"
        "namespace ns2"
        "{"
          "struct foo {};"
        "}"
      "}"
    )
  );
}

JUST_TEST_CASE(test_enum_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("foo"), get_output("foo", "enum foo {a, b, c};"));

  JUST_ASSERT_EQUAL(
    type("ns1::ns2::foo"),
    get_output(
      "ns1::ns2::foo",
      "namespace ns1"
      "{"
        "namespace ns2"
        "{"
          "enum foo {a, b, c};"
        "}"
      "}"
    )
  );
}

JUST_TEST_CASE(test_template_instance_pretty_printing)
{
  JUST_ASSERT_EQUAL(
    type("foo<char, double>"),
    get_output(
      "foo<char, double>",
      "template <class A, class B>"
      "struct foo {};"
    )
  );

  JUST_ASSERT_EQUAL(
    type("foo<13>"),
    get_output(
      "foo<11 + 2>",
      "template <int N>"
      "struct foo {};"
    )
  );
}

JUST_TEST_CASE(test_reference_type_pretty_printing)
{
  JUST_ASSERT_EQUAL(type("int &"), get_output("int&"));
  JUST_ASSERT_EQUAL(type("int &&"), get_output("int&&"));
}

