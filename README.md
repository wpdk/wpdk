# Windows Platform Development Kit

The Windows Platform Development Kit (WPDK) enables applications based on the Storage Performance Development Kit ([SPDK](http://www.spdk.io)) to build and run as native Windows executables by providing header files and a lightweight library that implement required POSIX/Linux functionality.

The scope of the project is limited to supporting the Storage Performance Development Kit. It is not intended to be a generic Linux emulation library like Cygwin. Functionality is mapped as closely as possible to existing Windows semantics with the minimum of emulation.

<a id="design"></a>
## Design Priciples

don't change the source code

replace system headers

wpdk_ header - allows wrapping, avoids conflict with library names

existing functionality wrapped - as well.

macros - can change variables etc

uses macros / inline - inline clash with library name and no longer inline

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
