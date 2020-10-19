# Mocking Support

GCC supports the --wrap=function flag to enable mocking of library functions.  This replaces calls to function() with __wrap_function() and makes the underlying implementation available as __real_function().  The Storage Performance Development Kit uses this to implement mocking in the unit tests.

The MSVC linker does not support this flag, so the equivalent functionality has been simulated as follows:

### wdpk/wrap.h

The wpdk/wrap.h header file redefines wpdk_function() to __wrap_function(), enabling the mock functions if they are defined.

This file is currently manually generated and only covers the functions that are known to be mocked by the SPDK.

In future, it would be possible to use version.map to autogenerate wrap.h to cover all functions defined by the WPDK library.

### Static Library

The wpdk/wrap.h file contains linker /alternatename directives to define two aliases for wpdk_function():
- __wrap_function()
- __real_function()

If the application defines __wrap_function it overrides the definition in the shared library.

### Shared Library

The shared library exports.def file is built to define three symbols for each function():
- wpdk_function()
- __wrap_function()
- __real_function()
