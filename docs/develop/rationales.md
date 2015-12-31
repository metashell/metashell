# Rationales

## Preshell integration

Preprocessor support has been added to Metashell to provide a
[Preshell](https://github.com/sabel83/preshell) replacement. It has been added
in a way that a set of seemingly meaningless switches have been added to the
shell, which can be set in a way that turns Metashell into a Preprocessor REPL
(Read-Eval-Print) shell. These settings are the following:

* Echo the preprocessed version of each command (`#msh preprocessed echo on`)

* Hide errors related to invalid C++ code (`#msh show cpp_errors off`)

* Don't evaluate metaprograms (`#msh metaprogram evaluation off`)

Hiding errors is important, as the preprocessor might be used to preprocess
other (not C++) code. Only hiding the errors instead of skipping the validation
is imporant to maintain a valid state of the shell: the user is able to switch
back to "normal mode" at any point in time. If he entered valid C++ code in
"preprocessing mode", it remains available while invalid code gets dropped.

Not evaluating metaprograms is important to avoid unnecessary work triggered by
accidentally entering long-running template metaprogram invocations.

Another approach that has been considered compared to the implemented one is
providing a preprocessing sub-shell (like mdb) that the user can launch, use and
leave (and get back to the normal shell) once he is done. This idea has been
dropped because:

* The user might include headers, (un)define macros, etc in the preprocessor
  shell. Loosing them when leaving the preprocessor shell is not ideal.

* The preprocessor shell is very similar to the "normal" one. It would have its
  own, similar set of commands, macros, etc. (Note that the mdb shell is
  significantly different because of providing a gdb-like interface).

