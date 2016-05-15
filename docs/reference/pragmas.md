# The pragmas Metashell provides

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
* __`#msh echo <text>`__ <br />
Display a comment containing <text>.

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

* __`#msh included headers [<expression>]`__ <br />
Displays the list of header files (recursively) included into the environment. When <expression> is provided, it displays the headers added to the envrionment by <expression>. Headers that are included multiple times are listed only once. Headers that are not included because of being in a conditional (#if ... #endif) part that is skipped are not listed.

* __`#msh ls {<include file>|"include file"}`__ <br />
Lists the available header files in the directories specified by the arguments.

* __`#msh macro names`__ <br />
Displays the names of the defined macros

* __`#msh macros`__ <br />
Displays the macro definitions

* __`#msh mdb [-full] [<type>]`__ <br />
Starts the metadebugger. For more information see evaluate in the Metadebugger command reference.

* __`#msh metaprogram evaluation [on|1|off|0]`__ <br />
Turns evaluation of metaprograms on or off. When no arguments are used, it displays if evaluation of metaprograms is turned on.

* __`#msh metaprogram mode`__ <br />
Set Metashell to metaprogram mode

* __`#msh pp <exp>`__ <br />
Displays the preprocessed expression.

* __`#msh precompiled_headers [on|1|off|0]`__ <br />
Turns precompiled header usage on or off. When no arguments are used, it displays if precompiled header usage is turned on.

* __`#msh preprocessed echo [on|1|off|0]`__ <br />
Turns display preprocessed commands on or off. When no arguments are used, it displays if display preprocessed commands is turned on.

* __`#msh preprocessor mode`__ <br />
Set Metashell to preprocessor mode

* __`#msh quit`__ <br />
Terminates the shell.

* __`#msh quoteincludes`__ <br />
Displays the directories checked for #include "..."

* __`#msh show cpp_errors [on|1|off|0]`__ <br />
Turns display C++ errors on or off. When no arguments are used, it displays if display C++ errors is turned on.

* __`#msh sysincludes`__ <br />
Displays the directories checked for #include <...>

* __`#msh verbose [on|1|off|0]`__ <br />
Turns verbose mode on or off. When no arguments are used, it displays if verbose mode is turned on.

* __`#msh which [-all] <include file>|"include file"`__ <br />
Searches the given include file on the include path. When -all is provided, it displays all headers found, not just the first one.


<!-- pragma_info -->
