# Changelog

## Not in any release yet

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
      (`#msh echo preprocessed`)
    * Support for hiding C++ errors (`#msh show cpp_errors`)

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
