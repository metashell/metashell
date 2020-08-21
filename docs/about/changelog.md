# Changelog

## Not in any release yet

* New features
    * New engine: `auto`. It chooses the engine to use based on the compiler
      specified in the engine arguments.
    * Support for displaying the currently used engine (`#msh engine show`)
    * Support loading compilation flags from `compile_commands.json` generated
      by CMake.
    * Support `-iquote` arguments in the Wave engine adding elements to the
      quote include path only.
    * Support `-idirafter` arguments in the Wave engine adding elements to the
      include path after the system header locations.
    * Support `-U` arguments in the Wave engine undefining macros.
    * Support for switching between engines from the shell. This makes it
      possible to switch to a different compiler without leaving the shell.
      (The `#msh engine switch` pragma)
    * Make it possible to override the working directory for each config.
    * Add `-isysroot` and `--sysroot` support to the Wave engine.
    * Support `--nostdinc` and `--nostdinc++` arguments in the `wave` engine
      removing the corresponding standard headers from the standard include
      path.
    * Support for user defined warnings in configs.
    * Code completion for Metashell commands
      (`#msh ...` and `#pragma metashell`)

* Fixes
    * When multiple shell configs use the same engine with different engine
      arguments, Metashell can now use the correct arguments for each config
      instead of using the arguments the shell config had, which was loaded
      first.
    * `.` is not on the sysinclude path in the Wave engine
    * Display all `-I` arguments with `#msh sysincludes` path in the Wave engine
    * Support Clang and GCC preprocessor shell in C++98 mode

