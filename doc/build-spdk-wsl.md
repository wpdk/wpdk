# Building SPDK for Windows using WSL

SPDK can be built for Windows using a WSL distribution and the MinGW cross compiler.

The examples in this Getting Started guide assume a Debian based distribution with the *apt* package manager. The instructions should translate readily to other distributions.

<a id="git"></a>
## Git Attributes

Install git and curl as follows:
~~~{.sh}
sudo apt update
sudo apt install git curl
~~~

To ensure that correct CR/LF line endings are used, add the definitions in
./wpdk/scripts/gitattributes as global gitattributes. If this is the first time that
git is being configured, then this can be done using the following:

~~~{.sh}
cd ~
curl -LJ -o .gitattributes https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes
git config --global --add core.attributesFile ~/.gitattributes
~~~

<a id="source"></a>
## Source Code

The source code for SPDK and WPDK can be obtained using:

~~~{.sh}
git clone https://github.com/wpdk/spdk
cd spdk
git checkout windows
git submodule update --init
~~~

<a id="prerequisites"></a>
## Prerequisites

The prereqisites required to build WPDK and SPDK can be installed from
the shell, using:

~~~{.sh}
sudo scripts/pkgdep.sh
sudo apt install gcc-mingw-w64 g++-mingw-w64 yasm
~~~

<a id="build"></a>
## Build

The build system uses Meson and Ninja and a shell script is provided as a convenience:

~~~{.sh}
./wpdk/build.sh [release|debug] [clean|rebuild]
~~~

The shell script remembers the previous configuration and only changes need to be specified on the command line.

SPDK usually builds executables without a suffix and the SPDK scripts are written with this expectation.
However, on Windows the standard *.exe* suffix has to be used. In order to help resolve this, *build.sh*
creates symbolic links so that either name will work.

<a id="runtime"></a>
## Runtime Prerequisites
Ensure that the [Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq) have been set up on the Windows host.

<a id="tests"></a>
## Unit Tests

The SPDK unit tests can then be run as detailed in the [SPDK documentation](https://github.com/spdk/spdk#unit-tests):
~~~{.sh}
./test/unit/unittest.sh
~~~
