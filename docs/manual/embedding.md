<h1>Embedding Metashell into other applications</h1>

Metashell provides a [JSON](http://json.org/)-based interface for building a
custom front-end or embedding Metashell into IDEs. To use it, you need to start
Metashell with the `--console=json` command-line argument. The shell will read
commands from standard input and display output on standard output. The format
of the commands and the output will be structured JSON documents separated by
`\n` characters.

## Supported commands

Here is the list of commands Metashell accepts when it is running in JSON-mode.

* __cmd__ <br />
<br />
Format: `{ "type":"cmd", "cmd":"<command>" }` <br />
<br />
Sending this command to Metashell means that the user has typed `<command>` in
and pressed enter in the shell.

* __code\_completion__ <br />
<br />
Format: `{ "type":"code_completion", "code":"<code to complete>" }` <br />
<br />
Sending this command to Meashell means that the user has asked for
code-completion and the code to complete is `<code to complete>`. As a response
to this, Metashell will display an object of type `code_completion_result`.
Note that Metashell might also display errors as well.

## Supported output documents

Here is the list of JSON documents Metashell might output to display things.
The order of the fields of objects is fixed to make SAX-like parsing easier.
As in some cases (when displaying forward traces) the documents might be large,
using SAX-like JSON parsers is recommended.

* __backtrace__ <br />
<br />
Format:
`{ "type":"backtrace", "frames":[<frames of the backtrace>] }` <br />
<br />
Displays a template metaprogramming backtrace, which is a list of recursive
template instantiations. Each frame in the trace is a template instatiation
triggered by the preceeding frame. The format of the frames is the
following: <br />
<br />
`{
  "name":"<type instantiated>",
  "kind":"<kind of instatiation>"
  "point_of_instantiation":"<point of instantiation>"
}` <br />
<br />
The `name`, `kind`, `point_of_instantiation` fields are the same as the fields
of the `frame` object.

* __call\_graph__ <br />
<br />
Format:
`{ "type":"call_graph", "nodes":[<nodes of the call graph>] }` <br />
<br />
Displays a template metaprogramming call graph, which is a tree (some nodes may
appear multiple times, in which case they a separate nodes of the tree). The
list of nodes are the nodes of the graph listed in the same order as a
depth-first traversal starting from the root of the tree would visit them. The
format of the nodes is the following: <br />
<br />
`{
  "name":"<type instantiated>",
  "kind":"<kind of instatiation>",
  "point_of_instantiation":"<point of instantiation>"
  "depth":<depth of the node in the tree>,
  "children":<number of children the node has>
}` <br />
<br />
The `name`, `kind` and `point_of_instantiation` fields are the same as the
fields of the `frame` object. The values of the `<depth>` and `<children>`
fields are integers.

* __code\_completion\_result__ <br />
<br />
Format:
`{ "type":"code_completion_result", "completions":[<list of completions>] }`
<br />
<br />
This is displayed as the response to a `code_completion` command. The
`<list of completions>` is a list of strings, which are the possible completions
of the code snippet provided in the `code_completion` command.

* __comment__ <br />
<br />
Format: `{ "type":"comment", "paragraphs":[<list of paragraphs>] }` <br />
<br />
Display a C++ comment. The content of the comment is a text in paragraphs. Each
paragraph is described by a JSON object with the following format: <br />
<br />
`{
  "first_line_indentation":"<characters to indent with>",
  "rest_of_lines_indentation":"<characters to indent with>",
  "content":"<text of the paragraph>"
}` <br />
<br />
The displayer can add line-breaks to the text of the paragraph to fit the width
of the display. The whitespaces (or other characters) to use for indentation are
provided for each paragraph. The first line have a custom indentation.

* __cpp\_code__ <br />
<br />
Format: `{ "type":"cpp_code", "code":"<C++ code>" }` <br />
<br />
Display a C++ code snippet.

* __error__ <br />
<br />
Format: `{ "type":"error", "msg":"<error message>" }` <br />
<br />
An error occured. The description of the error is `<error message>`.

* __frame__ <br />
<br />
Format:
`{
  "type":"frame",
  "name":"<type instantiated>",
  "kind":"<kind of instatiation>"
  "point_of_instantiation":"<point of instantiation>"
}` <br />
<br />
Display a template class instantiation. This is treated as a stack frame of a
template metaprogram execution (this is where the name comes from). The
`<type instantiated>` is the pretty-printed version of the template instance.
The `kind` and `point_of_instantiation` fields are optional depending on
whether Metashell has this information. The possible values for kind are: <br />

    * `DefaultFunctionArgumentInstantiation`
    * `DefaultTemplateArgumentChecking`
    * `DefaultTemplateArgumentInstantiation`
    * `DeducedTemplateArgumentSubstitution`
    * `ExceptionSpecInstantiation`
    * `ExplicitTemplateArgumentSubstitution`
    * `Memoization`
    * `NonTemplateType`
    * `PriorTemplateArgumentSubstitution`
    * `TemplateInstantiation`
    * `UnknownKind`

Format of `point_of_instantiation` is `<file_name>:<row>:<column>`. For example:
`main.cpp:35:16`. There is a special file called `<stdin>` which can appear in
this field. This is a placeholder for the the code directly entered into the
shell.

* __prompt__ <br />
<br />
Format: `{ "type":"prompt", "prompt":"<prompt to display>" }` <br />
<br />
This means that the shell is waiting for input. The prompt to use is
`<prompt to display>`. After displaying this command the shell will not display
further output until the next command arrives.

* __raw\_text__ <br />
<br />
Format: `{ "type":"raw_text", "value":"<text>" }` <br />
<br />
`<text>` should be displayed.

* __type__ <br />
<br />
Format: `{ "type":"type", "name":"<pretty printed type>" }` <br />
<br />
Display a C++ type.

<p>&nbsp;</p>

