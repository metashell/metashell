# Using different compilers

Metashell uses [Clang](http://clang.llvm.org) by default, but supports other
C/C++ compilers as well. The easiest way to use a different compiler is to
provide its binary after the `--` argument. For example:

```
$ metashell -- /usr/bin/gcc
```

Note that this method gives access only to external compilers. Internally used
compilers (eg. [Boost.Wave](http://boost.org/libs/wave)) require using the
`--engine` argument. See
[Explicitly specifying the compiler to use](#explicitly-specifying-the-compiler-to-use)
for further details.

Also note that the set of supported
[Metashell features](../reference/engines/index.html#available-features)
varies across compilers. Please check the
[Compiler Support Matrix](../reference/engines/index.html#compiler-support-matrix)
for further reference.

## Explicitly specifying the compiler to use

Metashell has a concept of the compiler engine, which is a
plugin responsible for executing commands and queries using a given compiler.

You can select the engine to use when launching Metashell, using the
`--engine <name of the engine>` command-line option. By default, Metashell
uses Clang. To use GCC instead, you need to run Metashell with the following
arguments:

```
$ metashell --engine gcc
```

## Compiler arguments

In most cases, extra arguments are needed for the compiler to work properly:
specify the standard to use, set the include path up, etc. As these arguments
are different for each compiler, they are handled by the engine responsible for
the compiler. Please check the
[reference of your engine](../reference/engines/index.html#available-engines) on
to see how to set them.

## Switching compilers without leaving the shell

To compare the behaviour of different compilers (or the same compiler with
different flags) you can use _configs_ in Metashell. A config is a set of
arguments for Metashell. When you start Metashell, the arguments you provide
build up a config, which is called `default`. You can display that config by
running the `#msh config show default` command from the shell. This will show
you the JSON representation of the config, which is an object with the following
fields:

* `name`: this specifies the name of the config, which you'll use to refer to
  it from Metashell.
* `engine`: the engine to use by the config.
* `engine_args`: the extra arguments to pass to the engine. This is what you
  provide after `--` from the command line.
* `use_precompiled_headers`: should the engine (if supported) use precompiled
  headers to make the shell respond quicker.
* `preprocessor_mode`: should the shell start in preprocessor mode. Setting to
  `true` has the same effect as starting Metashell with `--preprocessor`.
* `cwd`: this overrides the working directory the compiler is run from. The
  default is the directory, where Metashell was launched.

You can create a JSON file with one or more such configs in it.
Here is an example for such a config file:

```json
[
  {
    "name":"clang",
    "engine":"internal",
    "engine_args":[],
    "use_precompiled_headers":true,
    "preprocessor_mode":false
  },
  {
    "name":"gcc",
    "engine":"gcc",
    "engine_args":["/usr/bin/g++", "-std=c++11"],
    "use_precompiled_headers":true,
    "preprocessor_mode":false
  },
  {
    "name":"wave",
    "engine":"wave",
    "engine_args":[],
    "use_precompiled_headers":true,
    "preprocessor_mode":true
  }
]
```

This defines three configs: one for Clang, one for GCC and one for Boost.Wave.
If you store it in `configs.json`, you can load them in Metashell by using
the `--load_configs configs.json` argument. Note that you can use the
`--load_configs` argument multiple times and load more than one such JSON file
if needed. Also note that the name of each config has to be unique in the
shell (across the loaded JSON files).

In the shell you can list the loaded configs:

```cpp
> #msh config
/*
 *  * default
 *    clang
 *    gcc
 *    wave
 */
```

As you can see apart from the configs loaded from `configs.json`, Metashell
creates the `default` config based on the command-line arguments and that
config is active by default. You can display the details of each config by
using the `#msh config show <name of the config>` command.

You can also use the shell (and the `default` config) as normal, for example
define a new macro and try it:

```cpp
> #define CAT_IMPL(a, b) a ## b
> #define CAT(a, b) CAT_IMPL(a, b)
> #msh pp CAT(in, t)
int
```

To try the macro with GCC as well, you can switch to the `gcc` engine. You can
do this by using the `#msh config load <name of the config>` command. This keeps
your [environment](environment/index.html) (in case the other engine
would fail to parse your environment, then Metashell does not let you switch to
that config). So you can try your already defined macro with GCC as well:

```cpp
> #msh config load gcc
/*
 * Switched to config gcc
 */
> #msh pp CAT(in, t)
int
```
