# Testing Metashell

## Running the automated tests

Metashell has unit tests and system tests that require no manual intervention.
They can be executed by running `make test` on Unix platforms and `ctest` on
Windows.

## Running the manual tests

Some features can not be automatically tested (yet). They need to be done
manually:

### Testing the console interface

#### `Quit` stops the display of a multi-screen file list

* Run:
    * `#include <iostream>`
    * `#msh included headers`
* Press `q` and `Enter`
* **Expected:** you get Metashell's prompt back

#### Tab completion results are not mixed up

* Type `#inclu` and press Tab.
* **Expected:** it gets completed to `#include`
* Delete the `clude` part, so it remains `#in` and press Tab again.
* **Expected:** it gets completed to `#include` again.

#### Tab completion of string literal

* Type `#msh ls "vecto and press Tab.
* **Expected:** it gets completed to `#msh ls "vector"`.

<p>&nbsp;</p>

