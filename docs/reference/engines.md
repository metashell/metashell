# Engines

## Available features

Metashell supports different underlying compilers and preprocessors, but not all
compilers support everything needed by Metashell. For example Boost.Wave can not
evaluate template metaprograms, as it is a preprocessor, but it is still
supported by Metashell for getting an insight into preprocessing.

The functionality of Metashell has been split up into a number of features to
make it easier to document what functionality is available for the different
compilers:

<!-- feature_info -->
* <strong id="type_shell">type shell</strong><br />
It is used for evaluating template metaprograms in Metashell.<br />
<br />
* <strong id="preprocessor_shell">preprocessor shell</strong><br />
It is used for evaluating preprocessor metaprograms in Metashell in preprocessor mode.<br />
<br />
* <strong id="code_completer">code completer</strong><br />
It is used for code-completion in the shell.<br />
<br />
* <strong id="header_discoverer">header discoverer</strong><br />
It is used for analysing the included headers and the include path.<br />
<br />
* <strong id="metaprogram_tracer">metaprogram tracer</strong><br />
It is used by MDB to get an insight into template instantiations.<br />
<br />
* <strong id="cpp_validator">cpp validator</strong><br />
It is used for checking that the code entered into Metashell is valid C++ code.<br />
<br />
* <strong id="macro_discovery">macro discovery</strong><br />
It is used for querying the defined macros.<br />
<br />
* <strong id="preprocessor_tracer">preprocessor tracer</strong><br />
It is used by PDB to get an insight into preprocessing (macro calls, includes, etc).<br />
<br />

<!-- feature_info -->

## Available engines

An engine is a Metashell plugin for executing Metashell commands using a
specific compiler. Metashell provides the following engines:

