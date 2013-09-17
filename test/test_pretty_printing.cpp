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

#define BOOST_TEST_MODULE metashell

#include "util.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_basic_type_pretty_printing)
{
  BOOST_CHECK_EQUAL("void", get_output("void"));
  BOOST_CHECK_EQUAL("bool", get_output("bool"));
  BOOST_CHECK_EQUAL("char", get_output("char"));
  BOOST_CHECK_EQUAL("unsigned char", get_output("unsigned char"));
  BOOST_CHECK_EQUAL("char16_t", get_output("char16_t"));
  BOOST_CHECK_EQUAL("char32_t", get_output("char32_t"));
  BOOST_CHECK_EQUAL("unsigned short", get_output("unsigned short"));
  BOOST_CHECK_EQUAL("unsigned int", get_output("unsigned int"));
  BOOST_CHECK_EQUAL("unsigned long", get_output("unsigned long"));
  BOOST_CHECK_EQUAL("unsigned long long", get_output("unsigned long long"));
  BOOST_CHECK_EQUAL("signed char", get_output("signed char"));
  BOOST_CHECK_EQUAL("wchar_t", get_output("wchar_t"));
  BOOST_CHECK_EQUAL("short", get_output("short"));
  BOOST_CHECK_EQUAL("int", get_output("int"));
  BOOST_CHECK_EQUAL("long", get_output("long"));
  BOOST_CHECK_EQUAL("long long", get_output("long long"));
  BOOST_CHECK_EQUAL("float", get_output("float"));
  BOOST_CHECK_EQUAL("double", get_output("double"));
  BOOST_CHECK_EQUAL("long double", get_output("long double"));
  BOOST_CHECK_EQUAL(
    "nullptr_t",
    get_output("std::nullptr_t", "#include <cstddef>")
  );
}

BOOST_AUTO_TEST_CASE(test_qualified_type_pretty_printing)
{
  BOOST_CHECK_EQUAL("const bool", get_output("const bool"));
}

BOOST_AUTO_TEST_CASE(test_pointer_type_pretty_printing)
{
  BOOST_CHECK_EQUAL("int *", get_output("int*"));
  BOOST_CHECK_EQUAL("const int *", get_output("const int*"));
  BOOST_CHECK_EQUAL("int *const", get_output("int * const"));
  BOOST_CHECK_EQUAL("int **", get_output("int**"));
  BOOST_CHECK_EQUAL("const int **", get_output("int const**"));
  BOOST_CHECK_EQUAL("int *const *", get_output("int*const*"));
  BOOST_CHECK_EQUAL("int **const", get_output("int** const"));
  BOOST_CHECK_EQUAL("int *const *const", get_output("int* const* const"));
  BOOST_CHECK_EQUAL(
    "volatile int *const *const",
    get_output("int volatile* const* const")
  );
}

BOOST_AUTO_TEST_CASE(test_type_alias_pretty_printing)
{
  BOOST_CHECK_EQUAL("double", get_output("foo", "typedef double foo;"));
}

BOOST_AUTO_TEST_CASE(test_array_type_pretty_printing)
{
  BOOST_CHECK_EQUAL("float [3]", get_output("decltype(foo)", "float foo[3];"));
  BOOST_CHECK_EQUAL(
    "float [3][4]",
    get_output("decltype(foo)", "float foo[3][4];")
  );
  BOOST_CHECK_EQUAL(
    "float [3][4][5]",
    get_output("decltype(foo)", "float foo[3][4][5];")
  );
  BOOST_CHECK_EQUAL(
    "float const[3]",
    get_output("decltype(foo)", "const float foo[] = {1, 2, 3};")
  );

  BOOST_CHECK_EQUAL(
    "float (*)[3]",
    get_output("decltype(&foo)", "float foo[] = {1, 2, 3};")
  );

  BOOST_CHECK_EQUAL(
    "float const (*)[3]",
    get_output("decltype(&foo)", "const float foo[] = {1, 2, 3};")
  );

  BOOST_CHECK_EQUAL(
    "void (*(*)[3])(int, char, double)",
    get_output(
      "decltype(&foo)",
      "typedef void (*fp)(int, char, double);"
      "fp foo[3];"
    )
  );

  BOOST_CHECK_EQUAL(
    "int const (*const (*)[3])[2]",
    get_output(
      "decltype(&foo)",
      "const int bar[2] = {0, 0};"
      "const decltype(&bar) foo[3] = {0, 0, 0};"
    )
  );

  BOOST_CHECK_EQUAL(
    "void (*(*(*)[2])[3])(int, char, double)",
    get_output(
      "decltype(&foo)",
      "typedef void (*fp)(int, char, double);"
      "fp bar[3];"
      "decltype(&bar) foo[2];"
    )
  );

}

BOOST_AUTO_TEST_CASE(test_fun_ptr_pretty_printing)
{
  BOOST_CHECK_EQUAL(
    "bool (*)(long, double, char)",
    get_output("foo", "typedef bool (*foo)(long, double, char);")
  );

  BOOST_CHECK_EQUAL(
    "bool (*const)(long, double, char)",
    get_output("foo", "typedef bool (* const foo)(long, double, char);")
  );

  BOOST_CHECK_EQUAL(
    "bool (**)(long, double, char)",
    get_output(
      "decltype(&f)",
      "typedef bool (*foo)(long, double, char);"
      "foo f;"
    )
  );
}

BOOST_AUTO_TEST_CASE(test_record_pretty_printing)
{
  BOOST_CHECK_EQUAL("foo", get_output("foo", "struct foo {};"));
  BOOST_CHECK_EQUAL("foo", get_output("foo", "class foo {};"));

  BOOST_CHECK_EQUAL(
    "ns1::ns2::foo",
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

BOOST_AUTO_TEST_CASE(test_enum_pretty_printing)
{
  BOOST_CHECK_EQUAL("foo", get_output("foo", "enum foo {a, b, c};"));

  BOOST_CHECK_EQUAL(
    "ns1::ns2::foo",
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

BOOST_AUTO_TEST_CASE(test_template_instance_pretty_printing)
{
  BOOST_CHECK_EQUAL(
    "foo<char, double>",
    get_output(
      "foo<char, double>",
      "template <class A, class B>"
      "struct foo {};"
    )
  );

  BOOST_CHECK_EQUAL(
    "foo<13>",
    get_output(
      "foo<11 + 2>",
      "template <int N>"
      "struct foo {};"
    )
  );
}

BOOST_AUTO_TEST_CASE(test_reference_type_pretty_printing)
{
  BOOST_CHECK_EQUAL("int &", get_output("int&"));
  BOOST_CHECK_EQUAL("int &&", get_output("int&&"));
}

