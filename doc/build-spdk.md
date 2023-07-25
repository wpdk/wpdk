# Building SPDK for Windows

SPDK can be built for Windows using a Linux environment and the MinGW cross compiler.

The examples in this Getting Started guide assume a Debian based distribution with
the *apt* package manager. The instructions should translate readily to other distributions.

[Windows Subsystem for Linux (WSL)](https://github.com/wpdk/wpdk/blob/master/doc/wsl.md)
can be used as the Linux environment,
offering the advantage that Windows applications can be started directly
from the Linux shell, allowing for more straightforward testing of the SPDK
executables.

<a id="git"></a>
## Initial Setup

For WSL, follow the instructions in
[Getting Started with WSL](https://github.com/wpdk/wpdk/blob/master/doc/wsl.md).

Install Git and curl as follows:
~~~{.sh}
sudo apt update
sudo apt install git curl
~~~

To ensure that correct CR/LF line endings are used, add the definitions in
[wpdk/scripts/gitattributes](http://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes)
as global gitattributes. If this is the first time that Git is being configured,
the following can be used:

~~~{.sh}
curl -LJ -o ~/.gitattributes https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes
git config --global --add core.attributesFile ~/.gitattributes
~~~

<a id="source"></a>
## Source Code

The source code for SPDK and WPDK can be obtained with:

~~~{.sh}
git clone https://github.com/wpdk/wpdk
git clone https://github.com/spdk/spdk
cd spdk
~~~

In general, WPDK should enable the latest SPDK tree to build, but from time to time recent commits may introduce compatability issues. The latest release that is known to build successfully and run the unit tests is **v22.05**. This can be selected with:

~~~{.sh}
git checkout v22.05.x
~~~

The dependent projects should then be initialised with:

~~~{.sh}
git submodule update --init
~~~

## Symbolic Links

For WSL, update the symbolic links as detailed in
[Getting Started with WSL](https://github.com/wpdk/wpdk/blob/master/doc/wsl.md#symlinks):

~~~{.sh}
../wpdk/scripts/mksymlinks.sh
~~~

This must be done before any scripts such as pkgdep.sh are run.

<a id="prerequisites"></a>
## Prerequisites

The prerequisites required to build WPDK and SPDK can be installed from
the shell, using:

~~~{.sh}
sudo scripts/pkgdep.sh
sudo apt install gcc-mingw-w64 g++-mingw-w64 yasm pkg-config
~~~
The minimum requirements are:

* MinGW (GCC >= 8.3)
* Meson (>= 0.55.0)

<a id="patch"></a>
## Apply Patches

To enable support for running SPDK without the DPDK virt2phys driver (see
[Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq)),
apply the following patch:

~~~{.sh}
# https://review.spdk.io/gerrit/c/spdk/spdk/+/6697
git apply ../wpdk/scripts/patches/spdk-rfc-when-virt2phys-is-unavailable.diff
~~~

<a id="spdk"></a>
## Build SPDK

A shell script that will build both WPDK and SPDK is provided as a convenience:

~~~{.sh}
../wpdk/build.sh [release|debug] [clean|rebuild]
~~~

The previous configuration is remembered and only changes need to be specified on the command line.
This is equivalent to:

~~~{.sh}
( cd ../wpdk; ./build.sh [release|debug] [clean|rebuild] )
CC=gcc ./configure --cross-prefix=x86_64-w64-mingw32 --with-wpdk=../wpdk/build --without-isal
make -j8
~~~

<a id="runtime"></a>
## Runtime Prerequisites
Ensure that the [Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq) have been set up on the Windows host.

<a id="unit"></a>
## Unit Tests

SPDK usually builds executables without a suffix and the SPDK scripts are written with this expectation.
However, on Windows the standard *.exe* suffix has to be used. If the *../wpdk/build.sh* script is used,
symbolic links will be created so that either name will work, using the script:

~~~{.sh}
../wpdk/scripts/symlink_exe.sh
~~~

The symbolic links can be removed with:

~~~{.sh}
../wpdk/scripts/symlink_exe.sh rm
~~~

The SPDK unit tests can be run as detailed in the [SPDK documentation](https://github.com/spdk/spdk#unit-tests):
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

<a id="tests"></a>
## Tests

Support for running the SPDK tests is still work in progress. To experiment with running them the following setup is required:

~~~{.sh}
sudo apt install jq
sudo mv /var/tmp /var/tmp-
sudo ln -s /mnt/c/Users/<user>/AppData/Local/Temp /var/tmp
~~~

