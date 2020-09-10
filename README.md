# Windows Platform Development Kit

The Windows Platform Development Kit (WPDK) enables applications based on the Storage Performance Deveopment Kit ([SPDK](http://www.spdk.io)) to build and run as native Windows executables by providing header files and a lightweight library that implement required POSIX/Linux functionality.

The scope of the project is limited to supporting the Storage Performance Development Kit. It is not intended to be a generic Linux emulation library along the lines of cygwin. Functionality is mapped as closely as possible to existing Windows semantics with the minimum of emulation.

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

<a id="build"></a>
## Build

~~~{.sh}
.\build.bat
~~~

## Unit Tests

~~~{.sh}
.\test\unit\unittest.bat
~~~
