#ifndef METASHELL_TEST_METAPROGRAMS_HPP
#define METASHELL_TEST_METAPROGRAMS_HPP

// Metashell - Interactive C++ template metaprogramming shell
// Copyright (C) 2014, Andras Kucsma (andras.kucsma@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version; 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string>

const std::string fibonacci_mp =
  "template <int N> struct fib {"
    "static constexpr int value = fib<N - 2>::value + fib<N - 1>::value;"
  "};"
  "template <> struct fib<0> { static constexpr int value = 0; };"
  "template <> struct fib<1> { static constexpr int value = 1; };"
  "template <int N> struct int_ {};";

const std::string missing_value_fibonacci_mp =
  "template <int N> struct fib {"
    "static constexpr int value = fib<N - 2>::value + fib<N - 1>::value;"
  "};"
  "template <> struct fib<0> {};"
  "template <> struct fib<1> { static constexpr int value = 1; };"
  "template <int N> struct int_ {};";

const std::string fibonacci_with_enum_mp =
  "template <int N> struct fib {"
    "enum ENUM { value = fib<N - 2>::value + fib<N - 1>::value };"
  "};"
  "template <> struct fib<0> { enum ENUM { value = 0 }; };"
  "template <> struct fib<1> { enum ENUM { value = 1 }; };"
  "template <int N> struct int_ {};";

const std::string multi_fibonacci_mp =
  "template <int N>"
  "struct multi_fib {"
    "static constexpr int value ="
        "multi_fib<N - 4>::value +"
        "multi_fib<N - 2>::value +"
        "multi_fib<N - 3>::value +"
        "multi_fib<N - 1>::value;"
  "};"
  "template <> struct multi_fib<0> { static constexpr int value = 1; };"
  "template <> struct multi_fib<1> { static constexpr int value = 1; };"
  "template <> struct multi_fib<2> { static constexpr int value = 1; };"
  "template <> struct multi_fib<3> { static constexpr int value = 1; };"
  "template <int N> struct int_ {};";

const std::string template_specialization_mp =
  "template<int N, int M> struct foo;"
  "template<int N> struct foo<N, 1> {"
    "static constexpr int value = 42 + N;"
  "};"
  "template <int N> struct int_ {};";

const std::string fibonacci_sfinae_mp =
  "template<bool B, class T = void> struct enable_if {};"
  "template<class T> struct enable_if<true, T> { typedef T type; };"

  "namespace v1 {" + fibonacci_mp + "}"
  "namespace v2 {" + fibonacci_mp + "}"

  "template <int N>"
  "typename enable_if<v1::fib<N>::value == 3, void>::type foo() {}"

  "template <int N>"
  "typename enable_if<v2::fib<N>::value != 3, char>::type foo() { return{}; }";

// Using fake unique_ptr in namespace _std to avoid slight signature and
// namespace differences between STLs (like the std::__1 namespace).
const std::string make_unique_sfinae_mp =
  "#include <utility>\n"
  "#include <type_traits>\n"

 R"(
  namespace _std {
    template<class T> struct unique_ptr {
      unique_ptr(T*) {}
    };
    template<class T> struct unique_ptr<T[]> {
      unique_ptr(T*) {}
    };
  }

  template<class T> struct unique_if {
    typedef _std::unique_ptr<T> single_object;
  };

  template<class T> struct unique_if<T[]> {
    typedef _std::unique_ptr<T[]> unknown_bound;
  };

  template<class T, unsigned N> struct unique_if<T[N]> {
    typedef void known_bound;
  };

  template<class T, class... Args>
  typename unique_if<T>::single_object
  make_unique(Args&&... args) {
    return _std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

  template<class T>
  typename unique_if<T>::unknown_bound
  make_unique(unsigned n) {
    typedef typename std::remove_extent<T>::type U;
    return _std::unique_ptr<T>(new U[n]());
  }

  template<class T, class... Args>
  typename unique_if<T>::known_bound
  make_unique(Args&&...) = delete;
)";

#endif


