// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxtypekind.hpp"
#include "cxstring.hpp"

using namespace metashell;

cxtypekind::cxtypekind(CXTypeKind kind_) : _kind(kind_) {}

std::string cxtypekind::spelling() const
{
  switch (_kind)
  {
  case CXType_Void: return "void";
  case CXType_Bool: return "bool";
  case CXType_Char_U: // the char type is unsigned
  case CXType_Char_S: // the char type is signed
    return "char";
  case CXType_UChar: return "unsigned char";
  case CXType_Char16: return "char16_t";
  case CXType_Char32: return "char32_t";
  case CXType_UShort: return "unsigned short";
  case CXType_UInt: return "unsigned int";
  case CXType_ULong: return "unsigned long";
  case CXType_ULongLong: return "unsigned long long";
  case CXType_UInt128: return "uint128_t"; 
  case CXType_SChar: return "signed char";
  case CXType_WChar: return "wchar_t";
  case CXType_Short: return "short";
  case CXType_Int: return "int";
  case CXType_Long: return "long";
  case CXType_LongLong: return "long long";
  case CXType_Int128: return "int128_t";
  case CXType_Float: return "float";
  case CXType_Double: return "double";
  case CXType_LongDouble: return "long double";
  case CXType_NullPtr: return "nullptr_t";

  case CXType_Invalid:
  case CXType_Unexposed:
  case CXType_Overload:
  case CXType_Dependent:
  case CXType_ObjCId:
  case CXType_ObjCClass:
  case CXType_ObjCSel:
  case CXType_Complex:
  case CXType_Pointer:
  case CXType_BlockPointer:
  case CXType_LValueReference:
  case CXType_RValueReference:
  case CXType_Record:
  case CXType_Enum:
  case CXType_Typedef:
  case CXType_ObjCInterface:
  case CXType_ObjCObjectPointer:
  case CXType_FunctionNoProto:
  case CXType_FunctionProto:
  case CXType_ConstantArray:
  case CXType_Vector:
    // not supported
    return cxstring(clang_getTypeKindSpelling(_kind));
  }
}

bool cxtypekind::operator==(const cxtypekind& k_) const
{
  return _kind == k_._kind;
}

CXTypeKind cxtypekind::value() const
{
  return _kind;
}


