# Using different compilers

Metashell uses [Clang](http://clang.llvm.org) by default, but supports other
C/C++ compilers as well. It has a concept of the compiler engine, which is a
plugin responsible for executing commands and queries using a given compiler.

> Note that set of supported
> [Metashell features](../reference/engines/index.html#available-features)
> varies across compilers. Please check the
> [Compiler Support Matrix](../reference/engines/index.html#compiler-support-matrix)
> for further reference.

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
