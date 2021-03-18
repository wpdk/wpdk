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

For WSL, follow the instructions in the guide to 
[Getting Started with WSL](https://github.com/wpdk/wpdk/blob/master/doc/wsl.md).

Install Git and curl as follows:
~~~{.sh}
sudo apt update
sudo apt install git curl
~~~

<a id="git"></a>
## Git Attributes

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
git submodule update --init
~~~

If using WSL, update the symbolic links as detailed in
[Getting Started with WSL](https://github.com/wpdk/wpdk/blob/master/doc/wsl.md#symlinks).

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

<a id="patch"></a>
## Apply Patches

In order to access physical NVMe devices, a patch to DPDK is required which can be applied with:

~~~{.sh}
# https://patches.dpdk.org/project/dpdk/patch/20210301095644.1711-1-nick.connolly@mayadata.io/
( cd dpdk; git apply ../../wpdk/scripts/patches/dpdk-bus-pci-nvme-on-Windows-requires-class-id-and-bus.diff )
~~~

This will not be needed once SPDK updates to use the forthcoming DPDK 21.05 release.

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

A number of compiler warnings will be generated building DPDK.
These can be ignored and are resolved in the latest DPDK release.

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

A newly introduced issue will cause nvme_io_msg_ut.exe to fail. A commit is pending to resolve this.

<a id="tests"></a>
## Tests

Support for running the SPDK tests is still work in progress. To experiment with running them the following setup is required:

~~~{.sh}
sudo apt install jq
sudo mv /var/tmp /var/tmp-
sudo ln -s /mnt/c/Users/<user>/AppData/Local/Temp /var/tmp
~~~

