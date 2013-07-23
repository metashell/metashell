// Copyright Abel Sinkovics (abel@sinkovics.hu)  2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "cxtype.hpp"
#include "cxcursor.hpp"
#include "cxstring.hpp"

#include <sstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/algorithm/string/join.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/bind.hpp>
#include <boost/iterator/transform_iterator.hpp>

using namespace metashell;

namespace
{
  std::string to_string(long long n_)
  {
    std::ostringstream s;
    s << n_;
    return s.str();
  }

  std::string namespace_spelling(cxcursor c_)
  {
    using boost::algorithm::join;
    using boost::make_iterator_range;
    using std::vector;
    using std::string;

    const vector<string> s(c_.namespace_begin(), c_.namespace_end());
    return join(make_iterator_range(s.rbegin(), s.rend()), "::") + "::";
  }

  std::string unqualified_spelling(cxtype type_)
  {
    using boost::algorithm::join;
    using boost::make_iterator_range;

    if (type_.function_pointer())
    {
      using boost::bind;
      using boost::transform_iterator;
      using boost::function;
      using std::string;

      typedef
        transform_iterator<function<string(cxtype)>, cxtype::arg_iterator>
        it;

      return
        type_.result_type().spelling()
          + "(*)("
          +
            join(
              make_iterator_range(
                it(type_.args_begin(), bind(&cxtype::spelling, _1)),
                it(type_.args_end())
              ),
              ", "
            )
          + ")";
    }
    else
    {
      switch (type_.kind().value())
      {
      case CXType_Pointer:
        {
          cxtype p = type_.pointee_type();
          return p.spelling() + (p.function_pointer() ? "" : "*");
        }
      case CXType_ConstantArray:
        {
          std::vector<std::string> dims;

          cxtype t = type_;
          while (t.kind() == CXType_ConstantArray)
          {
            dims.push_back(to_string(t.num_elements()));
            t = t.element_type();
          }
          return
            t.spelling() + "["
            + join(make_iterator_range(dims.begin(), dims.end()), "][") + "]";
        }
        break;
      case CXType_Record:
      case CXType_Unexposed: // classes in namespaces seem to be this one
                             // and function pointers
      case CXType_Typedef:
      case CXType_Enum:
      case CXType_FunctionProto:
        {
          cxcursor d = type_.declaration();
          return namespace_spelling(d) + d.spelling();
        }
        break;
      default:
        return type_.kind().spelling();
      }
    }
  }

  cxtype get_nth_arg(CXType type_, int n_)
  {
    return cxtype(clang_getArgType(type_, n_));
  }
}

cxtype::cxtype() {}

cxtype::cxtype(CXType type_) : _type(type_) {}

std::string cxtype::spelling() const
{
  const std::string
    const_mf = clang_isConstQualifiedType(_type) ? "const " : "";
  const std::string
    volatile_mf = clang_isVolatileQualifiedType(_type) ? "volatile " : "";
  return const_mf + volatile_mf + unqualified_spelling(*this);
}

cxtype cxtype::pointee_type() const
{
  return cxtype(clang_getPointeeType(_type));
}

cxtype cxtype::canonical_type() const
{
  return cxtype(clang_getCanonicalType(_type));
}

cxtypekind cxtype::kind() const
{
  return cxtypekind(_type.kind);
}

bool cxtype::function_pointer() const
{
  return clang_getNumArgTypes(_type) != -1;
}

cxcursor cxtype::declaration() const
{
  return cxcursor(clang_getTypeDeclaration(_type));
}

cxtype cxtype::element_type() const
{
  return cxtype(clang_getElementType(_type));
}

long long cxtype::num_elements() const
{
  return clang_getNumElements(_type);
}

cxtype cxtype::result_type() const
{
  return cxtype(clang_getResultType(_type));
}

cxtype::arg_iterator cxtype::args_begin() const
{
  return arg_iterator(boost::bind(get_nth_arg, _type, _1), 0);
}

cxtype::arg_iterator cxtype::args_end() const
{
  return
    arg_iterator(
      boost::bind(get_nth_arg, _type, _1),
      clang_getNumArgTypes(_type)
    );
}


