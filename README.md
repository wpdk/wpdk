# Windows Platform Development Kit

The Windows Platform Development Kit (WPDK) enables applications based on the Storage Performance Development Kit ([SPDK](http://www.spdk.io)) to build and run as native Windows executables by providing header files and a lightweight library that implement required POSIX/Linux functionality.

The scope of the project is limited to supporting the Storage Performance Development Kit. It is not intended to be a generic Linux emulation library like Cygwin. Functionality is mapped as closely as possible to existing Windows semantics with the minimum of emulation.

<a id="design"></a>
## Design Principles

* A lightweight layer that runs as native windows code, with no surprises, that can be tested independently.
* Minimal changes to the SPDK source code, for example by overloading the standard include files to add missing functionality.
* Use *wpdk_* as a prefix for new functionality to avoid clashes with the standard library or private implementations.
* A mechanism similar to GCC's --wrap is needed to enable SPDK unit tests to run correctly.
* Supporting packages such as *libcunit* should be included to simplify the use of the WPDK.
* Support tools to enable building SPDK should also be included.

Overloading can be achieved through macros, but this risks changing variable names as well as functions. Static inline functions avoid the issue, but cause link issues if the symbol is defined as dllimport by the standard include files. A mixture of the two techniques has been used as appropriate.

<a id="status"></a>
## Current Status

The project is at prototype stage:

* Stubs have been provided to enable all of the SPDK source to compile, apart from spdk_top which requires *libcurses*.
* Functionality has been provided to enable virtually all of the SPDK Unit Tests to pass.
* The initial implementation of sockets is sufficient to enable an iSCSI target to start and to serve storage.

There are many areas that are currently unimplemented, or where quick hacks have been applied.
These are indicated in the code with *WPDK_UNIMPLEMENTED* and *HACK*.

<a id="source"></a>
## Source Code

~~~{.sh}
git clone https://github.com/wpdk/wpdk
cd wpdk
git submodule update --init
~~~

<a id="prerequisites"></a>
## Prerequisites

* Visual Studio or Clang.
* Meson 0.55.0 or later.

In addition, MSYS2 is neeeded to build the SPDK tree.

<a id="build"></a>
## Build

The build system uses Meson and Ninja and a batch file is provided as a covenience:

~~~{.sh}
.\build.bat [release|debug] [cl|clang] [clean|rebuild]
~~~

The batch file remembers the previous configuration and only changes need to be specified on the command line.

## Unit Tests

Unit tests are available for *libcunit* and the WPDK. These can be run with the batch file:

~~~{.sh}
.\autotest.bat
~~~

<a id="spdk"></a>
## Using with SPDK


<a id="contrib"></a>
## Contributing

Contributions are welcome. It is expected that the process will be modelled on the contributor guidelines for the SPDK (available [here](https://spdk.io/development/)) and the Data Plane Development Kit (DPDK) (available [here](https://doc.dpdk.org/guides/contributing/index.html)).
<a id="core"></a>

## Core Maintainers

The core maintainers primary responsibility is to provide technical oversight for the WPDK Project. The current list of WPDK core maintainers includes:
* Nick Connolly
