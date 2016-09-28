# Building Metashell

This guide helps you build Metashell quickly. If you would like to work on it
(make changes, fix bugs, etc), you should take a look at the [detailed
build steps](../develop/build/index.html).

## Linux, FreeBSD and OpenBSD

* Download the source code from [github](http://github.com/metashell/metashell).
* Run `install_build_dependencies.sh` to install the dependent tools and
  libraries
* Run `build.sh` to build Clang with Templight and Metashell. To take advantage
  of multicore systems, you can set the `BUILD_THREADS` environment variable to
  the maximum number of concurrent builds. (eg. `BUILD_THREADS=4 ./build.sh`)

## Windows

* Download the source code from [github](http://github.com/metashell/metashell).
* Install [CMake](http://cmake.org/)
* Install [Python](https://www.python.org/downloads/)
* Run `build.bat` from a Visual Studio command prompt.

## Building Metashell on OS X

* Download the source code from [github](http://github.com/metashell/metashell).
* Install the dependent tools:
    * CMake
    * XCode (with Command Line Developer Tools installed)
    * The appropriate version of "Auxiliary tools for Xcode" which contains the
      PackageMaker compiler.
* Run `build.sh` to build Clang with Templight and Metashell. To take advantage
  of multicore systems, you can set the `BUILD_THREADS` environment variable to
  the maximum number of concurrent builds. (eg. `BUILD_THREADS=4 ./build.sh`)

<p>&nbsp;</p>

