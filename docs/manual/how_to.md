<h1>How to...</h1>

## see what happens during template argument deduction?

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
+ foo (DeducedTemplateArgumentSubstitution)
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
Breakpoint "foo" will stop the execution on 2 locations
(mdb) continue 2
Breakpoint 1: "foo" reached
foo<std::vector<int, std::allocator<int> > > (TemplateInstantiation)
```

## see what a type alias resolves to?

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

## see what templates are instantiated by a certain (normal C++) expression?

In some cases it is both interesting and useful to know what templates and with
what arguments a certain expression in your code instantiates. It can be very
helpful in understanding and fixing compilation errors. But even when your code
compiles and works as expected, there might be significantly more templates
involved that you thought (eg. when you use generic libraries like STL).

Let's look at the following example: a function taking advantage of variadic
templates to summarise an arbitrary number of values:

```cpp
#include <type_traits>

template <class T>
T sum(T t_)
{
  return t_;
}

template <class T, class... Ts>
typename std::common_type<T, Ts...>::type sum(T t_, Ts... ts_)
{
  return t_ + sum(ts_...);
}
```

To sum the numbers between 1 and 10, one can call
`sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)`.

We know (or think we know) what templates get instantiated by the above
expression. We can use Metashell and MDB to verify it. Let's store the above
code in `sum.hpp` and include it into the shell:

```cpp
> #include "sum.hpp"
> #include <metashell/instantiate_expression.hpp>
```

We have included a built-in header, `metashell/instantiate_expression.hpp` as
well. This header defines the `METASHELL_INSTANTIATE_EXPRESSION` macro, which
can be used to evaluate an arbitrary C++ expression in the shell. Let's do it:

```cpp
> METASHELL_INSTANTIATE_EXPRESSION( sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) )
metashell::expression_instantiated<true>
```

The shell says that all templates could be instantiated successfully (when there
are errors, the shell displays the error report). We can use MDB to see what
templates are instantiated by the expression and how they are related:

```cpp
> #msh mdb METASHELL_INSTANTIATE_EXPRESSION( sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) )
For help, type "help".
Metaprogram started
```

Now we can display the instantiation tree:

```cpp
(mdb) ft
METASHELL_INSTANTIATE_EXPRESSION( sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) )
+ sum<int, int, int, int, int, int, int, int, int, int> (TemplateInstantiation from <stdin>:2:26)
| ` sum<int, int, int, int, int, int, int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|   ` sum<int, int, int, int, int, int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|     ` sum<int, int, int, int, int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|       ` sum<int, int, int, int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|         ` sum<int, int, int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|           ` sum<int, int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|             ` sum<int, int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|               ` sum<int, int> (TemplateInstantiation from ./sum.hpp:12:15)
|                 ` sum<int> (TemplateInstantiation from ./sum.hpp:12:15)
` metashell::expression_instantiated<true> (TemplateInstantiation from <stdin>:2:99)
```

> Note that `decltype( sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) )` shows the result of
> instantiating the function template signature only.
> `METASHELL_INSTANTIATE_EXPRESSION` instantiates the body of the function
> templates (and further template instantiations triggered by them) as well.

## debug expressions involving SFINAE?

Expressions leveraging on SFINAE (short for Substitution Failure Is Not An
Error) are very common in template metaprograms. The term refers to the fact
that if errors occur while substituting template arguments to a function or
class template, that candidate is just simply discarded from the candidate set
of the overload resolution and an actual error is not emitted.

Expressions involving SFINAE can be very complex, and sometimes it is very
difficult to reason about why a particular candidate was or wasn't choosen in
the overload resolution just by looking at the code. Mdb can help in debugging
this by showing you all the types that are instantiated while templates are
substituted.

Let's take a look at an example. The following code section is a slightly
modified version of the `make_unique` implementation from
[N3656](https://isocpp.org/files/papers/N3656.txt).

```cpp
#include <memory>
#include <cstddef>
#include <utility>
#include <type_traits>

template<class T> struct unique_if {
  typedef std::unique_ptr<T> single_object;
};

template<class T> struct unique_if<T[]> { // line 10
  typedef std::unique_ptr<T[]> unknown_bound;
};

template<class T, size_t N> struct unique_if<T[N]> {
  typedef void known_bound;
};

template<class T, class... Args>
typename unique_if<T>::single_object
make_unique(Args&&... args) { // line 20
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
typename unique_if<T>::unknown_bound
make_unique(size_t n) { // line 26
  typedef typename std::remove_extent<T>::type U;
  return std::unique_ptr<T>(new U[n]());
}

template<class T, class... Args>
typename unique_if<T>::known_bound
make_unique(Args&&...) = delete; // line 33
```

Let's start metashell, include this file and start mdb with an instantiation of
`make_unique`:

```cpp
> #include "make_unique.hpp"
> #msh mdb decltype(make_unique<int>(15))
```

Now let's take a look at the forwardtrace:

```cpp
(mdb) ft
decltype(make_unique<int>(15))
+ make_unique at ./make_unique.hpp:33:1 (ExplicitTemplateArgumentSubstitution from <stdin>:2:35)
| ` unique_if<int> at ./make_unique.hpp:6:26 (TemplateInstantiation from ./make_unique.hpp:32:1)
+ make_unique at ./make_unique.hpp:26:1 (ExplicitTemplateArgumentSubstitution from <stdin>:2:35)
| ` unique_if<int> at ./make_unique.hpp:6:26 (Memoization from ./make_unique.hpp:25:1)
+ make_unique at ./make_unique.hpp:20:1 (ExplicitTemplateArgumentSubstitution from <stdin>:2:35)
| + unique_if<int> at ./make_unique.hpp:6:26 (Memoization from ./make_unique.hpp:19:1)
| ` unique_if<int> at ./make_unique.hpp:6:26 (Memoization from ./make_unique.hpp:19:10)
+ make_unique at ./make_unique.hpp:20:1 (DeducedTemplateArgumentSubstitution from <stdin>:2:35)
+ make_unique<int, int> at ./make_unique.hpp:20:1 (TemplateInstantiation from <stdin>:2:35)
` std::__1::unique_ptr<int, std::__1::default_delete<int> > at /tmp/just-svfS9J/metashell_environment.hpp:9:12 (TemplateInstantiation from <stdin>:2:59))
```

The important parts of the trace are the ExplicitTemplateArgumentSubstitution
events and the DeducedTemplateArgumentSubstitution event. If you take a look
at the line numbers at the ExplicitTemplateArgumentSubstitution events, you can
see, that the compiler tried to instantiate all three overloads of
`make_unique`, but only one of them succeeded. The successful one is denoted by
the DeducedTemplateArgumentSubstitution event. The last event shows that indeed,
the non-array version of `make_unique` got instantiated in the end by the
expression.

You can try instantiating the two other overloads of `make_unique` and see how
the compiler instantiates the types in those cases.
