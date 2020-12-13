# Getting started

This section presents how to use Metashell to do simple debugging.

## Trying Metashell online

You can try Metashell in your browser without installing anything. You can find
an online demo [here](../about/demo/index.html).

## Using Metashell on your C++ project

If you are using [CMake](https://cmake.org/), you can easily debug the code you
are working on using Metashell. Make sure, that you enable
[`CMAKE_EXPORT_COMPILE_COMMANDS`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html)
in order to generate `compile_commands.json` in the directory you run `cmake`
from. Once you have that file, Metashell can load the compilation flags
(eg. the ones setting the include path) from that file:

```
$ metashell <path to compile_commands.json>
```

Once Metashell has started, you will need to select the compilation unit,
whose compilation flags you would like to use. You can list them using the
following command:

```cpp
> #msh config
```

This command will list the directories containing source files. You can list
the content of a directory with the same command:

```cpp
> #msh config <name of directory>
```

Using this command repeatedly on subdirectories will get you to the source
files of your project. You can also use tab completion to help the navigation.
Once you know which one you'd like to debug, you can load it:

```cpp
> #msh config load <name of the config>
```

Metashell will detect the compiler you use for building your code and
use the same compiler in the backend.
[GCC](../reference/engines#gcc) and [Visual C++](../reference/engines#msvc)
support is currently very limited in Metashell, but you can switch to the
Templight shipped by Metashell by running

```cpp
> #msh engine switch internal
```

to be able to use the type shell or the template debugger or to Wave by running

```cpp
> #msh engine switch wave
```

to be able to use the macro debugger.

## Evaluating simple expressions

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

## How about Fibonacci?

You have seen how to do simple arithmetic using Boost.MPL's integral types but
you haven't been able to run the classical Fibonacci example of template
metaprogramming. Let's write it first:

```cpp
> template <int N> struct fib { static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; };
> template <> struct fib<0> { static constexpr int value = 1; };
> template <> struct fib<1> { static constexpr int value = 1; };
```

Note that Metashell supports multiline definitions as well. You can enter the
above in the shell the following way as well:

```cpp
> template <int N> \
...> struct fib \
...> { \
...>   static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; \
...> };
> template <> \
...> struct fib<0> \
...> { \
...>   static constexpr int value = 1; \
...> };
> template <> \
...> struct fib<1> \
...> { \
...>   static constexpr int value = 1; \
...> };
```

What is important is that the last character of a line _has to be_ `\`. That is
how Metashell knows that there is more to come.

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

## Using the Metadebugger

Metadebugger lets you inspect step by step how the compiler runs your
metaprograms.

> Note that depending on your platform or underlying compiler, the details of
> the template instantiations may differ. For example the order and amount of
> memoizations might be different than what you find in this tutorial.

Let's debug the fibonacci metaprogram seen earlier. Start the metadebugger by
entering:

```cpp
> #msh mdb boost::mpl::int_<fib<6>::value>
For help, type "help".
Metaprogram started
```

You'll see, that the prompt has changed to `(mdb)`. Now you can enter
metadebugger commands.

### Stepping

Metadebugger provides an interface similar to gdb. For example you can step
the metaprogram forward three steps:

```cpp
(mdb) step 3
fib<4> (TemplateInstantiation)
```

As you can see, metadebugger tells you that in this step `fib<4>` is getting
instantiated in a TemplateInstantiation event.

Stepping backwards is also trivial in a template metaprogram:

```cpp
(mdb) step -1
fib<5> (TemplateInstantiation)
```

### Backtrace

You can check the current backtrace:
```cpp
(mdb) bt
#0 fib<5> (TemplateInstantiation)
#1 fib<6> (TemplateInstantiation)
#2 int_<fib<6>::value>
```

This shows us that:

* We started the template metaprogram execution by evaluating
  `int_<fib<6>::value>`.
* The evaluation of this expression has (at some point) called `fib<6>`.
* The `fib` metafunction has (at some point) called `fib<5>`. This is where we
  are in the execution of the metaprogram.

### Forwardtrace

Metadebugger can also see into the future, and print the forwardtrace from any
step:

```cpp
(mdb) ft
fib<5> (TemplateInstantiation)
+ fib<4> (TemplateInstantiation)
| + fib<3> (TemplateInstantiation)
| | + fib<2> (TemplateInstantiation)
| | | + fib<1> (Memoization)
| | | ` fib<0> (Memoization)
| | ` fib<1> (Memoization)
| ` fib<2> (Memoization)
` fib<3> (Memoization)
```

This shows us what metafunctions the metaprogram *will* call after the current
location. As you can see the output shows the relations between the function
calls: which metafunction calls which other metafunctions. The events in the
output of forwardtrace happen in that order from the top down.

You probably noticed that there are two kinds of events metadebugger shows you:

* __TemplateInstantiation__ event happens when the compiler first encounters and
  instantiates a new template type. During a TemplateInstantiation event the
  compiler will instantiate every subtype it needs to get to the result.
* __Memoization__ event happens when a compiler encounters a type, which it had
  already instantiated before. It won't go through the instantiation process
  again, instead it uses technique called
  [memoization](http://en.wikipedia.org/wiki/Memoization) to speed up the
  compilation. This basically means that the compiler remembers every type it
  had instantiated, and reuses them when it encounters them again.

  Full template specializations (e.g. `fib<0>` and `fib<1>`) also appear as
  Memoization events.

For example, in the above forwardtrace output, you can see that `fib<5>`
creates `fib<4>` in a TemplateInstantiation event, which in turn instantiates
`fib<3>` also in a TemplateInstantiation event and so on. You can also see,
that when `fib<5>` gets to the point to instantiate `fib<3>` it has already
been instantiated by `fib<4>`, so only a Memoization event happens.

There are a few more type of template events that we haven't seen so far:

* __ExplicitTemplateArgumentSubstitution__: event happens when the compiler
  substitutes explicit template arguments provided for a function template.
* __DeducedTemplateArgumentSubstitution__: event happens when the compiler
  substitutes template arguments determined as part of template argument
  deduction for either a class template partial specialization or a function
  template.

Check out the section about
[How to debug expressions involving SFINAE?](how_to.md#debug-expressions-involving-sfinae)
for more information about these event types.

### Breakpoints and continue

You can also create breakpoints:

```cpp
(mdb) rbreak fib<3>
Breakpoint "fib<3>" will stop the execution on 3 locations
```

Now let's continue the execution until the first breakpoint:

```cpp
(mdb) continue
Breakpoint 1: regex("fib<3>") reached
fib<3> (TemplateInstantiation)
```

Commands can be abbreviated if unambiguous. For example you can use just `c`
instead of `continue`:

```cpp
(mdb) c
Breakpoint 1: regex("fib<3>") reached
fib<3> (Memoization)
```

You can repeat the last command by simply hitting enter again and again:

```cpp
(mdb)
Breakpoint 1: regex("fib<3>") reached
fib<3> (Memoization)
(mdb)
Metaprogram finished
mpl_::int_<13>
```

### Evaluating failing metaprograms

Sometimes you want to debug metaprograms which don't compile for some reason.
Metadebugger allows this and will behave similarly to how runtime debuggers
stop the execution when some runtime failure is detected (like a Segmentation
Fault). To demonstrate this, consider the following faulty version of the
fibonacci metaprogram:

```cpp
> template <int N> struct fib { static constexpr int value = fib<N - 1>::value + fib<N - 2>::value; };
> template <> struct fib<0> { static constexpr int value = 1; };
> template <> struct fib<1> {};
> template <int N> struct int_;
```

Let's try to evaluate `int_<fib<5>::value>`:

```cpp
> int_<fib<5>::value>
```

You'll see a bunch of error messages appear. Let's debug out metaprogram to
see what's going on:

```cpp
> #msh mdb int_<fib<5>::value>
For help, type "help".
Metaprogram started
```

Continue the execution until we hit the error:

```cpp
> continue
Metaprogram finished
```

You should see the same error messages as above, but this time we can actually
check out where the error happened:

```cpp
(mdb) bt
#0 fib<1> (Memoization)
#1 fib<2> (TemplateInstantiation)
#2 fib<3> (TemplateInstantiation)
#3 fib<4> (TemplateInstantiation)
#4 fib<5> (TemplateInstantiation)
#5 int_<fib<5>::value>
```

Since the top frame (`#0`) is the Memoization of `fib<1>` we can suspect that
there is something wrong with that. Indeed, there is no value member defined
in the `fib<1>` specialization.

### Reevaluation and precompiled headers

Metadebugger never uses precompiled headers. This is because instantiation
events which happened in the precompiled code, doesn't appear again, when a
metaprogram is compiled using precompiled headers. This is expected, since this
is one of the reasons PCH can speed up compilation.

This might seem like bad news, but it has a very useful side effect. If you
included files while you set up the compilation environment, you can actually
modify those files on the fly without restarting Metadebugger and using
`#msh environment reload`. The modified file will be included and reparsed
every time you evaluate a metaprogram.

This technique can be used outside Metadebugger, but you might have to
explicitly turn off precompiled headers. See this section for more information:
[What happens to files included to the environment?](#what-happens-to-files-included-to-the-environment)

To reevaulate the last metaprogram, you can simply enter `evaluate` (or `e` for
short) without giving any expression as an argument:

```cpp
(mdb) e
Metaprogram started
```

### Profiling

Metadebugger can be used to profile template metaprograms. To enable profiling
call `evaluate` or `#msh mdb` with the `-profile` flag:

```cpp
(mdb) evaluate -profile int_<fib<4>::value>
Metaprogram started
(mdb) ft
int_<fib<4>::value>
+ [0.41ms, 76.89%] fib<4> (TemplateInstantiation)
| + [0.14ms, 26.70%] fib<2> (TemplateInstantiation)
| | + [0.01ms, 2.44%] fib<1> (Memoization)
| | ` [0.01ms, 2.06%] fib<0> (Memoization)
| ` [0.06ms, 11.84%] fib<3> (TemplateInstantiation)
|   + [0.01ms, 1.12%] fib<1> (Memoization)
|   ` [0.01ms, 0.94%] fib<2> (Memoization)
+ [0.05ms, 8.84%] int_<5> (TemplateInstantiation)
` [0.01ms, 0.94%] fib<4> (Memoization)
```

Some extra information is printed every time a frame is printed by any command.
You can see the absolute time taken by an instantiation and all of it's sub
instantiations as well as an approximate ratio of the time taken by an
instantiation and the full template instantiation process as a percentage.
The sum of the percentages on the top level will never add up to 100% since the
compiler does other things which are not shown in the trace.

One more improtant difference in profile mode, is that the instantiations and
sub instantiations are not ordered by the order which they were actually
instantiated by the compiler, but the time they took the longest one being the
first. This means that you can usually find the bottleneck just by looking at
the first few steps of the metaprogram.

#### Profiling a translation unit

When trying to find out why a specific translation unit takes a long time to
compile, you may want to see the trace for the full file, not just a single
expression. You can do this with Metadebugger as well:

```cpp
> #include "file_which_takes_a_long_time_to_compile.cpp"
> #msh mdb -profile -
Metaprogram started
(mdb)
```

The `-` means, that the final trace you can traverse is not constrained to a
single expression, but to whole translation unit.  From this point, you can use
the usual commands to traverse and inspect the instantiation tree.

### Full mode

There are three modes which Metadebugger can operate in. The normal mode and
profiling mode which was shown in the previous chapters, and the full mode.
To demonstrate the difference let's evaluate a metaprogram in full mode and
print the forwardtrace:

```cpp
(mdb) evaluate -full int_<fib<4>::value>
Metaprogram started
(mdb) ft
int_<fib<4>::value>
+ fib<4>
| + fib<3>
| | + fib<2>
| | | + fib<1>
| | | ` fib<0>
| | ` fib<1>
| ` fib<2>
|   + fib<1>
|   ` fib<0>
` mpl_::int_<5>
```

Full mode doesn't try to follow what the real compiler does, but instead it
tries to simulate a dumb compiler, which doesn't use memoizations to speed up
compilation. For example, `fib<2>` and its full sub call tree is visible two
times. This mode can be useful, when the part of the trace you're interested in
is hidden under multiple layers of Memoizations in normal mode.

Please note, that traces in full mode can get extremely long.

### Using step over

A special qualifier, `over` can be passed to the `step` command. Using
`step over` will jump over sub instantiations. Let's take a look at an example:

```cpp
(mdb) evaluate int_<fib<5>::value>
Metaprogram started
(mdb) ft
int_<fib<5>::value>
+ fib<5> (TemplateInstantiation)
| + fib<4> (TemplateInstantiation)
| | + fib<3> (TemplateInstantiation)     A
| | | + fib<2> (TemplateInstantiation)
| | | | + fib<1> (Memoization)
| | | | ` fib<0> (Memoization)
| | | ` fib<1> (Memoization)             B
| | ` fib<2> (Memoization)               C
| ` fib<3> (Memoization)
+ fib<5> (Memoization)
` int_<5> (TemplateInstantiation)
```

Three instantiations are marked, so it is easier to talk about them. First,
let's simply step forward to `A`:

```cpp
(mdb) step 3
fib<3> (TemplateInstantiation)       A
```

Here, `step over` will jump over the types instantiated by `A` to `C`:

```cpp
(mdb) step over
fib<2> (Memoization)       C
```

Of course, `step over -1` will do the reverse and bring us back to `A`:

```cpp
(mdb) step over -1
fib<3> (TemplateInstantiation)       A
```

Be careful though, `step over -1` is not always the inverse of `step over`.
Let's step forward to `B` to demonstrate this:


```cpp
(mdb) step 4
fib<1> (Memoization)       B
```

Since there are no more sub instantiations to step over, from here `step over`
has no other choice but to behave like a normal `step` command and jump out to
`C`.

```cpp
(mdb) step over
fib<2> (Memoization)       C
```

But from `C`, as we've seen earlier, `step over -1` will bring us back to `A`
and not `B`.

```cpp
(mdb) step over -1
fib<3> (TemplateInstantiation)       A
```

### Quit Metadebugger

To exit from metadebugger use Ctrl+D or the `quit` command.

```cpp
(mdb) quit
>
```

## Data structures of Boost.MPL

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
boost_::mpl::vector<void, int, double, char>
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

## Writing custom formatters

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

### Using specialisation

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

### Using tag dispatching

Another way of overriding the formatter is using tag dispatching. The default
implementation of `metashell::format` evaluates
`metashell::format_impl<T::tag>::apply<T>::type` where `T` is `format`'s
argument. When `T::tag` is not defined, `void` is used instead.

Specialising `metashell::format_impl` for a new tag adds support for formatting
values using that tag. These specialisations can be part of the implementation
of a data-type to avoid Metashell users having to include extra headers for
formatting.

### Embedding custom formatters

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
  struct format<cons<typename format<Head>::type, Tail>>
  {
    // ...
  };
}
#endif
```

This example defines the formatter only when the header file is used inside
Metashell. This is useful for debugging and has no impact on the regular users
of the header.

Note that it is a formatter for a _container_, which contains elements. Those
elements might also need formatting, therefore it formats the elements as well
by calling `format` recursively on each element (see `format<Head>::type`).

The version of the shell is also available for the headers. Metashell defines
the `__METASHELL_MAJOR`, `__METASHELL_MINOR` and `__METASHELL_PATCH` macros.

## Testing macros

Metashell can be used as an interactive shell for testing macros. The easiest
way to get started with it is to run the following command in Metashell:

```cpp
> #msh preprocessor mode
```

This updates the shell's settings to turn it into an ideal environment for
testing macros. You can start Metashell in this mode by using the
`--preprocessor` command line argument.

### Evaluating simple expressions

Let's look at how to evaluate the expression `6 + 7`. If you have access to
`python`, `ghci` or `erl`, start it and type `6 + 7` (`6 + 7.` in Erlang) and
press enter. You will see `13`, the result of this expression immediately.

Let's do the same in preprocessor metaprogramming. Start Metashell and run
`#msh preprocessor mode` to turn it into a preprocessor shell. Addition is
implemented by the `BOOST_PP_ADD` macro - get it by running the following
command:

```cpp
> #include <boost/preprocessor/arithmetic/add.hpp>
```

You will see a number of empty lines and lines starting with `#` printed on your
screen. The reason behind it is that Metashell displays the result of
preprocessing the included file. The preprocessor directives are processed (and
produce no output) but all the empty lines in the included file are displayed.
The lines starting with `#` provide source file name and line information to any
C++ compiler processing the precompiled code, so the compiler can provide useful
source locations in the error messages.

Once you have set your environment up, you can evaluate the expression `6 + 7`
by running the following:

```cpp
> BOOST_PP_ADD(6, 7)
```

You get `13`, which is the result of this addition. You can try adding other
numbers as well.

Note that you can add arbitrarily large numbers using `BOOST_PP_ADD`. The
arguments (and the result) have to be in the range `0 .. BOOST_PP_LIMIT_MAG`
where `BOOST_PP_LIMIT_MAG` is a macro. Let's find out its value. It is defined
in the following header:

```cpp
> #include <boost/preprocessor/config/limits.hpp>
```

The above command makes the `BOOST_PP_LIMIT_MAG` macro available, so it is easy
to check its value:

```cpp
> BOOST_PP_LIMIT_MAG
256
```

### Checking which macros are available

Preshell can be used to get the list of available macros. The following command
lists the names of the macros:

```cpp
> #msh macro names
```

The result of it is the list of macros defined at point where the pragma was
used. The following command displays the definition of the macros as well:

```cpp
> #msh macros
```

The output of this command can be long, since it displays all macros and their
actual definition.

## Using the preprocessor debugger

The preprocessor debugger lets you inspect step by step how the preprocessor
evaluates your macros.

> Note that this feature is supported by the
> [Wave engine](../reference/engines/index.html#available-engines) only. You
> need to start Metashell with the
> [Wave engine](../reference/engines/index.html#available-engines) to be able to
> follow the steps of this part of the tutorial. If you are using the
> [online demo](../about/demo/index.html), you can switch to the the
> [Wave engine](../reference/engines/index.html#available-engines) by running
> the `#msh config load wave` command in the shell. Make sure that you are in
> preprocessor mode by running `#msh preprocessor mode`.

Let's define an example macro and debug it. First, let's define a macro for
turning something into a `const` pointer:

```cpp
> #define CONST_PTR(x) const x*
```

It can be used the following way:

```cpp
> CONST_PTR(int)
const int*
> CONST_PTR(double)
const double*
```

Now let's define a macro, which repeats a set of tokens multiple times:

```cpp
> #define REPEAT(n, m) REPEAT ## n(m)
> #define REPEAT0(m)
> #define REPEAT1(m) m
> #define REPEAT2(m) m, REPEAT1(m)
> #define REPEAT3(m) m, REPEAT2(m)
> #define REPEAT4(m) m, REPEAT3(m)
> #define REPEAT5(m) m, REPEAT4(m)
```

It can be used the following way:

```cpp
> REPEAT(3, int)
int, int, int
```

The two macros can be combined:

```cpp
> REPEAT(3, CONST_PTR(int))
const int*, const int*, const int*
```

Let's debug the above macro expansion. Start the preprocessor debugger by
entering:

```cpp
> #msh pdb REPEAT(3, CONST_PTR(int))
For help, type "help".
Metaprogram started
(pdb)
```

You'll see, that the prompt has changed to `(pdb)`. Now you can enter
preprocessor debugger commands.

### Stepping

The preprocessor debugger provides an interface similar to
[gdb](https://www.gnu.org/software/gdb/). For example you can step the macro
evaluation forward three steps:

```cpp
(pdb) step 3
const int* (Rescanning)
```

As you can see, the preprocessor debugger tells you that in this step
`const int*` is getting expanded in a Rescanning event.

Stepping backwards is also trivial in a macro evaluation:

```cpp
(pdb) step -1
CONST_PTR(int) (MacroExpansion)
```

### Backtrace

You can check the current backtrace:
```cpp
(pdb) bt
#0 CONST_PTR(int) (MacroExpansion)
#1 REPEAT(3, CONST_PTR(int)) (MacroExpansion)
#2 REPEAT(3, CONST_PTR(int))
```

This shows us that:

* We started the macro evaluation by evaluating
  `REPEAT(3, CONST_PTR(int))`.
* The evaluation of this expression has (at some point) called
  `REPEAT(3, CONST_PTR(int))`.
* The `REPEAT` macro has (at some point) called `CONST_PTR`. This is where we
  are in the evaluation of the macro call.

### Forwardtrace

The preprocessor debugger can also see into the future, and print the
forwardtrace from any step:

```cpp
(pdb) ft
CONST_PTR(int) (MacroExpansion)
` const int* (Rescanning)
  ` const int* (ExpandedCode)
```

This shows us what macros the metaprogram *will* call after the current
location. As you can see the output shows the relations between the
preprocessing steps: which steps trigger which other steps. The events in the
output of forwardtrace happen in that order from the top down.

You probably noticed that there are three kinds of events metadebugger shows
you:

* __MacroExpansion__ event happens when the preprocessor starts the expansion of
  a macro call.
* __Rescanning__ event happens when the preprocessor finished substituting the
  body of a macro and is about to expand the recursive macro calls.
* __ExpandedCode__ event happens when the preprocessor is done with rescanning a
  macro expansion. The tokens produced are the result of a macro expansion.

For example, in the above forwardtrace output, you can see that the preprocessor
substitues the `CONST_PTR(int)` macro call with `const int*`. This triggers a
Rescanning event, which produces the same tokens (`const int*`) as there are no
macro calls in it. This triggers an ExpandedCode event with with the same
tokens.

There are a few more type of events that we haven't seen so far:

* __MacroDefinition__ happens, when a macro is defined (using `#define`).
* __MacroDeletion__ happens, when a macro is deleted (using `#undef`).
* __GeneratedToken__ happens, when an output token is generated (eg. as the
  result of a macro call or because of outputting tokens directly from the
  input).
* __SkippedToken__ happens, when a token is skipped (eg. because of being in the
  skipped branch of an `#if`).
* __QuoteInclude__ happens, when the preprocessor starts the evaluation of a
  `#include "..."`.
* __SysInclude__ happens, when the preprocessor starts the evaluation of a
  `#include <...>`.
* __PreprocessingCondition__ happens, when the preprocessor starts the
  evaluation of a condition (eg. `#if`, `#ifdef`).
* __PreprocessingConditionResult__ happens, when the preprocessor determines the
  result of a condition (`true` or `false`).
* __PreprocessingElse__ happens, when the preprocessor reaches a `#else`.
* __PreprocessingEndif__ happens, when the preprocessor finishes the evaluation
  of a conditional by reaching `#endif`.
* __ErrorDirective__ happens, when the preprocessor evaluates a `#error`.
* __LineDirective__ happens, when the preprocessor evaluates a `#line`

### Breakpoints and continue

You can also create breakpoints:

```cpp
(pdb) rbreak const int
Breakpoint "const int" will stop the execution on 13 locations
```

Now let's continue the execution until the first breakpoint:

```cpp
(pdb) continue
Breakpoint 1: regex("const int") reached
const int* (Rescanning)
```

Commands can be abbreviated if unambiguous. For example you can use just `c`
instead of `continue`:

```cpp
(mdb) c
Breakpoint 1: regex("const int") reached
const int* (ExpandedCode)
```

You can repeat the last command by simply hitting enter again and again:

```cpp
(pdb)
Breakpoint 1: regex("const int") reached
REPEAT3( const int*) (Rescanning)
(pdb)
Breakpoint 1: regex("const int") reached
REPEAT3( const int*) (MacroExpansion)
(pdb)
Breakpoint 1: regex("const int") reached
 const int*, REPEAT2( const int*) (Rescanning)
(pdb)
Breakpoint 1: regex("const int") reached
REPEAT2( const int*) (MacroExpansion)
(pdb)
Breakpoint 1: regex("const int") reached
 const int*, REPEAT1( const int*) (Rescanning)
(pdb)
Breakpoint 1: regex("const int") reached
REPEAT1( const int*) (MacroExpansion)
(pdb)
Breakpoint 1: regex("const int") reached
 const int* (Rescanning)
(pdb)
Breakpoint 1: regex("const int") reached
const int* (ExpandedCode)
(pdb)
Breakpoint 1: regex("const int") reached
const int*, const int* (ExpandedCode)
(pdb)
Breakpoint 1: regex("const int") reached
const int*, const int*, const int* (ExpandedCode)
(pdb)
Breakpoint 1: regex("const int") reached
const int*, const int*, const int* (ExpandedCode)
(pdb)
Metaprogram finished
const int*, const int*, const int*
```

### Using step over

A special qualifier, `over` can be passed to the `step` command. Using
`step over` will jump over sub events. Let's take a look at an example:

```cpp
(pdb) evaluate REPEAT(3, CONST_PTR(int))
Metaprogram started
(pdb) ft
REPEAT(3, CONST_PTR(int))
+ REPEAT(3, CONST_PTR(int)) (MacroExpansion)
| + CONST_PTR(int) (MacroExpansion)
| | ` const int* (Rescanning)
| |   ` const int* (ExpandedCode)
| ` REPEAT3( const int*) (Rescanning)
|   + REPEAT3( const int*) (MacroExpansion)
|   | `  const int*, REPEAT2( const int*) (Rescanning)
|   |   + REPEAT2( const int*) (MacroExpansion)
|   |   | `  const int*, REPEAT1( const int*) (Rescanning)
|   |   |   + REPEAT1( const int*) (MacroExpansion)            A
|   |   |   | `  const int* (Rescanning)
|   |   |   |   ` const int* (ExpandedCode)                     B
|   |   |   ` const int*, const int* (ExpandedCode)            C
|   |   ` const int*, const int*, const int* (ExpandedCode)
|   ` const int*, const int*, const int* (ExpandedCode)
+ const (GeneratedToken)
+   (GeneratedToken)
+ int (GeneratedToken)
+ * (GeneratedToken)
+ , (GeneratedToken)
+   (GeneratedToken)
+ const (GeneratedToken)
+   (GeneratedToken)
+ int (GeneratedToken)
+ * (GeneratedToken)
+ , (GeneratedToken)
+   (GeneratedToken)
+ const (GeneratedToken)
+   (GeneratedToken)
+ int (GeneratedToken)
+ * (GeneratedToken)
` \n (GeneratedToken)
```

Three events are marked, so it is easier to talk about them. First, let's simply
step forward to `A`:

```cpp
(mdb) step 10
REPEAT1( const int*) (MacroExpansion)                    A
```

Here, `step over` will jump over the details of the macro expansion triggered by
`A` to the expanded tokens, `C`:

```cpp
(mdb) step over
const int*, const int* (ExpandedCode)                    C
```

Of course, `step over -1` will do the reverse and bring us back to `A`:

```cpp
(mdb) step over -1
REPEAT1( const int*) (MacroExpansion)                    A
```

Be careful though, `step over -1` is not always the inverse of `step over`.
Let's step forward to `B` to demonstrate this:


```cpp
(mdb) step 2
const int* (ExpandedCode)                                B
```

Since there are no more sub events to step over, from here `step over` has no
other choice but to behave like a normal `step` command and jump out to `C`.

```cpp
(mdb) step over
const int*, const int* (ExpandedCode)                    C
```

But from `C`, as we've seen earlier, `step over -1` will bring us back to `A`
and not `B`.

```cpp
(mdb) step over -1
REPEAT1( const int*) (MacroExpansion)                    A
```

### Quit the preprocessor debugger

To exit from the preprocessor debugger use Ctrl+D or the `quit` command.

```cpp
(pdb) quit
>
```

### Evaluating failing macros

Sometimes you want to debug macros which don't compile for some reason. The
preprocessor debugger allows this and will behave similarly to how runtime
debuggers stop the execution when some runtime failure is detected (like a
Segmentation Fault). To demonstrate this, consider the following faulty version
of the `REPEAT` macro:

```cpp
> #define REPEAT(n, m) REPEAT ## n(n, m)
> #define REPEAT0(m)
> #define REPEAT1(m) m
> #define REPEAT2(m) m, REPEAT1(m)
> #define REPEAT3(m) m, REPEAT2(m)
> #define REPEAT4(m) m, REPEAT3(m)
> #define REPEAT5(m) m, REPEAT4(m)
```

> Note that in case you've been following this tutorial from the beginning,
> you already have defined a (correct) `REPEAT` macro. In this case you can
> get the broken version by running `#undef REPEAT` and
> `#define REPEAT(n, m) REPEAT ## n(n, m)`, as the helper macros remain
> unchanged.

Let's try to evaluate `REPEAT(3, int)`:

```cpp
> REPEAT(3, int)
<stdin>:54:1: warning: too many macro arguments: REPEAT3
```

Let's debug our macro to see what's going on:

```cpp
> #msh pdb REPEAT(3, int)
For help, type "help".
Metaprogram started
```

Continue the execution until we hit the error:

```cpp
> continue
Metaprogram finished
<stdin>:5:1: warning: too many macro arguments: REPEAT3
```

You should see the same error messages as above, but this time we can actually
check out where the error happened:

```cpp
(pdb) bt
#0 REPEAT3(3,  int) (Rescanning)
#1 REPEAT(3, int) (MacroExpansion)
#2 REPEAT(3, int)
```

As the backtrace shows, `REPEAT(3, int)` tried calling the `REPEAT3` macro with
two arguments, while this macro takes only one.

