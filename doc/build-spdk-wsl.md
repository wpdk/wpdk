# Building SPDK for Windows using WSL

SPDK can be built for Windows using a WSL distribution and the MinGW cross compiler.

The examples in this Getting Started guide assume a Debian based distribution with the *apt* package manager. The instructions should translate readily to other distributions.

<a id="wsl"></a>
## Windows Subsystem for Linux (WSL)

Windows Subsystem for Linux (WSL) can be used as the Linux environment,
offering the advantage that native Windows applications can be run directly from the shell.
Using a WSL1 installation is currently recommended over WSL2 because it supports
AF_UNIX sockets which are used to configure SPDK.

Documentation for getting started with WSL can be found at
https://docs.microsoft.com/en-us/windows/wsl/.
On a system that supports WSL2, a distribution can be installed as WSL1 using:

~~~{.sh}
wsl --set-default-version 1
~~~

It is recommended that the source code be located in a Windows directory, since some
WSL installations fail to run Windows executables from Linux paths.
To start a WSL shell within the source directory, use:

~~~{.sh}
cd <source_dir>
wsl
~~~

<a id="git"></a>
## Initial Setup

Install git and curl in the Linux environment as follows:
~~~{.sh}
sudo apt update
sudo apt install git curl
~~~

On Windows, run the following from an elevated command prompt:

~~~{.sh}
curl -LJ -o %TEMP%\pkgdep.bat https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/pkgdep.bat
%TEMP%\pkgdep.bat wsl
~~~

<a id="git"></a>
## Git Attributes

To ensure that correct CR/LF line endings are used, add the definitions in
[wpdk/scripts/gitattributes](http://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes)
as global gitattributes. If WSL is being used it is recommened that this is done
in both the Linux and Windows environments. If this is the first time that git is being configured,
use the following on Linux:

~~~{.sh}
curl -LJ -o ~/.gitattributes https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes
git config --global --add core.attributesFile ~/.gitattributes
~~~

On Windows:

~~~{.sh}
curl -LJ -o %USERPROFILE%\.gitattributes https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes
git config --global --add core.attributesFile %USERPROFILE%\.gitattributes
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
The minimum requirements are:

* MinGW (GCC >= 8.3)
* Meson (>= 0.55.0)

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

<a id="unit"></a>
## Unit Tests

The SPDK unit tests can then be run as detailed in the [SPDK documentation](https://github.com/spdk/spdk#unit-tests):
~~~{.sh}
./test/unit/unittest.sh
~~~

The script will currently attempt to run unittest_ftl which will fail.
This can be fixed by adding a colon to the start of the line as follows:

~~~{.sh}
if [ $(uname -s) = Linux ]; then
	: run_test "unittest_ftl" unittest_ftl
fi
~~~

A newly introduced issue will cause nvme_io_msg_ut.exe to fail. A commit is pending to resolve this.

<a id="tests"></a>
## Tests

Support for running the SPDK tests is still work in progress. To experiment with running them the following setup is required:

~~~{.sh}
sudo apt install jq
sudo mv /var/tmp /var/tmp-
sudo ln -s /mnt/c/Users/<user>/AppData/Local/Temp /var/tmp
~~~

