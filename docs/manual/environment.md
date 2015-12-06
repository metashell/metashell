# The environment in Metashell

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

## Extending the environment

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

## The environment stack

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

## What happens to files included to the environment?

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
