# Design and Implementation

## Design Principles

The project conforms to the following design principles:

* A lightweight layer that runs as native Windows code, with no surprises, that can be tested independently.
* Minimal changes to the SPDK source code, for example by overloading the standard include files to add missing functionality.
* Use *wpdk_* as a prefix for new functionality to avoid clashes with the standard library or private implementations.
* Supporting packages such as *libcunit* should be included to simplify the use of the WPDK.
* Support tools to enable building the SPDK should also be included.

## Overloading

Overloading can be achieved through macros, but this risks changing variable names as well as functions. Static inline functions avoid the issue, but cause link issues if the symbol is defined as dllimport by the standard include files. A mixture of the two techniques has been used as appropriate.

Overloading library calls adds a small performance overhead, but this is minor compared with the cost of a system call.

## Mocking

A mechanism similar to GCC's --wrap is needed to enable the SPDK unit tests to run correctly. Further details about the [mocking implementation](https://github.com/wpdk/wpdk/blob/master/doc/mocking.md) can be found in the [project documentation](https://github.com/wpdk/wpdk/blob/master/doc).
