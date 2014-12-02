# Metashell

[![Build Status](https://secure.travis-ci.org/sabel83/metashell.png?branch=master "Build Status")](http://travis-ci.org/sabel83/metashell)

![](https://raw.github.com/sabel83/metashell/master/metashell.png "Metashell")

[![Try online](https://raw.github.com/sabel83/metashell/master/try_online.png "Try online")](http://abel.web.elte.hu/shell/metashell)
[![Download](https://raw.github.com/sabel83/metashell/master/download.png "Download")](#version-200)

The goal of this project is to provide an interactive template metaprogramming
shell.

## Table of contents

- [Motivation](#motivation)
- [Supported platforms](#supported-platforms)
- [Binary installers](#binary-installers)
    - [Version 2.0.0](#version-200)
        - [Linux](#linux)
        - [Windows](#windows)
        - [OS X](#os-x)
    - [Version 1.0.0](#version-100)
        - [Linux](#linux-1)
- [Building Metashell](#building-metashell)
    - [Building Metashell on Linux, FreeBSD and OpenBSD](#building-metashell-on-linux-freebsd-and-openbsd)
        - [Using the helper scripts](#using-the-helper-scripts)
        - [Building manually](#building-manually)
        - [Clang and Templight](#clang-and-templight)
    - [Building Metashell on Windows](#building-metashell-on-windows)
        - [Standard header files on Windows](#standard-header-files-on-windows)
        - [WinEditLine](#wineditline)
    - [Building Metashell on OS X](#building-metashell-on-os-x)
        - [Using the helper scripts](#using-the-helper-scripts-1)
        - [Building manually](#building-manually-1)
- [Getting started](#getting-started)
    - [Trying Metashell online](#trying-metashell-online)
    - [Evaluating simple expressions](#evaluating-simple-expressions)
    - [How about Fibonacci?](#how-about-fibonacci)
    - [Using the Metadebugger](#using-the-metadebugger)
        - [Stepping](#stepping)
        - [Backtrace](#backtrace)
        - [Forwardtrace](#forwardtrace)
        - [Breakpoints and continue](#breakpoints-and-continue)
        - [Reevaluation and precompiled headers](#reevaluation-and-precompiled-headers)
        - [Full mode](#full-mode)
        - [Quit Metadebugger](#quit-metadebugger)
    - [Data structures of Boost.MPL](#data-structures-of-boostmpl)
    - [Writing custom formatters](#writing-custom-formatters)
        - [Using specialisation](#using-specialisation)
        - [Using tag dispatching](#using-tag-dispatching)
        - [Embedding custom formatters](#embedding-custom-formatters)
- [The environment in Metashell](#the-environment-in-metashell)
    - [Extending the environment](#extending-the-environment)
    - [The environment stack](#the-environment-stack)
    - [What happens to files included to the environment?](#what-happens-to-files-included-to-the-environment)
- [How to...](#how-to)
    - [see what happens during template argument deduction?](#see-what-happens-during-template-argument-deduction)
    - [see what a type alias resolves to?](#see-what-a-type-alias-resolves-to)
- [Changelog](#changelog)
    - [Version 2.0.0](#version-200-1)
    - [Version 1.0.0](#version-100-1)
- [Reference](#reference)
    - [The pragmas Metashell provides](#the-pragmas-metashell-provides)
    - [Metadebugger command reference](#metadebugger-command-reference)
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

## Supported platforms

The following platforms are supported:

* [Ubuntu](http://www.ubuntu.com/)
* [Debian](http://www.debian.org/)
* [Fedora](http://fedoraproject.org/)
* [OpenSuSE](http://www.opensuse.org/)
* [FreeBSD](http://www.freebsd.org/)
* [OpenBSD](http://www.openbsd.org/)
* [Windows](http://windows.microsoft.com)
* [Mac OS X](https://www.apple.com/osx/) Tested on Mavericks and Yosemite.
  Thanks to Gabor Marton and Pavel Morozkin.

## Binary installers

You can get binary installers for a number of platforms. If you can't or don't
want to install them, you can build Metashell yourself.

### Version 2.0.0

#### Linux

* Ubuntu
    * Ubuntu 14.10:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_ubuntu14.10_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_ubuntu14.10_i686.deb)
    * Ubuntu 14.04:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_ubuntu14.04_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_ubuntu14.04_i686.deb)
* Debian
    * Debian 7.7:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_debian7.7_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_debian7.7_i686.deb)
    * Debian 7.6:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_debian7.6_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_debian7.6_i686.deb)
* Fedora
    * Fedora 20:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_fedora20_x86_64.rpm) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_fedora20_i686.rpm)
* OpenSuSE
    * OpenSuSE 13.2:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_opensuse13.2_x86_64.rpm) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_opensuse13.2_i686.rpm)
    * OpenSuSE 13.1:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_opensuse13.1_x86_64.rpm) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_opensuse13.1_i686.rpm)

#### Windows

* [Windows installer](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_AMD64.exe)
  tested on Windows 7 and Windows 8.1

#### OS X

* [Yosemite](https://github.com/sabel83/metashell/releases/download/v2.0.0/metashell_2.0.0_osx10.10_x86_64.dmg)
  Note that you need to install XCode for Metashell to work properly.

### Version 1.0.0

#### Linux

* Ubuntu
    * Ubuntu 13.10:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_ubuntu13.10_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_ubuntu13.10_i686.deb)
    * Ubuntu 13.04:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_ubuntu13.04_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_ubuntu13.04_i686.deb)
    * Ubuntu 12.04:
      [64 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_ubuntu12.04_x86_64.deb) |
      [32 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_ubuntu12.04_i686.deb)
* OpenSuSE
    * OpenSuSE 13.3: [64 bit](https://github.com/sabel83/metashell/releases/download/v1.0.0/metashell_1.0.0_opensuse12.3_x86_64.rpm)

## Building Metashell

### Building Metashell on Linux, FreeBSD and OpenBSD

#### Using the helper scripts

* Download the source code from [github](http://github.com/sabel83/metashell).
* Run `install_build_dependencies.sh` to install the dependent tools and
  libaries
* Run `build.sh` to build Clang with Templight and Metashell. To take advantage
  of multicore systems, you can set the `BUILD_THREADS` environment variable to
  the maximum number of concurrent builds. (eg. `BUILD_THREADS=4 ./build.sh`)

#### Building manually

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent libraries:
    * Readline (or Libedit)
    * Termcap
* Install the dependent tools:
    * CMake
* Build Clang with [Templight](http://plc.inf.elte.hu/templight/)
    * `cd templight`
    * `mkdir build`
    * `cd build`
    * `cmake ../llvm -DLIBCLANG_BUILD_STATIC=ON`
    * `make clang`
    * `make libclang`
    * `make libclang_static`
* Now compile Metashell. In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
        * Note: to use
          [libedit](http://thrysoee.dk/editline/) instead
          of [Readline](http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)
          add the `-DUSE_EDITLINE=true` argument to the above command line.
        * Note: if you don't want to link staticly against libclang, you need to
          add `-DCLANG_STATIC=false` to the above command line.
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
* To compile the source for profiling using
  [gprof](https://www.cs.utah.edu/dept/old/texinfo/as/gprof.html) (g++ only):
    * `mkdir bin`
    * `cd bin`
    * `cmake .. -DENABLE_PROFILING=true`
    * `make`
    * The binaries will now generate profiling data to gmon.out.
      Please see the gprof documentation for futher information.

#### Clang and Templight

The source code of Metashell contains the source code of LLVM/Clang patched with
[Templight](http://plc.inf.elte.hu/templight/). The Templight patch can be found
in the `templight/patch` directory. The naming convention is
`templight_patch_r<SVN release it patches>.diff`. To update the Clang version
used:

* Place a newer patch in the `patch` directory
* Run the `tools/get_templight.sh` script

### Building Metashell on Windows

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install [CMake](http://cmake.org/)
* First you need to build Clang with
  [Templight](http://plc.inf.elte.hu/templight/).
    * Start a command line on your Windows host and go into the source directory
    * Run `cd templight`
    * Run `md build`
    * Run `cd build`
    * Run `cmake ..\llvm`
    * Start Visual Studio 2013 and open the generated solution
      (`templight\build\LLVM.sln`)
    * In the _BUILD_ menu choose _Configuration Manager..._. As
      _Active solution configuration:_ choose _Release_ and click on _Close_.
      This is needed to do a release instead of a debug build of Clang.
    * In _Solution Explorer_ right-click on _Clang libraries_ and choose
      _Build_.
    * In _Solution Explorer_ right-click on _Clang executables_ and choose
      _Build_.
    * You can optionally try building the rest of the projects, but Metashell
      does not need them.
* Now you can build Metashell
    * Start a command line on your Windows host and go into the source directory
    * Run `md bin`
    * Run `cd bin`
    * Run `cmake .. -G "Visual Studio 12 2013"`.
    * CMake will generate a solution file for Metashell in the `bin` directory.
      Open it with Visual Studio 2013 and build it. You can do a debug or a
      release build (or both) against the release build of the Templight-enabled
      Clang.
    * After a successful build you can find the Metashell executables in
      `bin/app/Release/metashell.exe` or `bin/app/Debug/metashell.exe` depending
      on the build type.
* To generate an installer for Metashell:
    * Install [NSIS](http://nsis.sourceforge.net).
    * Build Metashell following the above instructions (make sure you build the
      Release config).
    * Run `cpack` in the directory you run `cmake` as well.

#### Standard header files on Windows

The `windows_headers` directory contains the [MinGW](http://mingw.org/) header
files. These files are made available for Metashell.

The `tools/get_windows_headers.sh' script can (re)download these MinGW standard
header files. You need to run this script on Linux.

#### WinEditLine

Metashell uses [WinEditLine](http://mingweditline.sourceforge.net/) on Windows.
To simplify the Windows build, the source code of WindEditLine is redistributed
as part of the Metashell source code and built together with Metashell.
The content of the `src` directory of the WinEditLine source code can be found
in the `wineditline` directory. The content of the original `CMakeLists.txt` has
been changed to build the code together with Metashell. If you want to upgrade
WinEditLine to a newer version, you need to update these files.

### Building Metashell on OS X

#### Using the helper scripts

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent tools:
    * CMake
    * XCode
    * The appropriate version of "Auxilary tools for Xcode" which contains the
      PackageMaker compiler.
* Run `build.sh` to build Clang with Templight and Metashell. To take advantage
  of multicore systems, you can set the `BUILD_THREADS` environment variable to
  the maximum number of concurrent builds. (eg. `BUILD_THREADS=4 ./build.sh`)

#### Building manually

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent tools:
    * CMake
    * XCode
* Build Clang with [Templight](http://plc.inf.elte.hu/templight/)
    * `cd templight`
    * `mkdir build`
    * `cd build`
    * `cmake ../llvm -DLIBCLANG_BUILD_STATIC=ON`
    * `make clang`
    * `make libclang`
    * `make libclang_static`
* Now compile Metashell. In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
        * Note: if you don't want to link staticly against libclang, you need to
          add `-DCLANG_STATIC=false` to the above command line.
    * `make`
    * To install it on the host run `make install` as root.
* To generate an installer package (.dmg):
    * Install the appropriate version of "Auxilary tools for Xcode" which
      contains the PackageMaker compiler. On Mavericks this will be the 2012
      late july version.
    * `cpack`

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

### Using the Metadebugger

Metadebugger lets you inspect step by step how the compiler runs your
metaprograms.

Let's debug the fibonacci metaprogram seen earlier. Start the metadebugger by
entering:

```cpp
> #msh mdb int_<fib<6>::value>
For help, type "help".
Metaprogram started
```

You'll see, that the prompt has changed to `(mdb)`. Now you can enter
metadebugger commands.

#### Stepping

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

#### Backtrace

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

#### Forwardtrace

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
  compiler will instantiate every subtype it needs to get to the result
* __Memoization__ event happens when a compiler encounters a type, which it had
  already instantiated before. It won't go through the instantiation process
  again, instead it uses technique called
  [memoization](http://en.wikipedia.org/wiki/Memoization) to speed up the
  compilation. This basically means that the compiler remembers every type it
  had instantiated, and reuses them when it encounters them again.

  Full template specializations (e.g. `fib<0>` and `fib<1>`) only appear in
  Memoization events.

For example, in the above forwardtrace output, you can see that `fib<5>`
creates `fib<4>` in a TemplateInstantiation event, which in turn instantiates
`fib<3>` also in a TemplateInstantiation event and so on. You can also see,
that when `fib<5>` gets to the point to instantiate `fib<3>` it has already
been instantiated by `fib<4>`, so only a Memoization event happens.

#### Breakpoints and continue

You can also create breakpoints:

```cpp
(mdb) rbreak fib<3>
Breakpoint "fib<3>" will stop the execution on 2 locations
```

Now let's continue the execution until the first breakpoint:

```cpp
(mdb) continue
Breakpoint "fib<3>" reached
fib<3> (TemplateInstantiation)
```

Commands can be abbreviated if unambigouos. For example you can use just `c`
instead of `continue`:

```cpp
(mdb) c
Breakpoint "fib<3>" reached
fib<3> (Memoization)
```

You can repeat the last command by simply hitting enter again:

```cpp
(mdb)
Metaprogram finished
mpl_::int_<13>
```

#### Reevaluation and precompiled headers

Metadebugger never uses precompiled headers. This is because instantiation
events which happened in the precompiled code, doesn't appear again, when a
metaprogram is compiled using precompiled headers. This is expected, since this
is one of the reasons PCH can speed up compilation.

This might seem like bad news, but it has a very useful side effect. If you
included files while you set up the compilation environment, you can actually
modify those files on the fly without restarting Metashell. The modified file
will be included every time you evaluate a metaprogram.

This technique can be used outside Metadebugger, but you might have to
explicitly turn off precompiled headers when starting Metashell with the
`--no_precompiled_headers` command line option.

To reevaulate the last metaprogram, you can simply enter `evaluate` (or `e` for
short) without giving any expression as an argument:

```cpp
(mdb) e
Metaprogram started
```

#### Full mode

There are two modes which Metadebugger can operate in. The normal mode, which
was shown in the previous chapters, and the full mode. To demonstrate the
difference let's evaluate a metaprogram in full mode and print the forwardtrace:

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

Full mode doesn't try to follow what the real complier does, but instead it
tries to simulate a dumb compiler, which doesn't use memoizations to speed up
compilation. For example, `fib<2>` and its full sub call tree is visible two
times. This mode can be useful, when the part of the trace you're intrested in
is hidden under multiple layers of Memoizations in normal mode.

Please note, that traces in full mode can get extremely long.

#### Quit Metadebugger

To exit from metadebugger use Ctrl+D or the `quit` command.

```cpp
(mdb) quit
>
```

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
* You can ask Metashell to re-evaluate the environment by running the
  `#msh environment reload` command.

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

## How to...

### see what happens during template argument deduction?

If you call a function template without explicitly specifying all template
arguments the compiler will deduce the missing template arguments from the
function arguments.

Suppose you have the following function in your environment:

```cpp
> template<class T> void foo(const T& t) { /* ... */ }
```

You want to find out what T will be, when you call `foo(13)`. To find out,
start mdb like this:

```cpp
> #msh mdb decltype(foo(13))
Metaprogram started
```

Now, if you inspect the output of the ft command, you will be able to find the
instantiation of the foo function:

```cpp
(mdb) ft
decltype(foo(13))
+ foo<int> (TemplateInstantiation)
` void (NonTemplateType)
```

Of course inspecting the forwardtrace output is not always feasible for
nontrivial metaprograms. Here is another technique you can use, to see what
T becomes when you call foo with a template type like `std::vector`
(don't forget to `#include <vector>` in your environment):

```cpp
(mdb) eval decltype(foo(std::vector<int>{}))
Metaprogram started
(mdb) rbreak foo
Breakpoint "foo" will stop the execution on 1 location
(mdb) continue
Breakpoint "foo" reached
foo<std::vector<int, std::allocator<int> > > (TemplateInstantiation)
```

### see what a type alias resolves to?

In some cases you need to know what a type alias resolves to. As an example,
assume that you need to understand (and fix or improve) a piece of code that
defines a template class `a` and the following template function:

```cpp
template <class T>
void fun()
{
  typename a<T>::handle h;
}
```

When the type `typename a<T>::handle` is a type alias, to tell what the type of
`h` is you need to resolve that type alias. This might not be a simple task as
there can be specialisations for the template class `a`, it can inherit the
`handle` type alias from another class (or instance of a template class), etc.

Metashell can be used to display the type of `h`:

```cpp
> #include "fun.cpp" // the file the `fun` template function is defined in
> a<double>::handle
// the type handle resolves to gets displayed here...
```

You can include `cpp` files into Metashell as well, not just headers and then
ask Metashell to resolve the type aliases you are interested in. To get the
correct results, you need to make sure that you use the same include path and
macro definitions (`-D` arguments) when you launch Metashell that you use to
build your `fun.cpp` file.

## Changelog

### Version 2.0.0

* New features
    * mdb, the template metaprogram debugger. (Andras Kucsma's contribution)
    * Support for precompiled header usage. It is based on Marshall Clow's
      advice and is a major speed improvement.
    * Pragmas controlling the shell
    * More flexible environment management
        * Environment stack (with `push` and `pop` operations) for trying
          temporary definitions out
        * Environment reset to avoid restarting the shell
        * Saving the environment in a file for code developed in the shell
    * Multi-line input in the shell
    * New command-line arguments:
        * `-D` for defining macros
        * `-w` for suppressing warnings
        * `--no_precompiled_headers` for disabling precompiled header support
        * `--clang` for specifying which clang binary to use
        * `-ftemplate-depth` for specifying the maximum template depth
        * `--enable_saving` for enabling saving the environment in a file

* Fixes
    * The shell accepts the following on the command prompt:
        * macro definitions
        * empty command
        * type alias definitions when `typedef` is not the first token
        * comment
        * constexpr function definition
    * Don't store empty lines in history
    * Store repeated commands only once in history

### Version 1.0.0

* First release

## Reference

### The pragmas Metashell provides

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
* __`#msh environment`__ <br />
Displays the entire content of the environment.

* __`#msh environment add <code>`__ <br />
Appends code to the environment. Use this if Metashell thinks about the code that it is an evaluation.

* __`#msh environment pop`__ <br />
Pops the last environment from the environment stack.

* __`#msh environment push`__ <br />
Pushes the current environment to the environment stack.

* __`#msh environment reload`__ <br />
Re-reads the included header files from disc.

* __`#msh environment reset`__ <br />
Resets the environment to its initial state. It does not change the environment stack.

* __`#msh environment save <path>`__ <br />
Saves the environment into a file. This is disabled by default. It can be enabled using the --enable_saving command line argument.

* __`#msh environment stack`__ <br />
Displays the size of the environment stack.

* __`#msh evaluate <code>`__ <br />
Evaluates code as a metaprogram. Use this if Metashell thinks about the code that it is an addition to the environment.

* __`#msh help [<command>]`__ <br />
Displays a help message.

* __`#msh mdb [-full] [<type>]`__ <br />
Starts the metadebugger. For more information see evaluate in the Metadebugger command reference.

* __`#msh precompiled_headers [on|1|off|0]`__ <br />
Turns precompiled header usage on or off. When no arguments are used, it displays if precompiled header usage is turned on.

* __`#msh quit`__ <br />
Terminates the shell.

* __`#msh verbose [on|1|off|0]`__ <br />
Turns verbose mode on or off. When no arguments are used, it displays if verbose mode is turned on.


<!-- pragma_info -->

### Metadebugger command reference

<!-- mdb_info -->
* __`evaluate [-full] [<type>]`__ <br />
Evaluate and start debugging a new metaprogram. <br />
Evaluating a metaprogram using the `-full` qualifier will expand all
  Memoization events.
  
  If called without <type>, then the last evaluated metaprogram will be
  reevaluated.
  
  Previous breakpoints are cleared.
  
  Unlike metashell, evaluate doesn't use metashell::format to avoid cluttering
  the debugged metaprogram with unrelated code. If you need formatting, you can
  explicitly enter `metashell::format< <type> >::type` for the same effect.

* __`step [over] [n]`__ <br />
Step the program. <br />
Argument n means step n times. n defaults to 1 if not specified.
  Negative n means step the program backwards.
  
  Use of the `over` qualifier will jump over sub instantiations.

* __`rbreak <regex>`__ <br />
Add breakpoint for all types matching `<regex>`.

* __`continue [n]`__ <br />
Continue program being debugged. <br />
The program is continued until the nth breakpoint or the end of the program
  is reached. n defaults to 1 if not specified.
  Negative n means continue the program backwards.

* __`forwardtrace|ft [n]`__ <br />
Print forwardtrace from the current point. <br />
The n specifier limits the depth of the trace. If n is not specified, then the
  trace depth is unlimited.

* __`backtrace|bt `__ <br />
Print backtrace from the current point.

* __`help [<command>]`__ <br />
Show help for commands. <br />
If <command> is not specified, show a list of all available commands.

* __`quit `__ <br />
Quit metadebugger.


<!-- mdb_info -->

### The full list of built-in header files

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