<!-- engine_info -->
* <strong id="clang">clang</strong><br />
Usage: `metashell --engine clang -- <Clang binary> -std=<standard to use> [<Clang args>]`<br />
<br />
Uses the [Clang compiler](http://clang.llvm.org) or [Templight](https://github.com/mikael-s-persson/templight). `<Clang args>` are passed to the compiler as command line-arguments. Note that Metashell requires C++11 or above. If your Clang uses such a standard by default, you can omit the `-std` argument.<br /><br />Supported features: <a href="#type_shell">type shell</a>, <a href="#preprocessor_shell">preprocessor shell</a>, <a href="#code_completer">code completer</a>, <a href="#header_discoverer">header discoverer</a>, <a href="#metaprogram_tracer">metaprogram tracer</a>, <a href="#cpp_validator">cpp validator</a>, <a href="#macro_discovery">macro discovery</a><br />
<br />
* <strong id="gcc">gcc</strong><br />
Usage: `metashell --engine gcc -- <gcc binary> -std=<standard to use> [<gcc args>]`<br />
<br />
Uses the [gcc compiler](https://gcc.gnu.org). `<gcc args>` are passed to the compiler as command line-arguments. Note that Metashell requires C++11 or above. If your gcc uses such a standard by default, you can omit the `-std` argument. Also note that currently only the preprocessor shell is supported.<br /><br />Supported features: <a href="#preprocessor_shell">preprocessor shell</a>, <a href="#header_discoverer">header discoverer</a>, <a href="#cpp_validator">cpp validator</a>, <a href="#macro_discovery">macro discovery</a><br />
<br />
* <strong id="internal">internal</strong><br />
Usage: `metashell --engine internal -- [<Clang args>]`<br />
<br />
Uses the [Templight](https://github.com/mikael-s-persson/templight) shipped with Metashell. `<Clang args>` are passed to the compiler as command line-arguments.<br /><br />Supported features: <a href="#type_shell">type shell</a>, <a href="#preprocessor_shell">preprocessor shell</a>, <a href="#code_completer">code completer</a>, <a href="#header_discoverer">header discoverer</a>, <a href="#metaprogram_tracer">metaprogram tracer</a>, <a href="#cpp_validator">cpp validator</a>, <a href="#macro_discovery">macro discovery</a><br />
<br />
* <strong id="msvc">msvc</strong><br />
Usage: `metashell --engine msvc -- <path to cl.exe> [<cl.exe args>]`<br />
<br />
Uses the [Visual C++ compiler](https://www.visualstudio.com/vs/cplusplus). `<cl.exe args>` are passed to the compiler as command line-arguments. Note that currently only the preprocessor shell is supported. You need to run Metashell from the Visual Studio Developer Prompt to use this engine.<br /><br />Supported features: <a href="#preprocessor_shell">preprocessor shell</a>, <a href="#header_discoverer">header discoverer</a>, <a href="#cpp_validator">cpp validator</a><br />
<br />
* <strong id="null">null</strong><br />
Usage: `metashell --engine null`<br />
<br />
An engine which does not support anything. Mainly for testing purposes.<br /><br />Supported features: _no features are supported_<br />
<br />
* <strong id="pure_wave">pure_wave</strong><br />
Usage: `metashell --engine pure_wave -- <Wave options>`<br />
<br />
Uses [Boost.Wave](http://boost.org/libs/wave), which is a preprocessor. Only the preprocessor shell is supported.<br /><br />Wave options:<br />&nbsp;&nbsp;`-I [ --include ]`        specify an additional include directory<br />&nbsp;&nbsp;`-S [ --sysinclude ]`     specify an additional system include directory<br />&nbsp;&nbsp;`-D [ --define ]`         specify a macro to define (as `macro[=[value]]`)<br />&nbsp;&nbsp;`--long_long`             enable long long support in C++ mode<br />&nbsp;&nbsp;`--variadics`             enable certain C99 extensions in C++ mode<br />&nbsp;&nbsp;`--c99`                   enable C99 mode (implies `--variadics`)<br />&nbsp;&nbsp;`--c++11`                 enable C++11 mode (implies `--variadics` and `--long_long`)<br />&nbsp;&nbsp;`--nostdinc++`            ignored (accepted to be compatible with the `wave` engine)<br /><br /><br />Supported features: <a href="#preprocessor_shell">preprocessor shell</a>, <a href="#header_discoverer">header discoverer</a>, <a href="#cpp_validator">cpp validator</a>, <a href="#macro_discovery">macro discovery</a>, <a href="#preprocessor_tracer">preprocessor tracer</a><br />
<br />
* <strong id="wave">wave</strong><br />
Usage: `metashell --engine wave -- <Wave options>`<br />
<br />
Uses [Boost.Wave](http://boost.org/libs/wave), which is a preprocessor. Only the preprocessor shell is supported. It uses the headers of Templight deployed with Metashell.<br /><br />Wave options:<br />&nbsp;&nbsp;`-I [ --include ]`        specify an additional include directory<br />&nbsp;&nbsp;`-S [ --sysinclude ]`     specify an additional system include directory<br />&nbsp;&nbsp;`-D [ --define ]`         specify a macro to define (as `macro[=[value]]`)<br />&nbsp;&nbsp;`--long_long`             enable long long support in C++ mode<br />&nbsp;&nbsp;`--variadics`             enable certain C99 extensions in C++ mode<br />&nbsp;&nbsp;`--c99`                   enable C99 mode (implies `--variadics`)<br />&nbsp;&nbsp;`--c++11`                 enable C++11 mode (implies `--variadics` and `--long_long`)<br />&nbsp;&nbsp;`--nostdinc++`            don't add standard headers to the include path<br /><br /><br />Supported features: <a href="#preprocessor_shell">preprocessor shell</a>, <a href="#header_discoverer">header discoverer</a>, <a href="#cpp_validator">cpp validator</a>, <a href="#macro_discovery">macro discovery</a>, <a href="#preprocessor_tracer">preprocessor tracer</a><br />
<br />

<!-- engine_info -->


## Compiler support matrix

Here is a table displaying which engine supports which features:

<!-- feature_matrix -->
|Feature                                               |<a href="#clang">clang</a>                  |<a href="#gcc">gcc</a>                      |<a href="#internal">internal</a>            |<a href="#msvc">msvc</a>                    |<a href="#null">null</a>                   |<a href="#pure_wave">pure_wave</a>          |<a href="#wave">wave</a>                    |
|:-----------------------------------------------------|:------------------------------------------:|:------------------------------------------:|:------------------------------------------:|:------------------------------------------:|:-----------------------------------------:|:------------------------------------------:|:------------------------------------------:|
|<a href="#type_shell">type shell</a>                  |<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" /> |
|<a href="#preprocessor_shell">preprocessor shell</a>  |<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|
|<a href="#code_completer">code completer</a>          |<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" /> |
|<a href="#header_discoverer">header discoverer</a>    |<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|
|<a href="#metaprogram_tracer">metaprogram tracer</a>  |<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" /> |
|<a href="#cpp_validator">cpp validator</a>            |<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|
|<a href="#macro_discovery">macro discovery</a>        |<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|
|<a href="#preprocessor_tracer">preprocessor tracer</a>|<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" /> |<img src="../../img/no.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|<img src="../../img/yes.png" width="20px" />|

<!-- feature_matrix -->

