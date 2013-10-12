# Metashell

[![Build Status](https://secure.travis-ci.org/sabel83/metashell.png?branch=master "Build Status")](http://travis-ci.org/sabel83/metashell)

The goal of this project is to provide an interactive template metaprogramming
shell.

## Motivation

When one starts learning a new programming language such as Haskell, Python or
Erlang an interactive shell (`python` for Python, `ghci` for Haskell, `erl` for
Erlang) is available for experimenting with the language. Later, after getting
familiar with the language, this shell becomes a tool to try code out, verify
test results or learn more about the problem during debugging.

When one starts learning C++ template metaprogramming, he has to use a C++
compiler which was designed to _compile code_ and not to play with template
tricks. People new to template metaprogramming have to learn how to make the
compiler show them the result of simple metaprograms. This makes it more
difficult to get started with template metaprogramming.

Later, after getting familiar with template metaprogramming, developers have to
use the compiler for testing and debugging. Understanding what is going on in a
template metaprogram during debugging is difficult, since the compiler needs to
be forced to present the result of a metaprogram (or a part of it) - and
compilers are not prepared for this. This extra difficulty in debugging makes
simple problems look difficult.

Metashell provides an interactive shell similar to the Python, Haskell and
Erlang shells for template metaprogramming. It uses Clang to evaluate the
metaprograms.

## Building Metashell

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent libraries:
    * libclang
    * Readline
* Install CMake
* In case some elements of the default system include path are missing when
  libclang is used, Metashell has to add them. You need to provide the system
  include path clang uses to Metashell:
    * `tools/clang_default_path > lib/extra_sysinclude.hpp`
* In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
        * Note: on Fedora you need to run `cmake .. -DCLANG_LIBRARYDIR=/usr/lib/llvm`
    * `make`

## Getting started

This section presents how to use Metashell to do simple debugging.

### Evaluating simple expressions

Let's look at how to evaluate the expression `6 + 7`. If you have access to
`python`, `ghci` or `erl`, start it and type `6 + 7` (`6 + 7.` in Erlang) and
press enter. You will see `13`, the result of this expression immediately.

Let's do the same in template metaprogramming. Start `metashell`. In template
metaprogramming the integer numbers are represented by instances of the `int_`
template class - get it by running the following command:

```cpp
> #include <boost/mpl/int.hpp>
```

Addition is implemented by the `plus` metafunction - get it by running the
following command:

```cpp
> #include <boost/mpl/plus.hpp>
```

Note that you can use the arrow keys to re-run earlier commands or slightly
updated versions of them. The following command is not necessary, but makes life
easier:

```cpp
> using namespace boost::mpl;
```

Once you have set your environment up, you can evaluate the expression `6 + 7`
by running the following:

```cpp
> plus<int_<6>, int_<7>>::type
```

You get `mpl_::integral_c<int, 13>`, which is the representation of the value
`13`.

### How about Fibonacci?

You have seen how to do simple arithmetic using Boost.MPL's integral types but
you haven't been able to run the classical Fibonacci example of template
metaprogramming. Let's write it first:

```cpp
> template <int N> struct fib { static constexpr int value = N * fib<N - 1>::value; };
> template <> struct fib<0> { static constexpr int value = 1; };
> template <> struct fib<1> { static constexpr int value = 1; };
```

If you type the above commands into the shell you define the `fib` template
class and its necessary specialisations. If you try to evaluate it, you get an
error:

```cpp
> fib<6>::value
<stdin>:6:26: error: template argument for template type parameter must be a type
```

The problem is that Metashell assumes, that _the result of a template
metaprogram is a type_. And `fib<6>::value` is not a type. It is an integer
value. You need some boxing solution, such as `boost::mpl::int_` to turn it into
a type:

```cpp
> #include <boost/mpl/int.hpp>
> boost::mpl::int_<fib<6>::value>
mpl_::int_<720>
```

In this case we knew that the result is an `int` value. For other values other
boxing classes have to be used. Keeping it all in mind is not convenient.
Metashell offers a tool for working with scalar results in the shell. Run the
following command to get it:

```cpp
> #include <metashell/scalar.hpp>
```

The above header defines the `SCALAR` macro which can be used to work with
expressions evaluating to scalar results. For example:

```cpp
> SCALAR(fib<6>::value)
std::integral_constant<int, 720>
```

The `SCALAR` macro instantiates `std::integral_constant` with the right
arguments.

### Data structures of Boost.MPL

Let's play with the vector implementation Boost.MPL provides. It is defined in
the following header:

```cpp
#include <boost/mpl/vector.hpp>
```

By including it, we get the `vector` template class. Again, to make life easier
let's run:

```cpp
> using namespace boost::mpl;
```

Now let's create a vector:

```cpp
> vector<int, double, char>
```

The shell's response is different than what we'd expect:

```cpp
boost::mpl::vector<int, double, char, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mp
l_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mp
l_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na>
```

`vector` uses default template arguments for simulating variadic templates and
Metashell displays them as well. `mpl_::na` means _there is no such element_.

Let's add a new element to the vector. There is a `push_front` operator for
this defined in the following header:

```cpp
> #include <boost/mpl/push_front.hpp>
```

To add a new element (`void` in this example) to this vector, let's run the
following command:

```cpp
> push_front<vector<int, double, char>, void>::type
```

This calls the `push_front` metafunction to add `void` to the beginning of the
vector. The result is something unexpected again:

```cpp
boost::mpl::v_item<void, boost::mpl::vector<int, double, char, mpl_::na, mpl_::n
a, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::n
a, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na, mpl_::na>, 1>
```

This is not an instance of the `vector` template class. Boost.MPL's vector
implementation uses _tag dispatching_ for identifying vector values.

Metashell provides a header file that can be used to display Boost.MPL's
containers in a way we'd expect to see them. Let's include this header:

```cpp
> #include <metashell/formatter.hpp>
```

This header defines the formatting rules for the Boost.MPL containers. Let's
try running the `push_front` operation again:

```cpp
> push_front<vector<int, double, char>, void>::type
boost_::mpl::vector<void, int, double>
```

This time the result is what we'd expect. Note that it is in the `boost_`
namespace instead of `boost`. The `vector` template class in `boost_` is a
helper template class used for pretty-printing.

The header `metashell/formatter.hpp` loads all formatters that come with
Metashell which may be more than what you need. Among other dependencies the
formatters include the data structures they format, which may slow the shell
down. To get only the formatters (and related data structures) you really need,
you can include them one by one. For example to get the formatter of
`mpl::vector` you need to include `metashell/formatter/vector.hpp`.

### Writing custom formatters

The previous section has presented how to make Metashell format the Boost.MPL
containers properly. Metashell needs to be prepared for other libraries using
tag dispatching as well. This can be done by using _standard C++ code_. Note
that the content of `mpl_formatter.hpp` that did the magic for the Boost.MPL
containers is also standard C++ code.

Metashell declares the following metafunction:

```cpp
namespace metashell
{
  template <class T>
  struct format;
}
```

When Metashell evaluates a template metaprogramming expression, such as
`push_front<vector<int, double, char>, void>::type` it passes the result of it
to this metafunction. The result of this metafunction call is displayed. The
default implementation of this metafunction returns its argument unchanged but
it can be overridden. This is what `mpl_formatter.hpp` does.

#### Using specialisation

One way of overriding it is by specialising it. As an example override, let's
make the shell display the type `int` as `double`:

```cpp
> namespace metashell { template <> struct format<int> { typedef double type; }; }
```

Now let's try evaluating the expression `int`:

```cpp
> int
double
```

The above specialisation of `metashell::format` made Metashell display `double`
instead of `int`.

#### Using tag dispatching

Another way of overriding the formatter is using tag dispatching. The default
implementation of `metashell::format` evaluates
`metashell::format_impl<T::tag>::apply<T>::type` where `T` is `format`'s
argument. When `T::tag` is not defined, `void` is used instead.

Specialising `metashell::format_impl` for a new tag adds support for formatting
values using that tag. These specialisations can be part of the implementation
of a data-type to avoid Metashell users having to include extra headers for
formatting.

#### Embedding custom formatters

Custom formatters can be part of a data type's implementation. Metashell defines
the `__METASHELL` macro, thus the implementation of a data type (eg. a typelist)
 can detect if it is used inside Metashell. For example:

```cpp
/*
 * typelist.hpp
 */

// The definition of the typelist
struct empty_list;

template <class Head, class Tail>
struct cons;

// Metashell formatter
#ifdef __METASHELL
namespace metashell
{
  template <>
  struct format<empty_list>
  {
    // ...
  };

  template <class Head, class Tail>
  struct format<cons<Head, Tail>>
  {
    // ...
  };
}
#endif
```

This example defines the formatter only when the header file is used inside
Metashell. This is useful for debugging and has no impact on the regular users
of the header.

The version of the shell is also available for the headers. Metashell defines
the `__METASHELL_MAJOR`, `__METASHELL_MINOR` and `__METASHELL_PATCH` macros.

## The full list of built-in header files

These header files are available in Metashell:

<table cellpadding='0' cellspacing='0'>
  <tr>
    <td><code>metashell/formatter/deque.hpp</code></td>
    <td>Defines formatting for <code>boost::mpl::deque</code> values</td>
  </tr>
  <tr>
    <td><code>metashell/formatter/list.hpp</code></td>
    <td>Defines formatting for <code>boost::mpl::list</code> values</td>
  </tr>
  <tr>
    <td><code>metashell/formatter/map.hpp</code></td>
    <td>Defines formatting for <code>boost::mpl::map</code> values</td>
  </tr>
  <tr>
    <td><code>metashell/formatter/set.hpp</code></td>
    <td>Defines formatting for <code>boost::mpl::set</code> values</td>
  </tr>
  <tr>
    <td><code>metashell/formatter/vector.hpp</code></td>
    <td>Defines formatting for <code>boost::mpl::vector</code> values</td>
  </tr>
  <tr>
    <td><code>metashell/formatter.hpp</code></td>
    <td>
      Defines formatting for <code>boost::mpl::deque</code>,
      <code>boost::mpl::list</code>, <code>boost::mpl::map</code>,
      <code>boost::mpl::set</code> and <code>boost::mpl::vector</code> values.
    </td>
  </tr>
  <tr>
    <td><code>metashell/scalar.hpp</code></td>
    <td>Defines the <code>SCALAR</code> macro.</td>
  </tr>
</table>

## License

Metashell is published under the
[GNU General Public License, version 3](http://www.gnu.org/licenses/gpl.html).

