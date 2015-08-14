<h1>Building Metashell</h1>

## Linux, FreeBSD and OpenBSD

### Using the helper scripts

* Download the source code from [github](http://github.com/sabel83/metashell).
* Run `install_build_dependencies.sh` to install the dependent tools and
  libraries
* Run `build.sh` to build Clang with Templight and Metashell. To take advantage
  of multicore systems, you can set the `BUILD_THREADS` environment variable to
  the maximum number of concurrent builds. (eg. `BUILD_THREADS=4 ./build.sh`)

### Building manually

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent libraries:
    * Readline (or Libedit)
    * Termcap
* Install the dependent tools:
    * CMake
* Build Clang with [Templight](https://github.com/mikael-s-persson/templight)
    * `cd 3rd/templight`
    * `mkdir build`
    * `cd build`
    * `cmake ../llvm -DLIBCLANG_BUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Release`
    * `make clang libclang libclang_static templight`
* Now compile Metashell. In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
        * Note: to use
          [libedit](http://thrysoee.dk/editline/) instead
          of [Readline](http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)
          add the `-DUSE_EDITLINE=true` argument to the above command line.
        * Note: if you don't want to link statically against libclang, you need to
          add `-DCLANG_STATIC=false` to the above command line.
    * `make`
    * To install it on the host run `make install`
    * To package it for Debian or Ubuntu run `cpack -G DEB`
    * To package it for OpenSUSE or Fedora run `cpack -G RPM`
* To generate code-coverage statistics for unit tests run the following commands
  in the source directory instead the above ones:
    * `mkdir bin`
    * `cd bin`
    * `cmake .. -DENABLE_CODE_COVERAGE=true -DCMAKE_BUILD_TYPE=Debug`
    * `make`
    * `make test_coverage`
    * You can find the test coverage statistics in the `bin/coverage` directory.
* To compile the source for profiling using
  [gprof](https://www.cs.utah.edu/dept/old/texinfo/as/gprof.html) (g++ only):
    * `mkdir bin`
    * `cd bin`
    * `cmake .. -DENABLE_PROFILING=true`
    * `make`
    * The binaries will now generate profiling data to gmon.out.
      Please see the gprof documentation for further information.

### Clang and Templight

The source code of Metashell contains the source code of LLVM/Clang patched with
[Templight](https://github.com/mikael-s-persson/templight). The Templight patch
can be found in the `3rd/templight/patch` directory. The naming convention is
`templight_patch_r<SVN release it patches>.diff`. To update the Clang version
used:

* Place a newer patch in the `patch` directory
* Run the `tools/get_templight.sh` script

## Windows

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install [CMake](http://cmake.org/)
* First you need to build Clang with
  [Templight](https://github.com/mikael-s-persson/templight).
    * Start a command line on your Windows host and go into the source directory
    * Run `cd 3rd\templight`
    * Run `md build`
    * Run `cd build`
    * Run `cmake ..\llvm`
    * Start Visual Studio 2013 and open the generated solution
      (`templight\build\LLVM.sln`)
    * In the _BUILD_ menu choose _Configuration Manager..._. As
      _Active solution configuration:_ choose _Release_ and click on _Close_.
      This is needed to do a release instead of a debug build of Clang.
    * In _Solution Explorer_ right-click on _Clang libraries_ and choose
      _Build_.
    * In _Solution Explorer_ right-click on _Clang executables_ and choose
      _Build_.
    * You can optionally try building the rest of the projects, but Metashell
      does not need them.
* Now you can build Metashell
    * Start a command line on your Windows host and go into the source directory
    * Run `md bin`
    * Run `cd bin`
    * Run `cmake .. -G "Visual Studio 12 2013"`.
    * CMake will generate a solution file for Metashell in the `bin` directory.
      Open it with Visual Studio 2013 and build it. You can do a debug or a
      release build (or both) against the release build of the Templight-enabled
      Clang.
    * After a successful build you can find the Metashell executables in
      `bin/app/Release/metashell.exe` or `bin/app/Debug/metashell.exe` depending
      on the build type.
* To generate an installer for Metashell:
    * Install [NSIS](http://nsis.sourceforge.net).
    * Build Metashell following the above instructions (make sure you build the
      Release config).
    * Run `cpack` in the directory you run `cmake` as well.

### Standard header files on Windows

The `windows_headers` directory contains the [MinGW](http://mingw.org/) header
files. These files are made available for Metashell.

The `tools/get_windows_headers.sh' script can (re)download these MinGW standard
header files. You need to run this script on Linux.

### WinEditLine

Metashell uses [WinEditLine](http://mingweditline.sourceforge.net/) on Windows.
To simplify the Windows build, the source code of WindEditLine is redistributed
as part of the Metashell source code and built together with Metashell.
The content of the `src` directory of the WinEditLine source code can be found
in the `3rd/wineditline` directory. The content of the original `CMakeLists.txt`
has been changed to build the code together with Metashell. If you want to
upgrade WinEditLine to a newer version, you need to update these files.

## Building Metashell on OS X

### Using the helper scripts

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent tools:
    * CMake
    * XCode
    * The appropriate version of "Auxiliary tools for Xcode" which contains the
      PackageMaker compiler.
* Run `build.sh` to build Clang with Templight and Metashell. To take advantage
  of multicore systems, you can set the `BUILD_THREADS` environment variable to
  the maximum number of concurrent builds. (eg. `BUILD_THREADS=4 ./build.sh`)

### Building manually

* Download the source code from [github](http://github.com/sabel83/metashell).
* Install the dependent tools:
    * CMake
    * XCode
* Build Clang with [Templight](https://github.com/mikael-s-persson/templight)
    * `cd 3rd/templight`
    * `mkdir build`
    * `cd build`
    * `cmake ../llvm -DLIBCLANG_BUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Release`
    * `make clang libclang libclang_static templight`
* Now compile Metashell. In the source directory run the following commands:
    * `mkdir bin`
    * `cd bin`
    * `cmake ..`
        * Note: if you don't want to link statically against libclang, you need to
          add `-DCLANG_STATIC=false` to the above command line.
    * `make`
    * To install it on the host run `make install` as root.
* To generate an installer package (.dmg):
    * Install the appropriate version of "Auxiliary tools for Xcode" which
      contains the PackageMaker compiler. On Mavericks this will be the 2012
      late July version.
    * `cpack`

## Building against custom Clang build

If you want to build Metashell against a different version of Clang or you have
built Clang on your system and would like to use that instead of rebuilding it
for Metashell, here is how you can do it.

> Note that you should apply [the Templight
> patch](https://github.com/mikael-s-persson/templight) against your Clang.

> Note that you need to build a static version of libClang. If you are building
> LLVM/Clang with CMake, you need to pass the `-DLIBCLANG_BUILD_STATIC=ON`
> argument to Clang's `cmake`.

> Also note that Metashell is prepared for being built against the version of
> Clang you can find in the `3rd/templight` directory. It is staticly linked
> against Clang and if you use a different version of Clang, the list of
> libraries to link against might be different. You can update it in
> `cmake/Modules/FindClang.cmake`.

You need to follow the manual build instructions of your platform and pass the
following extra arguments to `cmake`:

```
-DCLANG_INCLUDEDIR=$LLVM/tools/clang/include -DCLANG_BINARYDIR=$CLANG_BIN/bin -DCLANG_LIBRARYDIR=$CLANG_BIN/lib
```

where

* `$LLVM` is the path to the (top level) `llvm` directory of your Clang source
  tree.
* `$CLANG_BIN` is the path to the directory where you have built the Clang
  binaries. This is `$BUILD` on Unix systems and `$BUILD/<build mode>` on
  Windows, where `$BUILD` is the `build` directory you were running `cmake` from
  and `<build mode>` is the build mode you choose in Visual Studio (eg. Debug,
  Release, etc).

To debug configuration issues, you can pass the `-DCLANG_DEBUG=true` argument to
`cmake`. This will show you which version of Clang it found.

<p>&nbsp;</p>

