# Windows Platform Development Kit

The Windows Platform Development Kit (WPDK) enables applications based on the Storage Performance Development Kit ([SPDK](http://www.spdk.io)) to build and run as native Windows executables by providing header files and a lightweight library that implement required POSIX/Linux functionality.

The scope of the project is limited to supporting the Storage Performance Development Kit. It is not intended to be a generic Linux emulation library like Cygwin. Functionality is mapped as closely as possible to existing Windows semantics with the minimum of emulation.

<a id="design"></a>
## Design Principles

* A lightweight layer that runs as native windows code, with no surprises, that can be tested independently.
* Minimal changes to the SPDK source code, for example by overloading the standard include files to add missing functionality.
* Use *wpdk_* as a prefix for new functionality to avoid clashes with the standard library or private implementations.
* A mechanism similar to GCC's --wrap is needed to enable SPDK unit tests to run correctly.

Overloading can be achieved through macros, but this risks changing variable names as well as functions. Static inline functions avoid the issue, but cause link issues if the symbol is defined as dllimport by the standard include files. A mixture of the two techniques has been used as appropriate.

<a id="status"></a>
## Current Status

Most code compiles

Unit tests mainly pass

iSCSI stack will serve commands

<a id="spdk"></a>
## Use with SPDK

<a id="source"></a>
## Source Code

~~~{.sh}
git clone https://github.com/nconnolly1/wpdk
cd wpdk
git submodule update --init
~~~

<a id="prerequisites"></a>
## Prerequisites

MSYS2 with the following packages:

A supported compiler is required, either Visual Studio 2019 or Clang.

Meson

<a id="build"></a>
## Build

The build system uses Meson and Ninja and a batch file is provided as a covenience:

~~~{.sh}
.\build.bat [release|debug] [cl|clang] [clean|rebuild]
~~~

The batch file remembers the previous configuration and only changes need to be specified on the command line.

## Unit Tests

~~~{.sh}
.\test\unit\unittest.bat
~~~

<a id="contrib"></a>
## Contributing

Contributions are welcome. It is expected that the process will be modelled on the contributor guidelines for the SPDK (available [here](https://spdk.io/development/)) and the Data Plane Development Kit (DPDK) (available [here](https://doc.dpdk.org/guides/contributing/index.html)).
<a id="core"></a>

## Core Maintainers

The core maintainers primary responsibility is to provide technical oversight for the WPDK Project. The current list of WPDK core maintainers includes:
* Nick Connolly
