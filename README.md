# Windows Platform Development Kit

The Windows Platform Development Kit (WPDK) enables applications based on the Storage Performance Development Kit ([SPDK](http://www.spdk.io)) to build and run as native Windows executables by providing header files and a lightweight library that implement required POSIX/Linux functionality.

The project grew out of work being done by [MayaData](https://mayadata.io/) to build [Mayastor](https://github.com/openebs/mayastor), a high speed Container Attached Storage stack for [OpenEBS](https://openebs.io/).

The scope of the project is limited to supporting the Storage Performance Development Kit. It is not intended to be a generic Linux emulation library like Cygwin. Functionality is mapped as closely as possible to existing Windows semantics with the minimum of emulation.

<a id="design"></a>
## Design Principles

* A lightweight layer that runs as native windows code, with no surprises, that can be tested independently.
* Minimal changes to the SPDK source code, for example by overloading the standard include files to add missing functionality.
* Use *wpdk_* as a prefix for new functionality to avoid clashes with the standard library or private implementations.
* A mechanism similar to GCC's --wrap is needed to enable the SPDK unit tests to run correctly.
* Supporting packages such as *libcunit* should be included to simplify the use of the WPDK.
* Support tools to enable building the SPDK should also be included.

Overloading can be achieved through macros, but this risks changing variable names as well as functions. Static inline functions avoid the issue, but cause link issues if the symbol is defined as dllimport by the standard include files. A mixture of the two techniques has been used as appropriate.

Wrapping library calls adds a small performance overhead, but this is minor compared with the cost of a system call.

<a id="status"></a>
## Current Status

The project is at an alpha stage:

* All of the SPDK source compiles, apart from spdk_top which requires *libcurses*.
* All of the SPDK Unit Tests pass.
* The iSCSI target can serve storage.
* The NVMe over TCP target can serve storage.
* Unit tests exist for the majority of the WPDK functionality.

There are still a few areas that are currently unimplemented, or where quick hacks have been applied.
These are indicated in the code with *WPDK_UNIMPLEMENTED* and *HACK*.

Currently only x64 builds are supported.

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
* Clang and Visual Studio 2019, or MinGW (win32 threads)
* Meson (>= 0.55.0)
* NASM
* MSYS2 (with make, python3, diffutils)

Note:
* Installation of the prerequisites has only been tested on a Windows 10 2004
Windows Sandbox. The pkgdep.bat script is intended to indicate what needs to be
installed, but may not apply to all environments. For convenience, the Chocolatey
package manager is used.
* The Windows Shell should be closed after pkgdep.bat completes to ensure that the
changes to the PATH variable are seen.

<a id="source"></a>
## Source Code

The source code can be obtained using:

~~~{.sh}
git clone https://github.com/wpdk/wpdk
~~~

<a id="build"></a>
## Build

The build system uses Meson and Ninja and a batch file is provided as a covenience:

~~~{.sh}
.\build.bat [release|debug] [gcc|clang|cl] [clean|rebuild]
~~~

The batch file remembers the previous configuration and only changes need to be specified on the command line.

Note:
* The built executables have not been tested within Windows Sandbox, only within
the host operating system.
## Unit Tests

Unit tests are available for *libcunit* and the WPDK. These can be run with the batch file:

~~~{.sh}
.\autotest.bat
~~~

<a id="spdk"></a>
## Using with SPDK

To build SPDK, obtain the source code using:

~~~{.sh}
git clone https://github.com/wpdk/spdk
cd spdk
git checkout windows
git submodule update --init
~~~

The build system uses configure and make and a batch file is provided as a convenience:

~~~{.sh}
.\wpdk\build.bat [release|debug] [gcc|clang] [clean|rebuild]
~~~

A shell with the correct environment can be started with the command:

~~~{.sh}
.\wpdk\build.bat sh
~~~

Experimental support has been added for cross compilation using WSL and the
MinGW compiler.  To use this, ensure the definitions from .\wpdk\gitattributes
are present as global gitattributes. The build can be started from Windows
with the command:

~~~{.sh}
.\wpdk\build.bat [release|debug] xgcc [clean|rebuild]
~~~

As a quick verification, a 64MB loopback iscsi RAM disk can be created with:

~~~{.sh}
.\wpdk\test\iscsi\iscsi_loopback.bat
~~~

A 50MB NVMe over TCP RAM disk can be created with:

~~~{.sh}
.\wpdk\test\nvmf\nvmf_tgt.bat
~~~

Note:
* This project builds upon the excellent work being done by the DPDK community to add support for Windows.
Please follow the DPDK setup instructions [here](https://doc.dpdk.org/guides/windows_gsg/run_apps.html)
to enable Large Page support and virtual to physical address translation.
* Python 3.9 does not support AF_UNIX for sockets on Windows which means that the SPDK ./scripts/rpc.py
does not work. Currently, the best workaround is to use WSL to access the socket file located
in /mnt/c/tools/msys64/var/tmp.

## Forked Projects

The WPDK repository currently contains the following forked projects:

* [SPDK](https://github.com/wpdk/spdk) - based on the latest SPDK repo with Windows specific changes in the 'windows' branch.
There is an ongoing dialog with the SPDK community to upstream these changes.
* [DPDK](https://github.com/wpdk/dpdk) - required temporarily until the SPDK is updated to use DPDK 20.11.
* [CUnit](https://github.com/wpdk/CUnit) - a convenience project for build purposes (no development).

<a id="contrib"></a>
## Contributing

Contributions are welcome. It is expected that the process will be modelled on the contributor guidelines for the SPDK (available [here](https://spdk.io/development/)) and the Data Plane Development Kit (DPDK) (available [here](https://doc.dpdk.org/guides/contributing/index.html)).

In the initial stages of the project, please email the [maintainers](https://github.com/wpdk/wpdk/blob/master/MAINTAINERS.md) directly.

<a id="core"></a>
## Core Maintainers

The core maintainers primary responsibility is to provide technical oversight for the WPDK Project. The current list of WPDK core maintainers includes:
* [Nick Connolly](https://github.com/nconnolly1), [MayaData](https://mayadata.io/)
