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

<p>&nbsp;</p>