* Changes to existing behaviour
    * **Breaking change** Regular expression syntax for breakpoints changes
    from
    [Perl syntax](https://www.boost.org/doc/libs/1_68_0/libs/regex/doc/html/boost_regex/syntax/perl_syntax.html)
    to
    [ECMAScript](https://en.cppreference.com/w/cpp/regex/ecmascript).
    * The default engine is `auto`.
    * Improved error messages for invalid mdb commands.
    * Improved input validation
    * Display the name of the underlying engine in error messages about missing
      features while using the `auto` engine
    * **Breaking change** When shell configs have path separators (`\\` or `/`)
      in their name, `#msh config` treats them as directories and behaves
      similar to the `ls` command. The old behvaiour of `#msh config` is
      available using the `#msh all config` pragma.
    * The default C++ standard version of the `internal` engine is C++14.
    * Using the Visual C++ system headers on Windows instead of the MinGW ones.
    * Use the relative order of user provided and system include
      directories in the `wave` engine, that GCC is using.
    * **Breaking change** The Wave argument `--nostdinc++` has been replaced
      with `-nostdinc++` and `-nostdinc`. Using both provides the same behaviour
      as `--nostdinc++` did.
    * Passing the path of a `compile_commands.json` or a config JSON file is
      enough, the corresponding arguments (eg. `--load_configs`) is optional.

## Version 4.0.0

* New features
    * The `clang` engine of release 3.0.0 has been renamed to `templight`. Note
      that there is a new engine called `clang` (see
      _Changes to existing behaviour_).
    * New engine: `null`
    * New engine: `gcc`. Note that only preprocessor shell is supported. (No
      template metaprogram evaluation or debugging)
    * New engine: `msvc`. Note that only preprocessor shell is supported. (No
      template metaprogram evaluation or debugging)
    * New engine: `pure_wave` adding Boost.Wave support. Note that by default
      it has an empty sysinclude path (which means it has no access to standard
      headers).
    * New engine: `wave` adding Boost.Wave support with the standard headers of
      the Templight (shipped with Metashell) on the system include path and the
      macros of Templight defined by default.
    * pdb, the macro debugger
    * Support for using different configs in Metashell.
    * Caching of the metaprogram execution can be disabled in mdb and pdb with
      `-nocache`

* Fixes
    * The `templight_metashell` executable is found even if the `metashell`
      executable is behind a symlink on macOS and OpenBSD systems. This also
      broke the Homebrew version of metashell.

* Changes to existing behaviour
    * **Breaking change** The `point_of_instantiation` fields of the objects of
      the JSON interface have been renamed to `point_of_event`. (The purpose is
      to share the interface between template and macro debugging frames).
    * The command-line options displaying Markdown documentation
      (`--show_pragma_help` and `--show_mdb_help`) have been removed. They have
      been moved to another utility, which is used internally and are not part
      of the interface any longer.
    * The `clang` engine uses the YAML template instantiation dump for template
      debugging instead of the Templight dump. This makes it possible to work
      with a Clang binary (patched with Templight) and does not require a
      Templight binary. Note that profiling is not supported.
    * The input events are read lazily in the debugger.

## Version 3.0.0

* New features
    * Add pager for MDB commands which tend to have long output
    * Experimental support for profiling metaprograms in MDB
    * Experimental support for traversing the whole environment in MDB
    * New MDB command: `finish`
    * New MDB command: `break`
    * MDB now shows DeducedTemplateArgumentSubstitution events, which helps
      you to debug expressions involving SFINAE.
    * Support for displaying preprocessed code (`#msh pp`)
    * Support for displaying the preprocessed version of the shell commands
      (`#msh preprocessed echo`)
    * Support for hiding C++ errors (`#msh show cpp_errors`)
    * Support for disabling the evaluation of metaprograms
      (`#msh metaprogram evaluation`)
    * Add command to set the shell to preprocessor mode
      (`#msh preprocessor mode`)
    * Add command to set the shell to metaprogramming mode
      (`#msh metaprogram mode`)
    * Add command to display a comment in the shell (`#msh echo`)
    * Add command to display macro definitions (`#msh macros`)
    * Add command to display macro names (`#msh macro names`)
    * Add commands to display include paths
      (`#msh sysincludes` and `#msh quoteincludes`)
    * Add command to search for include files (`#msh which`)
    * Add command to list included headers (`#msh included headers`)
    * Add command to list available headers with given prefixes
      (`#msh ls`)

* Changes to existing behaviour
    * File sections displayed by MDB now shows the source location of the
      instantiated template instead of the point of instantiation.
    * **Breaking change** Compiler-dependent command line arguments (eg. -I, -D,
      etc) are accepted as compiler arguments only (after the --) and are not
      allowed as "normal" Metashell arguments.
    * **Breaking change** `--clang <clang path>` is not supported. To use a
      custom Clang, one has to use `--engine clang -- <clang path> -std=c++0x
      -ftemplate-depth=256 -Wfatal-errors`. On Windows the
      `-fno-ms-compatibility -U_MSC_VER` arguments also need to be added.
    * **Breaking change** Environment saving is enabled by default and can be
      disabled by using `--disable_saving`.
    * Reduced temporary directory creation: one shell creates one temporary
      directory only and uses that (and its subdirectories)
    * New document type in the JSON interface: `filename_list`
    * New document type in the JSON interface: `filename_set`
    * The `-nostdinc` and `-nosdtinc++` arguments remove the path of the headers
      shipped with Metashell from the system include path.

* Fixes
    * The temporary `code_complete.cpp` has been removed from the include path.
    * Leaving MDB does not crash when included file becomes invalid before
      returning to the normal shell.

* Documentation updates
    * New "How to..." section about debugging SFINAE.

## Version 2.1.0

* New features
    * Support different shell modes (plain, readline, JSON)
    * Optional logging of what Metashell is doing in the background.
    * New MDB command: `step out`
    * New MDB command: `next`, alias for `step over`
    * New MDB command: `frame`
    * MDB now can debug metaprograms which fail to compile
    * MDB now prints the relevant sections of the source code for movement
      commands
    * Significantly improved MDB performance for large metaprograms
    * New command-line arguments:
        * `--console` for choosing shell mode
        * `--log` for enabling logging
        * `--nosplash` for disabling the splash at (sub)shell startup

* Fixes
    * Detect environment setup commands that are expanded from a macro call

* Documentation updates
    * New section about `step over` in Getting started.

## Version 2.0.0

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

## Version 1.0.0

* First release
