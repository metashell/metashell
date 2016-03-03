# Demo server

The code running and updating the demo server is part of the main repository.
This document describes how it can be used to run a Metashell demo server and
how the code works.

## Running a Metashell demo server

Requirements:

* Ubuntu server 14.04. Note that it might work on other systems, but it is not
  tested.
* At least 10 GB free space
* A number of dependent packages (see the list of them in the installation
  instructions)

Installation:

Before you install the demo, you need to decide which branch of Metashell to use
for getting the server's code. Unless you'd like to test a specific branch, you
should use `master`. These steps use this branch. If you'd like to use a
different branch, you need to replace `master` with the name of your branch in
the commands.

* Download the installer:
  `wget https://raw.githubusercontent.com/sabel83/metashell/master/tools/demo_server/bin/install`
* Run the installer:
  `METASHELL_BRANCH=master /bin/bash install`
* The above step will install
  [Shell in a Box](https://code.google.com/p/shellinabox/) on your server if it
  was not installed already. Installing it will run it as well. If you don't
  need it, you can stop and disable it by running:
  `sudo /etc/init.d/shellinabox stop` and `sudo update-rc.d shellinabox disable`

The installation script is configurable using environment variables. Here are
the options and ther default values:

* `METASHELL_BRANCH=master` The branch to get the server's code (eg. updater
  script, wrapper for Metashell, etc) from.
* `INSTALL_DIR=/usr/local/share/metashell` The directory to use for fetching
  and building Metashell and libraries provided by the demo.
* `SERVICE_USER=nobody` The user to run the demo Metashell processes as.
* `SERVICE_GROUP=nogroup` The group to use for the demo.
* `SERVICE_PORT=4202` The port ShellInABox will be listening on.
* `DELAY_SECS_BETWEEN_PULLS=3600` The delay the deployer script waits between
  the pulls (regardless of how long a pull & build takes)
* `SPLASH=` Text to display in the splash message of the shell.

## Uninstalling the Metashell demo server

Run the following command:
`wget -O- https://raw.githubusercontent.com/sabel83/metashell/master/tools/demo_server/uninstall | /bin/bash`

Note that if you installed the demo into a directory other than the default, you
need to set the `INSTALL_DIR` environment variable here for the uninstaller
script to remove that as well.

You can optionally remove the dependent packages from your server (you can find
the list of them in the installation steps).

## How the demo server works

The demo consists of two daemons on your server:

* `metashell_upgrader` is responsible for upgrading Metashell from the main
  repository and the libraries provided by the demo.
* `metashell_demo` is responsible for running ShellInABox for Metashell.

As they are deployed as two daemons, you can turn one or the other off.

The `tools/demo_server/bin/deploy` script takes a config file describing a
number of repositories to clone, build and deploy. It deploys/updates the code.
This script is executed regularly by the `metashell_upgrader` daemon.

### Cloning repositories

The `deploy` script downloads the code from a number of repositories (specified
by the config file). These repositories are cloned into `INSTALL_DIR/src`. Each
repository is given an identifier (specified by the config file) and is cloned
into a subdirectory with the same name. Under that directory, each branch of the
remote repository has its own clone in a directory with the branch name.

For example if there is a Git repository with the following branches:

* `master`
* `develop`
* `some_feature`

and the config file gives this repository the name `some_lib`, the following
directories are created:

* `src/some_lib/master` The repository is cloned here and the `master` branch is
  checked out.
* `src/some_lib/develop` The repository is cloned here and the `develop` branch
  is checked out.
* `src/some_lib/some_feature` The repository is cloned here and the
  `some_feature` branch is checked out.

Note that subsequent execution of the `deploy` script updates the checked-out
code and deletes subdirectories when a branch gets deleted in the remote
repository.

### Deploying headers and binaries

After cloning the repositories, the `deploy` script builds the code (according
to the build commands specified by the config file) and deploys the binaries
and headers (specified by the config file).

#### Deploying the binaries

The binaries are deployed into `INSTALL_DIR/bin`. The files are suffixed with
`_<git commit hash>` of the commit they are coming from. A symlink is created
for each branch in the same directory pointing to the binary coming from the
head of the branch. For example if a repository has a `master` branch and
deploys the `application` binary, the script will create
`INSTALL_DIR/bin/application_<git commit hash>` and
`INSTALL_DIR/bin/application_master` pointing to
`application_<git commit hash>`.

Note that old binaries not referred to by a branch are deleted.

#### Deploying the headers

The headers are deployed into
`INSTALL_DIR/include/<name of config>_<git commit hash>`. A symlink is created
for each branch similarly to the way it is done for binaries.

#### Deploying shared files

Shared files are deployed into
`INSTALL_DIR/share/<name of config>_<git commit hash>`. A symlink is created
for each branch similarly to the way it is done for binaries.

### The config file

The `deploy` script has a config file describing which projects to download and
build. The config file is a JSON document (and the one used is located in
`tools/demo_server/config/metashell.json`), which contains an array of config
entries. Each entry describes on project to download and build. Each entry is an
object with the following fields:

* `name`: The name of the project. This is used the unique identifier of the
  project.
* `git`: The source to clone the code from using Git.
* `build`: (optional) The commands to build the downloaded code. The value of it
  is a list. Each entry is a list of strings describing one command. These
  commands are executed in the root of the cloned repository.
* `bin`: (optional) The list of binaries to deploy. The value of it is a list of
  strings. Each string is a path (relative to the root of the cloned repository)
  of a file to deploy.
* `include`: (optional) The list of headers to deploy. The value of it is a list
  of objects. Each object describes a file or directory to deploy.
* `share`: (optional) The list of shared files to deploy. The value of it is a
  list of objects. Each object describes a file or directory to deploy.

Entries in the `include` and `share` list are objects with the following fields:

* `src`: The path (relative to the root of the cloned repository) of the file or
  directory to deploy.
* `dst`: (optional) The name of the deployed file or directory.
* `to_config`: (optional) The name of a config object. When specified, the
  file or directory will be deployed as a file belonging to the specified
  project, not the one it is coming from.

The value of the `src` field can contain a `*` character. It is a wildcard for
a path entry. Note that when it is used, it has to be an entire entry of the
path (eg. `include/*/boost` is ok, `include/1.*/boost` is not). Also note that
it matches hidden files.

When `*` is used in `src`, then all matches will be deployed. The values it
matches will override the branch of the repository, the files are coming from.
For example if `include/*/boost` is used and there is a directory called
`include/1.0/boost`, then it will be deployed as if it was coming from the
branch `1.0` of the repository referred to by the `git` element of the config.
