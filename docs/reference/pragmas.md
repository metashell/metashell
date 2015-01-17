<h1>The pragmas Metashell provides</h1>

Metashell provides a number of pragmas controlling the shell. Note that these
pragmas are available only from the shell's command prompt. If you put them in a
header file and include that header, these pragmas will be ignored.
The pragmas accepted by Metashell begin with `#pragma metashell`. Since this is
a long prefix to type, Metashell accepts shorter versions as well. Here is the
complete list of accepted forms. All of them are equivalent:

* `#pragma metashell`
* `#pragma msh`
* `#metashell`
* `#msh`

Metashell supports the following pragmas:

<!-- pragma_info -->
* __`#msh environment`__ <br />
Displays the entire content of the environment.

* __`#msh environment add <code>`__ <br />
Appends code to the environment. Use this if Metashell thinks about the code that it is an evaluation.

* __`#msh environment pop`__ <br />
Pops the last environment from the environment stack.

* __`#msh environment push`__ <br />
Pushes the current environment to the environment stack.

* __`#msh environment reload`__ <br />
Re-reads the included header files from disc.

* __`#msh environment reset`__ <br />
Resets the environment to its initial state. It does not change the environment stack.

* __`#msh environment save <path>`__ <br />
Saves the environment into a file. This is disabled by default. It can be enabled using the --enable_saving command line argument.

* __`#msh environment stack`__ <br />
Displays the size of the environment stack.

* __`#msh evaluate <code>`__ <br />
Evaluates code as a metaprogram. Use this if Metashell thinks about the code that it is an addition to the environment.

* __`#msh help [<command>]`__ <br />
Displays a help message.

* __`#msh mdb [-full] [<type>]`__ <br />
Starts the metadebugger. For more information see evaluate in the Metadebugger command reference.

* __`#msh precompiled_headers [on|1|off|0]`__ <br />
Turns precompiled header usage on or off. When no arguments are used, it displays if precompiled header usage is turned on.

* __`#msh quit`__ <br />
Terminates the shell.

* __`#msh verbose [on|1|off|0]`__ <br />
Turns verbose mode on or off. When no arguments are used, it displays if verbose mode is turned on.


<!-- pragma_info -->

<p>&nbsp;</p>

