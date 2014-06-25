# Metashell

[![Build Status](https://secure.travis-ci.org/sabel83/metashell.png?branch=master "Build Status")](http://travis-ci.org/sabel83/metashell)

![](https://raw.github.com/sabel83/metashell/master/metashell.png "Metashell")

[![Download](https://raw.github.com/sabel83/metashell/master/download.png "Download")](https://github.com/sabel83/metashell/releases)

The goal of this project is to provide an interactive template metaprogramming
shell.

## Table of contents

- [Motivation](#motivation)
- [Building Metashell](#building-metashell)
- [Getting started](#getting-started)
    - [Trying Metashell online](#trying-metashell-online)
    - [Evaluating simple expressions](#evaluating-simple-expressions)
    - [How about Fibonacci?](#how-about-fibonacci)
    - [Data structures of Boost.MPL](#data-structures-of-boostmpl)
    - [Writing custom formatters](#writing-custom-formatters)
        - [Using specialisation](#using-specialisation)
        - [Using tag dispatching](#using-tag-dispatching)
        - [Embedding custom formatters](#embedding-custom-formatters)
- [The environment in Metashell](#the-environment-in-metashell)
    - [Extending the environment](#extending-the-environment)
    - [The environment stack](#the-environment-stack)
    - [What happens to files included to the environment?](#what-happens-to-files-included-to-the-environment)
- [The pragmas Metashell provides](#the-pragmas-metashell-provides)
- [The full list of built-in header files](#the-full-list-of-built-in-header-files)
- [License](#license)

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
    * Boost
* Install CMake
* In case some elements of the default system include path are missing when
  libclang is used, Metashell has to add them. You need to provide the system
  include path clang uses to Metashell:
    * `tools/clang_default_path > lib/extra_sysinclude.hpp`
* Metashell needs the `clang++` compiler to generate precompiled headers. There
  is a default search path Metashell uses at startup to find `clang++`. In case
  it can be found in another directory on your system, you should add that path
  to the default search path: add the path to
  `lib/default_clang_search_path.hpp`. If you don't know where `clang++` on your
  system is, you can use the `which clang++` command to find it.
* In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
        * Note: on Fedora you need to run `cmake .. -DCLANG_LIBRARYDIR=/usr/lib/llvm`
    * `make`
    * To install it on the host run `make install`
    * To package it for Debian or Ubuntu run `cpack -G DEB`
    * To package it for OpenSUSE or Fedora run `cpack -G RPM`
* To generate code-coverage statistics for unit tests run the following commands
  in the source directory instead the above ones:
    * `mkdir bin`
    * `cd bin`
    * `cmake .. -DENABLE_CODE_COVERAGE=true -DCMAKE_BUILD_TYPE=Debug`
    * `make`
    * `make test_coverage`
    * You can find the test coverage statistics in the `bin/coverage` directory.

## Getting started

This section presents how to use Metashell to do simple debugging.

### Trying Metashell online

You can try Metashell in your browser without installing anything. You can find
an online demo at
[http://abel.web.elte.hu/shell/](http://abel.web.elte.hu/shell/).

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
> template <int N> struct fib { static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; };
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
mpl_::int_<13>
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
std::integral_constant<int, 13>
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

## The environment in Metashell

Metashell maintains an *environment*, which is a piece of C++ code. When you
evaluate a metaprogram in the shell (eg. `fib<3>::value`), the metaprogram is
evaluated in that environment: it can use all the declarations and definitions
from the environment and the files included from the environment.

Metashell has an initial environment. To see what it contains, start a new shell
and run the following command:

```cpp
> #msh environment
```

`#msh` is a short version of `#pragma metashell`. Metashell accepts both, but
`#msh` is easier to use. The `metashell environment` pragma displays the content
of the environment. A new shell contains the initial environment.

### Extending the environment

When you define a metafunction, you add the definition of that metafunction to
the environment. For example by running the following command:

```cpp
> template <class T> struct add_const { typedef const T type; };
```

You add the definition of `add_const` to the environment. After running it, you
can use the `metashell environment` pragma to see that it has been added.

When you type in a command (eg. `template <class T> struct add_const ...` or
`add_const<T>`) Metashell has to decide if it is a command instructing the shell
to evaluate a metaprogram or it is an addition to the environment. Metashell
will try to figure it out, which category your command belongs to. If you'd like
to add something to the environment, but Metashell thinks that it is a
metaprogram to evaluate, there is a pragma you can use:

```cpp
> #msh environment add template <class T> struct add_const { typedef const T type; };
```

This will always add your command to the environment. Similarly, if you'd like
to run a metaprogram, but Metashell tries adding it to the environment, there is
a pragma which you can use:

```cpp
> #msh evaluate add_const<int>::type
```

This will always evaluate your command as a metaprogram. Note that if you need
to use any of these pragmas, you have found a bug in Metashell. Please file a
bug report about it.

### The environment stack

It might happen, that you'd like to experiment with some definitions and then
return to an earlier environment. For example you have a template class, you
add some extra specialisations, try running some metaprograms and you'd like to
remove the specialisations and get back to the original state.

Metashell has an *environment stack* which you can use. You can push the current
environment on this stack by running the following command:

```cpp
> #msh environment push
```

This command does not change the environment, but saves it in a stack. You can
add new definitions, include new header files and run metaprograms in that
updated environment. Once you're done and you'd like to get back to the original
environment, you can run the following command:

```cpp
> #msh environment pop
```

This will restore the environment you saved by the `environment push` pragma and
remove it from the environment stack. You can add multiple environments to the
stack and get back to them later. You can always check the current size of the
environment stack by running the following command:

```cpp
> #msh environment stack
```

This command displays how many environments have been stored on the stack.

### What happens to files included to the environment?

When you use `#include` to add the content of a file to the environment, the
`#include` line is added to the environment, not the content of the file. Every
time the environment is re-evaluated, the content of the included file is
re-read. When is the environment re-evaluated?

It depends on multiple things.

* When you add something new to the environment, the entire environment is
  re-evaluated. Any changes to the included header files are picked up.
* When you run a metaprogram *and* you have turned precompiled header usage off,
  the environment is re-evaluated. If you have enabled precompiled header usage,
  the environment is not re-evaluated.

Metashell uses precompiled headers to make the shell faster. When you use
metaprograms you might (eg. by using Boost.MPL) need to include large header
files taking a lot of time to process. If you re-evaluate them for every command
the shell responds slowly. The speed can be increased significantly by creating
a precompiled header of the environment. This precompiled header is rebuilt
every time there are changes to the environment.

Precompiled header usage is enabled by default (if Metashell can find the
`clang++` binary on your computer). You can turn it off by using the
`--no_precompiled_headers` command-line argument at startup or by running the
following command in the shell:

```cpp
#msh precompiled_headers off
```

This command disables precompiled header usage in the current shell.

## The pragmas Metashell provides

Metashell provides a number of pragmas controlling the shell. Note that these
pragmas are available only from the shell's command prompt. If you put them in a
header file and include that header, these pragmas will be ignored.
The pragmas accepted by Metashell begin with `#pragma metashell`. Since this is
a long prefix to type, Metashell accepts shorter versions as well. Here is the
complete list of accepted forms. All of them are equivalent:

* `#pragma metashell`
* `#pragma msh`
* `#metashell`
* `#msh`

Metashell supports the following pragmas:

<!-- pragma_info -->
* `#msh environment` <br /> <br /> Displays the entire content of the environment.

* `#msh environment add <code>` <br /> <br /> Appends code to the environment. Use this if Metashell thinks about the code that it is an evaluation.

* `#msh environment pop` <br /> <br /> Pops the last environment from the environment stack.

* `#msh environment push` <br /> <br /> Pushes the current environment to the environment stack.

* `#msh environment reset` <br /> <br /> Resets the environment to its initial state. It does not change the environment stack.

* `#msh environment stack` <br /> <br /> Displays the size of the environment stack.

* `#msh evaluate <code>` <br /> <br /> Evaluates code as a metaprogram. Use this if Metashell thinks about the code that it is an addition to the environment.

* `#msh help` <br /> <br /> Displays a help message.

* `#msh precompiled_headers [on|1|off|0]` <br /> <br /> Turns precompiled header usage on or off. When no arguments are used, it displays if precompiled header usage is turned on.

* `#msh quit` <br /> <br /> Terminates the shell.

* `#msh verbose [on|1|off|0]` <br /> <br /> Turns verbose mode on or off. When no arguments are used, it displays if verbose mode is turned on.


<!-- pragma_info -->

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

