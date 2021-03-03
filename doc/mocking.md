# Mocking Support

GCC supports the --wrap=function flag to enable mocking of library functions.  This replaces calls to function() with __wrap_function() and makes the underlying implementation available as __real_function().  The Storage Performance Development Kit uses this to implement mocking in the unit tests.

The MSVC linker does not support this flag, so the equivalent functionality has been simulated as follows:

## wdpk/wrap.h

The wpdk/wrap.h header file redefines wpdk_function() to __wrap_wpdk_function(), enabling the mock functions if they are defined.

References to __wrap_function() and __real_function() are redefined to include 'wpdk' in the function name. This is required for both GCC and MSVC support.

This file is currently generated from src/wrap.map and only covers the functions that are known to be mocked by the SPDK.
In future, it would be possible to use version.map to cover all functions defined by the WPDK library.

## Static Library

The wpdk/wrap.h file contains linker /alternatename directives to define two aliases for each wpdk_function():
- __wrap_wpdk_function()
- __real_wpdk_function()

If the application defines __wrap_wpdk_function it overrides the definition in the shared library.

## Shared Library

The shared library exports.def file is built to define three symbols for each function():
- wpdk_function()
- __wrap_wpdk_function()
- __real_wpdk_function()
