# Building SPDK for Windows

SPDK can be built for Windows using either Linux, or a Windows Subsystem for Linux (WSL) distribution and the MinGW cross compiler. WSL has the advantage that it is possible to run native Windows applications directly from the shell.

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
git clone https://github.com/wpdk/wpdk
git clone https://github.com/spdk/spdk
cd spdk
git checkout master
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

The bulk of the changes required to support SPDK on Windows have been upstreamed,
but there are a couple of patches that are still being reviewed. These can be applied
(one at a time) with:

~~~{.sh}
# https://review.spdk.io/gerrit/c/spdk/spdk/+/6494
git apply ../wpdk/scripts/patches/spdk-mk-support-file-extension-for-executables.diff
~~~

~~~{.sh}
# https://review.spdk.io/gerrit/c/spdk/spdk/+/6589
git apply ../wpdk/scripts/patches/spdk-mk-add-support-for-mingw-builds.diff
~~~

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

SPDK usually builds executables without a suffix and the SPDK scripts are written with this expectation.
However, on Windows the standard *.exe* suffix has to be used. In order to help resolve this, symbolic links
can be created so that either name will work:

~~~{.sh}
../wpdk/scripts/symlink_exe.sh
~~~

These can be removed with:

~~~{.sh}
../wpdk/scripts/symlink_exe.sh rm
~~~

<a id="build"></a>
## Build

A shell script that will build both WPDK and SPDK is provided as a convenience:

~~~{.sh}
../wpdk/build.sh [release|debug] [clean|rebuild]
~~~

<a id="runtime"></a>
## Runtime Prerequisites
Ensure that the [Runtime Prerequisites](https://github.com/wpdk/wpdk#prereq) have been set up on the Windows host.

<a id="tests"></a>
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
