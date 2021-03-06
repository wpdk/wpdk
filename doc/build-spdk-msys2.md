# Building SPDK for Windows using MSYS2

<a id="prerequisites"></a>
## Prerequisites

The prereqisites required to build WPDK and SPDK can be installed from
a Windows Command Line running with Administrator privilege, using:

~~~{.sh}
curl -LJO https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/pkgdep.bat
.\pkgdep.bat [gcc|clang]
~~~

These include:

* Git
* Clang and Visual Studio 2019, or MinGW (GCC >= 8.3 win32 threads)
* Meson (>= 0.55.0)
* NASM
* MSYS2 (with make, python3, diffutils)

Installation of the prerequisites has been tested on a Windows 10 2004
Windows Sandbox. The pkgdep.bat script is intended to indicate what needs to be
installed, but may not apply to all environments. For convenience, the Chocolatey
package manager is used.

Open a new Windows Shell after pkgdep.bat completes to ensure that the changes
to the PATH variable are seen.

<a id="attrib"></a>
## Git Attributes

To ensure that correct CR/LF line endings are used, add the definitions in
.\wpdk\scripts\gitattributes as global gitattributes. If this is the first time that
git is being configured, then this can be done using the following:

~~~{.sh}
cd %USERPROFILE%
curl -LJ -o .gitattributes https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes
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

<a id="build"></a>
## Build

The build system uses Meson and Ninja and a batch file is provided as a convenience:

~~~{.sh}
.\wpdk\build.bat [release|debug] [gcc|clang] [clean|rebuild]
~~~

The batch file remembers the previous configuration and only changes need to be specified on the command line. The resulting executables have only been tested within the host operating system and not within a Windows Sandbox.

<a id="runtime"></a>
## Runtime Prerequisites
Ensure that the [Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq) have been set up on the Windows host.

<a id="tests"></a>
## Unit Tests

A shell with the correct environment can be started with:
~~~{.sh}
.\wpdk\build.bat sh
~~~

The SPDK unit tests can then be run as detailed in the [SPDK documentation](https://github.com/spdk/spdk#unit-tests):
~~~{.sh}
./test/unit/unittest.sh
~~~
