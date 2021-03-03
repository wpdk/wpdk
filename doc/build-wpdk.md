# Building WPDK without SPDK

<a id="prerequisites"></a>
## Prerequisites

The prereqisites required to build WPDK can be installed from
a Windows Command Line running with Administrator privilege, using:

~~~{.sh}
curl -LJO https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/pkgdep.bat
.\pkgdep.bat wpdk [gcc|clang|cl]
~~~

These include:

* Git
* Clang and Visual Studio 2019, or MinGW (win32 threads)
* Meson (>= 0.55.0)

Installation of the prerequisites has been tested on a Windows 10 2004
Windows Sandbox. The pkgdep.bat script is intended to indicate what needs to be
installed, but may not apply to all environments. For convenience, the Chocolatey
package manager is used.

Open a new Windows Shell after pkgdep.bat completes to ensure that the changes
to the PATH variable are seen. Administrator privilege is not required.

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

The source code for WPDK can be obtained using:

~~~{.sh}
git clone https://github.com/wpdk/wpdk
cd wpdk
~~~

<a id="build"></a>
## Build

The build system uses Meson and Ninja and a batch file is provided as a convenience:

~~~{.sh}
.\build.bat [release|debug] [gcc|clang|cl] [clean|rebuild]
~~~

The batch file remembers the previous configuration and only changes need to be specified on the command line.

Experimental support has been added for cross compilation using WSL and the
MinGW compiler. The build can be started from Windows with the command:

~~~{.sh}
.\wpdk\build.bat [release|debug] xgcc [clean|rebuild]
~~~

<a id="testing"></a>
## Unit Tests

The unit tests can then be run with:
~~~{.sh}
.\autotest.bat
~~~
