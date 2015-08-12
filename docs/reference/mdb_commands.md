<h1>Metadebugger command reference</h1>

You can find the list of MDB commands here.

<!-- mdb_info -->
* __`evaluate [-full] [-profile] [<type>]`__ <br />
Evaluate and start debugging a new metaprogram. <br />
Evaluating a metaprogram using the `-full` qualifier will expand all
  Memoization events.
  
  If called without `<type>`, then the last evaluated metaprogram will be
  reevaluated.
  
  Previous breakpoints are cleared.
  
  Unlike metashell, evaluate doesn't use metashell::format to avoid cluttering
  the debugged metaprogram with unrelated code. If you need formatting, you can
  explicitly enter `metashell::format< <type> >::type` for the same effect.
  
  The qualifier `-profile` is intentionally undocumented. It is only used for
  internal profiling, and could be changed or removed at any time.

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

* __`frame|f `__ <br />
Print the nth frame from the current point.

* __`help [<command>]`__ <br />
Show help for commands. <br />
If <command> is not specified, show a list of all available commands.

* __`quit `__ <br />
Quit metadebugger.


<!-- mdb_info -->

<p>&nbsp;</p>

