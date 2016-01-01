// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2015, Andras Kucsma (andras.kucsma@gmail.com)
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

#include <just/test.hpp>

#include <metashell/metashell.hpp>

using namespace metashell;

JUST_TEST_CASE(test_repair_string_type_1)
{
  JUST_ASSERT_EQUAL("bool", repair_type_string("_Bool"));
}

JUST_TEST_CASE(test_repair_string_type_2)
{
  JUST_ASSERT_EQUAL("<bool", repair_type_string("<_Bool"));
}

JUST_TEST_CASE(test_repair_string_type_3)
{
  JUST_ASSERT_EQUAL("bool>", repair_type_string("_Bool>"));
}

JUST_TEST_CASE(test_repair_string_type_4)
{
  JUST_ASSERT_EQUAL(" bool>", repair_type_string(" _Bool>"));
}

JUST_TEST_CASE(test_repair_string_type_5)
{
  JUST_ASSERT_EQUAL("a_Bool", repair_type_string("a_Bool"));
}

JUST_TEST_CASE(test_repair_string_type_6)
{
  JUST_ASSERT_EQUAL("_Boola", repair_type_string("_Boola"));
}

JUST_TEST_CASE(test_repair_string_type_7)
{
  JUST_ASSERT_EQUAL("b_Boola", repair_type_string("b_Boola"));
}

JUST_TEST_CASE(test_repair_string_type_8)
{
  JUST_ASSERT_EQUAL("X", repair_type_string("struct X"));
}

JUST_TEST_CASE(test_repair_string_type_9)
{
  JUST_ASSERT_EQUAL("X", repair_type_string("class X"));
}

JUST_TEST_CASE(test_repair_string_type_10)
{
  JUST_ASSERT_EQUAL("X", repair_type_string("union X"));
}

JUST_TEST_CASE(test_repair_string_type_11)
{
  JUST_ASSERT_EQUAL("X", repair_type_string("enum X"));
}

JUST_TEST_CASE(test_repair_string_type_12)
{
  JUST_ASSERT_EQUAL("X<Y>", repair_type_string("struct X<struct Y>"));
}

