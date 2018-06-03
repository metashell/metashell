# Changelog

## Not in any release yet

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
