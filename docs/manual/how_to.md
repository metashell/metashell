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

<p>&nbsp;</p>

