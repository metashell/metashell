# Creating a Metashell release

Here are the steps of creating a release of Metashell:

* Set the release number in `CMakeLists.txt` in the source root. It is defined
  by the `MAJOR_VERSION`, `MINOR_VERSION` and `PATCH_VERSION` variables.
* Download and build the code following the
  [simple build steps](getting_metashell/build/index.html) on all
  [supported platforms](getting_metashell/platforms/index.html).
    * Note that 32 and 64 bit builds are provided on most Linux platforms
      (On OpenSuSE only
      [64 bit is supported](https://forums.opensuse.org/showthread.php/510584-Missing-32-bit-version-of-new-openSUSE-42-1-)).
    * **Ubuntu**: build for the latest version and the latest LTS (they might be
      the same)
    * **Debian**: build for the latest stable version
    * **Fedora** and **OpenSuSE**: build for the latest versions
* When compilation or the tests fail for any of the platforms, fix it and start
  over again.
* Test the installer on each platform (and each version, each 32/64 bitness):
    * Install Metashell on a newly installed system
    * Copy the system test binaries over to the host (they have no dependencies)
    * Run the system tests against the installed binaries
    * Run the [manual tests](develop/testing/index.html)
* If installation or testing fails, fix it and start building the installers
  again
* Tag the latest commit with `v<version number>`.
* Push the tag to Github
* Make a release from the tag on Github
    * Copy the [changes of the release](about/changelog/index.html) as release
      notes
    * Upload the installers
* Add a new section to the beginning of
  [Binary installers](getting_metashell/installers/index.html) with the Github
  download links.

<p>&nbsp;</p>

