# Building SPDK for Windows

SPDK can be built for Windows using a Linux environment and the MinGW cross compiler.

The examples in this Getting Started guide assume a Debian based distribution with
the *apt* package manager. The instructions should translate readily to other distributions.

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
as global gitattributes. If WSL is being used it is recommended that this is setup
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

If WSL is being used, it is recommended that the source code for SPDK and WPDK is
obtained using the Windows environment.

The relevant commands are:

~~~{.sh}
git clone https://github.com/wpdk/wpdk
git clone https://github.com/spdk/spdk
cd spdk
git submodule update --init
~~~

If building with WSL, adjust the way that git symbolic links are handled
by running the following command in the Linux environment:

~~~{.sh}
../wpdk/scripts/mksymlinks.sh
~~~

This can be undone using:

~~~{.sh}
../wpdk/scripts/mksymlinks.sh rm
~~~

<a id="prerequisites"></a>
## Prerequisites

The prerequisites required to build WPDK and SPDK can be installed from
the shell, using:

~~~{.sh}
sudo scripts/pkgdep.sh
sudo apt install gcc-mingw-w64 g++-mingw-w64 yasm
~~~
The minimum requirements are:

* MinGW (GCC >= 8.3)
* Meson (>= 0.55.0)

<a id="patchdpdk"></a>
## Patch DPDK

The bulk of the changes required to support DPDK on Windows have been upstreamed,
but there is one patch still being reviewed. This is required for access to physical NVMe devices and can be applied with:

~~~{.sh}
# https://patches.dpdk.org/project/dpdk/patch/20210301095644.1711-1-nick.connolly@mayadata.io/
( cd dpdk; git apply ../../wpdk/scripts/patches/dpdk-bus-pci-nvme-on-Windows-requires-class-id-and-bus.diff )
~~~

<a id="patch"></a>
## Patch SPDK

To enable support for running SPDK without the DPDK virt2phys driver (see
[Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq)),
apply the following patch:

~~~{.sh}
# https://review.spdk.io/gerrit/c/spdk/spdk/+/6697
git apply ../wpdk/scripts/patches/spdk-rfc-when-virt2phys-is-unavailable.diff
~~~

<a id="wpdk"></a>
## Build WPDK

The build system uses Meson and Ninja and a shell script is provided as a convenience:

~~~{.sh}
cd ../wpdk
./build.sh [release|debug] [clean|rebuild]
~~~

The shell script remembers the previous configuration and only changes need to be specified on the command line.

<a id="spdk"></a>
## Build SPDK

~~~{.sh}
cd ../spdk
CC=gcc ./configure --cross-prefix=x86_64-w64-mingw32 --with-wpdk=../wpdk/build --without-isal
make -j8
~~~

A number of compiler warnings will be generated building DPDK.
These can be ignored and are resolved in the latest DPDK release.

<a id="build"></a>
## Build

A shell script that will build both WPDK and SPDK is provided as a convenience:

~~~{.sh}
../wpdk/build.sh [release|debug] [clean|rebuild]
~~~

<a id="runtime"></a>
## Runtime Prerequisites
Ensure that the [Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq) have been set up on the Windows host.

<a id="unit"></a>
## Unit Tests

SPDK usually builds executables without a suffix and the SPDK scripts are written with this expectation.
However, on Windows the standard *.exe* suffix has to be used. If the *../wpdk/build.sh* script is used,
symbolic links will be created so that either name will work. Alternatively, this can be done manually with:

~~~{.sh}
../wpdk/scripts/symlink_exe.sh
~~~

The symbolic links can be removed with:

~~~{.sh}
../wpdk/scripts/symlink_exe.sh rm
~~~

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

