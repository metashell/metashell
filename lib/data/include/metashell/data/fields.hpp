#ifndef METASHELL_DATA_FIELDS_HPP
#define METASHELL_DATA_FIELDS_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2018, Abel Sinkovics (abel@sinkovics.hu)
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

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <iostream>
#include <optional>
#include <tuple>
#include <vector>

namespace metashell
{
  namespace data
  {
    namespace impl
    {
      template <class T>
      void display(std::ostream& out_, const T& t_)
      {
        out_ << t_;
      }

      template <class T>
      void display(std::ostream& out_, const std::vector<T>& v_)
      {
        out_ << "{";
        bool first = true;
        for (const T& t : v_)
        {
          if (first)
          {
            first = false;
          }
          else
          {
            out_ << ", ";
          }
          display(out_, t);
        }
        out_ << "}";
      }

      template <class T>
      void display(std::ostream& out_, const std::optional<T>& t_)
      {
        if (t_)
        {
          display(out_, *t_);
        }
        else
        {
          out_ << "none";
        }
      }
    }
  }
}

#ifdef METASHELL_DATA_FIELDS_FIELD
#error METASHELL_DATA_FIELDS_FIELD already defined
#endif
#define METASHELL_DATA_FIELDS_FIELD(r, data, elem)                             \
  BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem);

#ifdef METASHELL_DATA_FIELDS_ONE
#error METASHELL_DATA_FIELDS_ONE already defined
#endif
#define METASHELL_DATA_FIELDS_ONE(r, data, i, elem)                            \
  BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(2, data, elem)

#ifdef METASHELL_DATA_FIELD_STREAM
#error METASHELL_DATA_FIELD_STREAM already defined
#endif
#define METASHELL_DATA_FIELD_STREAM(r, unused, i, elem)                        \
  BOOST_PP_EXPR_IF(i, out_ << ", ";)                                           \
  ::metashell::data::impl::display(out_, t_.BOOST_PP_TUPLE_ELEM(2, 1, elem));

#ifdef METASHELL_DATA_FIELDS
#error METASHELL_DATA_FIELDS already defined
#endif
#define METASHELL_DATA_FIELDS(type, fields)                                    \
  BOOST_PP_SEQ_FOR_EACH(METASHELL_DATA_FIELDS_FIELD, _, fields)                \
                                                                               \
  std::tuple<BOOST_PP_SEQ_FOR_EACH_I(METASHELL_DATA_FIELDS_ONE, 0, fields)>    \
  to_tuple() const                                                             \
  {                                                                            \
    return std::tuple<BOOST_PP_SEQ_FOR_EACH_I(                                 \
        METASHELL_DATA_FIELDS_ONE, 0, fields)>(                                \
        BOOST_PP_SEQ_FOR_EACH_I(METASHELL_DATA_FIELDS_ONE, 1, fields));        \
  }                                                                            \
                                                                               \
  template <event_kind ToKind>                                                 \
  explicit operator timeless_event_details<ToKind>() const                     \
  {                                                                            \
    return {BOOST_PP_SEQ_FOR_EACH_I(METASHELL_DATA_FIELDS_ONE, 1, fields)};    \
  }                                                                            \
                                                                               \
  friend std::ostream& operator<<(std::ostream& out_, const type& t_)          \
  {                                                                            \
    out_ << "{";                                                               \
    BOOST_PP_SEQ_FOR_EACH_I(METASHELL_DATA_FIELD_STREAM, _, fields)            \
    return out_ << "}";                                                        \
  }

#ifdef METASHELL_DATA_NO_FIELDS
#error METASHELL_DATA_NO_FIELDS already defined
#endif
#define METASHELL_DATA_NO_FIELDS(type)                                         \
  std::tuple<> to_tuple() const { return std::tuple<>(); }                     \
                                                                               \
  friend std::ostream& operator<<(std::ostream& out_, const type&)             \
  {                                                                            \
    return out_ << "{}";                                                       \
  }

#endif
