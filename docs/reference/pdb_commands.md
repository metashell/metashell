# Preprocessor debugger command reference

You can find the list of PDB commands here.

<!-- pdb_info -->
* __`evaluate [-profile] [-nocache] [<expression>|-]`__ <br />
Evaluate and start debugging a new metaprogram. <br />
Evaluating a metaprogram using the `-profile` qualifier will enable
  profile mode.
  
  Instead of `<expression>`, evaluate can be called with `-`, in which case the
  whole environment is being debugged not just a single type expression.
  
  If called without `<expression>` or `-`, then the last evaluated metaprogram will
  be reevaluated.
  
  Previous breakpoints are cleared.
  
  Evaluating a metaprogram using the `-nocache` qualifier will disable caching of the events, which will prevent stepping backwards, predicting how many times a breakpoint will be hit and displaying forwardtrace.

* __`step [over|out] [n]`__ <br />
Step the program. <br />
Argument n means step n times. n defaults to 1 if not specified.
  Negative n means step the program backwards.
  
  `step over` is an alias for next.
  Use of the `out` qualifier will jump out of the current instantiation frame.
  Similarly to `next`, `step out -1` is not always the inverse of `step out`.

* __`next [n]`__ <br />
Jump over to the next instantiation skipping sub instantiations. <br />
Argument n means jump n times. n defaults to 1 if not specified.
  Negative n means step the program backwards.
  
  Please note that `next -1` is not always the inverse of `next`.
  In particular when there are no more instantiations that got instantiated
  by the current parent, then `next` will behave like a normal `step`,
  and will step out of one or more instantiation frames.
  
  `step over` is an alias for next.

* __`rbreak <regex>`__ <br />
Add breakpoint for all types matching `<regex>`.

* __`break list`__ <br />
List breakpoints.

* __`continue [n]`__ <br />
Continue program being debugged. <br />
The program is continued until the nth breakpoint or the end of the program
  is reached. n defaults to 1 if not specified.
  Negative n means continue the program backwards.

* __`finish `__ <br />
Finish program being debugged. <br />
The program is continued until the end ignoring any breakpoints.

* __`forwardtrace|ft [n]`__ <br />
Print forwardtrace from the current point. <br />
The n specifier limits the depth of the trace. If n is not specified, then the
  trace depth is unlimited.

* __`backtrace|bt `__ <br />
Print backtrace from the current point.

* __`frame|f n`__ <br />
Inspect the nth frame of the current backtrace.

* __`help [<command>]`__ <br />
Show help for commands. <br />
If <command> is not specified, show a list of all available commands.

* __`quit `__ <br />
Quit metadebugger.


<!-- pdb_info -->