JUST_TEST_CASE(test_type_from_ast_string_int)
{
  std::string ast = R"(
TranslationUnitDecl 0x7feb540124c0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7feb5300cb10 <<stdin>:1:1, col:59> col:59 __metashell_v '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' callinit
| `-CXXConstructExpr 0x7feb5300cbb8 <col:59> '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' 'void (void) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("int", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_int_no_noexcept)
{
  std::string ast = R"(
TranslationUnitDecl 0x7feb540124c0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7feb5300cb10 <<stdin>:1:1, col:59> col:59 __metashell_v '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' callinit
| `-CXXConstructExpr 0x7feb5300cbb8 <col:59> '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' 'void (void)'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("int", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_bool)
{
  std::string ast = R"(
TranslationUnitDecl 0x7f9c5c8278c0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7f9c5d000910 <<stdin>:1:1, col:60> col:60 __metashell_v '::metashell::impl::wrap< ::metashell::format<_Bool>::type>':'struct metashell::impl::wrap<_Bool>' callinit
| `-CXXConstructExpr 0x7f9c5d0009b8 <col:60> '::metashell::impl::wrap< ::metashell::format<_Bool>::type>':'struct metashell::impl::wrap<_Bool>' 'void (void) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("bool", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_int_int_struct)
{
  std::string ast = R"(
TranslationUnitDecl 0x7f9cca00eac0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7f9cca020470 <<stdin>:1:1, col:65> col:65 __metashell_v '::metashell::impl::wrap< ::metashell::format<int_<int> >::type>':'struct metashell::impl::wrap<struct int_<int> >' callinit
| `-CXXConstructExpr 0x7f9cca020518 <col:65> '::metashell::impl::wrap< ::metashell::format<int_<int> >::type>':'struct metashell::impl::wrap<struct int_<int> >' 'void (void) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("int_<int>", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_int_int_class)
{
  std::string ast = R"(
TranslationUnitDecl 0x7f9cca00eac0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7f9cca020470 <<stdin>:1:1, col:65> col:65 __metashell_v '::metashell::impl::wrap< ::metashell::format<int_<int> >::type>':'struct metashell::impl::wrap<class int_<int> >' callinit
| `-CXXConstructExpr 0x7f9cca020518 <col:65> '::metashell::impl::wrap< ::metashell::format<int_<int> >::type>':'struct metashell::impl::wrap<class int_<int> >' 'void (void) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("int_<int>", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_enum)
{
  std::string ast = R"(
TranslationUnitDecl 0x7fb81c0278c0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7fb81d802860 <<stdin>:1:1, col:57> col:57 __metashell_v '::metashell::impl::wrap< ::metashell::format<X>::type>':'struct metashell::impl::wrap<enum X>' callinit
| `-CXXConstructExpr 0x7fb81d802908 <col:57> '::metashell::impl::wrap< ::metashell::format<X>::type>':'struct metashell::impl::wrap<enum X>' 'void (void) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("X", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_union)
{
  std::string ast = R"(
TranslationUnitDecl 0x7fd6c3031cc0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7fd6c304e460 <<stdin>:1:1, col:57> col:57 __metashell_v '::metashell::impl::wrap< ::metashell::format<X>::type>':'struct metashell::impl::wrap<union X>' callinit
| `-CXXConstructExpr 0x7fd6c304e508 <col:57> '::metashell::impl::wrap< ::metashell::format<X>::type>':'struct metashell::impl::wrap<union X>' 'void (void) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("X", get_type_from_ast_string(ast));
}

// This happens on windows
JUST_TEST_CASE(test_type_from_ast_string_int_with_attribute_thiscall)
{
  std::string ast = R"(
TranslationUnitDecl 0x7feb540124c0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7feb5300cb10 <<stdin>:1:1, col:59> col:59 __metashell_v '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' callinit
| `-CXXConstructExpr 0x7feb5300cbb8 <col:59> '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' 'void (void) __attribute__((thiscall)) noexcept'
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL("int", get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_with_extra_trailing_lines)
{
  std::string ast = R"(
TranslationUnitDecl 0x7fee54031cc0 <<invalid sloc>> <invalid sloc>
|-VarDecl 0x7fee5486c0e0 <<stdin>:1:1, col:40> col:40 __metashell_v '::metashell::impl::wrap<str_hello_t>':'struct metashell::impl::wrap<struct boost::mpl::list_c<char, 104, 101, 108, 108, 111, 32, 9, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807> >' callinit
| `-CXXConstructExpr 0x7fee548a9500 <col:40> '::metashell::impl::wrap<str_hello_t>':'struct metashell::impl::wrap<struct boost::mpl::list_c<char, 104, 101, 108, 108, 111, 32, 9, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807, 9223372036854775807> >' 'void (void) noexcept'
|-LinkageSpecDecl 0x7fee549fdbb0 </Users/r0mai/workspace/metashell/bin/app/../include/metashell/libcxx/string:1287:12> col:12 implicit C
| `-FunctionDecl 0x7fee549fdc00 <col:12> col:12 implicit used __builtin_memcpy 'void *(void *, const void *, unsigned long)' extern
|   |-ParmVarDecl 0x7fee549fdca0 <<invalid sloc>> <invalid sloc> 'void *'
|   |-ParmVarDecl 0x7fee549fdd00 <<invalid sloc>> <invalid sloc> 'const void *'
|   |-ParmVarDecl 0x7fee549fdd60 <<invalid sloc>> <invalid sloc> 'unsigned long'
|   `-NoThrowAttr 0x7fee549fddd0 <col:12> Implicit
`-<undeserialized declarations>
)";
  JUST_ASSERT_EQUAL(
      "boost::mpl::list_c<char, 104, 101, 108, 108, 111, 32, 9, "
      "9223372036854775807, 9223372036854775807, 9223372036854775807, "
      "9223372036854775807, 9223372036854775807, 9223372036854775807, "
      "9223372036854775807, 9223372036854775807, 9223372036854775807, "
      "9223372036854775807, 9223372036854775807, 9223372036854775807, "
      "9223372036854775807>",
      get_type_from_ast_string(ast));
}

JUST_TEST_CASE(test_type_from_ast_string_no_pch)
{
  std::string ast = R"(
TranslationUnitDecl 0x7febd883d2c0 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x7febd883d800 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
|-TypedefDecl 0x7febd883d860 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
|-TypedefDecl 0x7febd883dc20 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
|-NamespaceDecl 0x7febd883dc70 </tmp/just-zdw7cr/metashell_environment.hpp:5:1, col:587> col:11 metashell
| |-NamespaceDecl 0x7febd883dcd0 <col:23, col:266> col:33 impl
| | |-ClassTemplateDecl 0x7febd883de70 <col:40, col:72> col:66 wrap
| | | |-TemplateTypeParmDecl 0x7febd883dd30 <col:50, col:56> col:56 class T
| | | |-CXXRecordDecl 0x7febd883dde0 <col:59, col:72> col:66 struct wrap definition
| | | | `-CXXRecordDecl 0x7febd8874930 <col:59, col:66> col:66 implicit struct wrap
| | | `-ClassTemplateSpecializationDecl 0x7febd887ee80 <col:40, col:72> col:66 struct wrap definition
| | |   |-TemplateArgument type 'int'
| | |   |-CXXRecordDecl 0x7febd887f0a0 prev 0x7febd887ee80 <col:59, col:66> col:66 implicit struct wrap
| | |   |-CXXConstructorDecl 0x7febd887f150 <col:66> col:66 implicit used constexpr wrap 'void (void) noexcept' inline
| | |   | `-CompoundStmt 0x7febd8881ac0 <col:66>
| | |   |-CXXConstructorDecl 0x7febd887f2b0 <col:66> col:66 implicit constexpr wrap 'void (const struct metashell::impl::wrap<int> &)' inline noexcept-unevaluated 0x7febd887f2b0
| | |   | `-ParmVarDecl 0x7febd887f3f0 <col:66> col:66 'const struct metashell::impl::wrap<int> &'
| | |   |-CXXConstructorDecl 0x7febd887f480 <col:66> col:66 implicit constexpr wrap 'void (struct metashell::impl::wrap<int> &&)' inline noexcept-unevaluated 0x7febd887f480
| | |   | `-ParmVarDecl 0x7febd887f600 <col:66> col:66 'struct metashell::impl::wrap<int> &&'
| | |   `-CXXDestructorDecl 0x7febd887f890 <col:66> col:66 implicit referenced ~wrap 'void (void)' inline noexcept-unevaluated 0x7febd887f890
| | |-FunctionTemplateDecl 0x7febd8874f10 <col:75, col:144> col:111 tag_of
| | | |-TemplateTypeParmDecl 0x7febd88749c0 <col:85, col:91> col:91 referenced class T
| | | `-FunctionDecl 0x7febd8874e70 <col:94, col:144> col:111 tag_of 'typename T::tag *(::metashell::impl::wrap<T>)'
| | |   `-ParmVarDecl 0x7febd8874cf0 <col:118, col:143> col:144 '::metashell::impl::wrap<T>':'wrap<T>'
| | |-FunctionDecl 0x7febd8874fb0 <col:147, col:163> col:153 referenced tag_of 'void *(...)'
| | |-ClassTemplateDecl 0x7febd8875160 <col:166, col:192> col:192 remove_ptr
| | | |-TemplateTypeParmDecl 0x7febd8875050 <col:176, col:182> col:182 class T
| | | |-CXXRecordDecl 0x7febd88750d0 <col:185, col:192> col:192 struct remove_ptr
| | | `-ClassTemplateSpecializationDecl 0x7febd887f9c0 <col:204, col:263> col:230 struct remove_ptr definition
| | |   |-TemplateArgument type 'void *'
| | |   |-CXXRecordDecl 0x7febd887fc90 prev 0x7febd887f9c0 <col:223, col:230> col:230 implicit struct remove_ptr
| | |   `-TypedefDecl 0x7febd887fd30 <col:247, col:257> col:257 referenced type 'void':'void'
| | `-ClassTemplatePartialSpecializationDecl 0x7febd8875550 <col:204, col:263> col:230 struct remove_ptr definition
| |   |-TemplateArgument type 'type-parameter-0-0 *'
| |   |-TemplateTypeParmDecl 0x7febd8875380 <col:214, col:220> col:220 referenced class T
| |   |-CXXRecordDecl 0x7febd8875760 <col:223, col:230> col:230 implicit struct remove_ptr
| |   `-TypedefDecl 0x7febd887ce00 <col:247, col:257> col:257 type 'T'
| |-ClassTemplateDecl 0x7febd887cf60 <col:268, col:389> col:296 format_impl
| | |-TemplateTypeParmDecl 0x7febd887ce50 <col:278, col:284> col:284 class Tag
| | |-CXXRecordDecl 0x7febd887ced0 <col:289, col:389> col:296 struct format_impl definition
| | | |-CXXRecordDecl 0x7febd887d1e0 <col:289, col:296> col:296 implicit referenced struct format_impl
| | | |-TypedefDecl 0x7febd887d280 <col:310, col:330> col:330 type 'format_impl<Tag>'
| | | `-ClassTemplateDecl 0x7febd887d410 <col:336, col:386> col:362 apply
| | |   |-TemplateTypeParmDecl 0x7febd887d2d0 <col:346, col:352> col:352 referenced class T
| | |   `-CXXRecordDecl 0x7febd887d380 <col:355, col:386> col:362 struct apply definition
| | |     |-CXXRecordDecl 0x7febd887d690 <col:355, col:362> col:362 implicit struct apply
| | |     `-TypedefDecl 0x7febd887d730 <col:370, col:380> col:380 type 'T'
| | `-ClassTemplateSpecializationDecl 0x7febd887fdf0 <col:268, col:389> col:296 struct format_impl definition
| |   |-TemplateArgument type 'void'
| |   |-CXXRecordDecl 0x7febd8880010 prev 0x7febd887fdf0 <col:289, col:296> col:296 implicit struct format_impl
| |   |-TypedefDecl 0x7febd88800b0 <col:310, col:330> col:330 type 'struct metashell::format_impl<void>'
| |   `-ClassTemplateDecl 0x7febd8880210 <col:336, col:362> col:362 apply
| |     |-TemplateTypeParmDecl 0x7febd8880100 <col:346, col:352> col:352 class T
| |     |-CXXRecordDecl 0x7febd8880180 <col:355, col:362> col:362 struct apply
| |     `-ClassTemplateSpecializationDecl 0x7febd8880440 <col:336, col:362> col:362 struct apply definition
| |       |-TemplateArgument type 'int'
| |       |-CXXRecordDecl 0x7febd8881660 prev 0x7febd8880440 <col:355, col:362> col:362 implicit struct apply
| |       `-TypedefDecl 0x7febd8881730 <col:370, col:380> col:380 referenced type 'int':'int'
| `-ClassTemplateDecl 0x7febd887d890 <col:392, col:584> col:418 format
|   |-TemplateTypeParmDecl 0x7febd887d780 <col:402, col:408> col:408 referenced class T
|   |-CXXRecordDecl 0x7febd887d800 <col:411, col:584> col:418 struct format definition
|   | |-public '::metashell::format_impl<typename ::metashell::impl::remove_ptr<decltype(::metashell::impl::tag_of(::metashell::impl::wrap<T>()))>::type>::apply<T>'
|   | `-CXXRecordDecl 0x7febd887eb40 <col:411, col:418> col:418 implicit struct format
|   `-ClassTemplateSpecializationDecl 0x7febd887ebf0 <col:392, col:584> col:418 struct format definition
|     |-public '::metashell::format_impl<typename ::metashell::impl::remove_ptr<decltype(::metashell::impl::tag_of(::metashell::impl::wrap<int>()))>::type>::apply<int>':'struct metashell::format_impl<void>::apply<int>'
|     |-TemplateArgument type 'int'
|     `-CXXRecordDecl 0x7febd88817b0 prev 0x7febd887ebf0 <col:411, col:418> col:418 implicit struct format
`-VarDecl 0x7febd8881a30 <<stdin>:2:1, col:59> col:59 __metashell_v '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' callinit
  `-CXXConstructExpr 0x7febd8881ad8 <col:59> '::metashell::impl::wrap< ::metashell::format<int>::type>':'struct metashell::impl::wrap<int>' 'void (void) noexcept'
)";
  JUST_ASSERT_EQUAL("int", get_type_from_ast_string(ast));
}
