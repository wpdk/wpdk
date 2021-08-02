# Windows Platform Development Kit

The Windows Platform Development Kit (WPDK) enables applications based on the Storage Performance Development Kit ([SPDK](http://www.spdk.io)) to build and run as native Windows executables, bringing the power and performance of NVMe and NVMe-oF to Windows. It provides header files and a lightweight library that implement the required POSIX/Linux functionality.

The project grew out of work being done by [MayaData](https://mayadata.io/), the Data Agility company, to build [OpenEBS Mayastor](https://github.com/openebs/mayastor), a high speed Container Attached Storage stack for [OpenEBS](https://openebs.io/).

The scope of the project is limited to supporting the Storage Performance Development Kit. Unlike Cygwin, it is not intended to be a generic POSIX emulation library. Functionality is mapped as closely as possible to existing Windows semantics with the minimum of emulation.

# In this ReadMe

* [Documentation](#documentation)
* [Source Code](#source)
* [Getting Started](#start)
* [Runtime Prerequisites](#prereq)
* [Current Status](#status)
* [Limitations](#limitations)
* [Known Issues](#issues)
* [Unit Tests](#unit)
* [Examples](#examples)
* [Contributing](#contrib)
* [Dependencies](#depend)
* [Acknowledgements](#acknowledge)
* [Core Maintainers](#core)

<a id="documentation"></a>
## Documentation

Further information about the [design and implementation](https://github.com/wpdk/wpdk/blob/master/doc/design.md) can be found in the [project documentation](https://github.com/wpdk/wpdk/blob/master/doc).

<a id="source"></a>
## Source Code

The source code can be obtained using:
~~~{.sh}
git clone https://github.com/wpdk/wpdk
~~~

<a id="start"></a>
## Getting Started

The project can be built and tested independently, but it is intended to be used in conjunction with SPDK. SPDK requires a POSIX-like environment for configuration, compiling and testing. Determining the best way to achieve this is still work in progress, but currently the recommended option is cross-compilation under Linux / WSL.

A Getting Started guide is available:

* [Building SPDK for Windows](https://github.com/wpdk/wpdk/blob/master/doc/build-spdk.md)

For more advanced use, the following Getting Started guides are also available:

* [Building SPDK for Windows using MSYS2](https://github.com/wpdk/wpdk/blob/master/doc/build-spdk-msys2.md)
* [Building WPDK without SPDK](https://github.com/wpdk/wpdk/blob/master/doc/build-wpdk.md)

<a id="prereq"></a>
## Runtime Prerequisites

A system running Windows build 17063 or later is required to support AF_UNIX sockets
which are used for SPDK configuration.

Running SPDK on Windows depends upon the work being done by the
[DPDK community](https://www.dpdk.org/) to add support for Windows. Please
follow [Running DPDK Applications](https://doc.dpdk.org/guides/windows_gsg/run_apps.html)
in the [Getting Started Guide for Windows](https://doc.dpdk.org/guides/windows_gsg/index.html)
to grant 'lock pages in memory' privileges and to load the 'virt2phys' driver which can be
found in the [dpdk-kmods](https://git.dpdk.org/dpdk-kmods/) repository.

Note: It is possible to run an iSCSI or NVMe over TCP target stack without the
'virt2phys' driver, but this currently requires a patch to SPDK. The 'windows' branch of
[wpdk/spdk](https://github.com/wpdk/spdk) already includes this, or it
can be obtained using:

~~~{.sh}
# https://review.spdk.io/gerrit/c/spdk/spdk/+/6697
git apply ../wpdk/scripts/patches/spdk-rfc-when-virt2phys-is-unavailable.diff
~~~

Access to physical NVMe disks requires loading the 'netuio' and 'virt2phys' drivers from
the [dpdk-kmods](https://git.dpdk.org/dpdk-kmods/)
repository. See the README files in the repository for build and installation instructions.
Additional information can be found in [Using a Physical NVMe Disk](https://github.com/wpdk/wpdk/blob/master/doc/physical-disk.md).

<a id="status"></a>
## Current Status

The project is at an alpha stage:

* All of the SPDK source compiles, apart from spdk_top which requires *libcurses*.
* All of the SPDK Unit Tests pass.
* The iSCSI target can serve storage.
* The NVMe over TCP target can serve storage.
* The SPDK stack can attach to a physical NVMe disk and issue I/O.
* Unit tests exist for the majority of the WPDK functionality.

<a id="limitations"></a>
## Limitations

* There are still a few areas that are currently unimplemented, or where quick hacks have been applied.
These are indicated in the code with *WPDK_UNIMPLEMENTED* and *HACK*.

* Where new functionality has been implemented (epoll, aio, etc) the initial focus has been
on functional correctness rather than performance.

* Currently only x64 builds are supported.

* Python 3.9 does not support AF_UNIX for sockets on Windows which means that the SPDK ./scripts/rpc.py does not work. Currently, the best workaround is to use WSL to access the socket file which can be located in either /mnt/c/tools/msys64/var/tmp or %TEMP%.

<a id="issues"></a>
## Known Issues

* SPDK applications will fail to start if DPDK cannot allocate large pages of memory. If memory is available but allocations are failing, try rebooting to reduce fragmentation.

<a id="unit"></a>
## Unit Tests

Unit tests are available for *libcunit* and WPDK. Once built, these can be run with:

~~~{.sh}
.\build\bin\test_cunit.exe
.\build\bin\wpdk_unit.exe
~~~

The file .\autotest.bat is provided as a convenience.

<a id="examples"></a>
## Examples

The generated SPDK binaries in *spdk*\build\bin can be used to create example target stacks.

~~~{.sh}
# 64MB loopback iSCSI RAM disk
.\iscsi_tgt.exe -c wpdk\test\iscsi\iscsi_loopback.json

# Physical NVMe disk presented over iSCSI
.\iscsi_tgt.exe -c wpdk\test\iscsi\iscsi_nvme.json

# 50MB NVMe over TCP RAM disk (replace LOCAL-IP in nvmf_tgt.json)
.\nvmf_tgt.exe -c wpdk\test\iscsi\nvmf_tgt.json"
~~~

<a id="contrib"></a>
## Contributing

Contributions are welcome and needed! It is expected that the process will be modelled on the [SPDK Development Guidelines](https://spdk.io/development/).
In these initial stages of the project, please email the [maintainers](https://github.com/wpdk/wpdk/blob/master/MAINTAINERS.md) directly.

Please join the [SPDK community](https://spdk.io/community) and tell us how you are using SPDK on Windows. For real-time discussions, the SPDK Slack contains a [Windows channel](https://spdk-team.slack.com/archives/C01Q700GPGU).

<a id="depend"></a>
## Dependencies

The WPDK repository currently contains the following forked projects:

* [SPDK](https://github.com/wpdk/spdk) - based on the latest SPDK repo with Windows specific changes in the 'windows' branch.
There is an ongoing dialog with the SPDK community and many changes have already been upstreamed.
* [DPDK](https://github.com/wpdk/dpdk) - required temporarily until fixes are upstreamed.
* [CUnit](https://github.com/wpdk/CUnit) - a convenience project for build purposes (no development).

<a id="acknowledge"></a>
## Acknowledments
The Windows Platform Development Kit has been developed and contributed by [MayaData](https://mayadata.io/), the Data Agility company.

Portions of the code are based on work done by the [DPDK community](https://www.dpdk.org/) to add support for Windows.

<a id="core"></a>
## Core Maintainers

The [core maintainers](https://github.com/wpdk/wpdk/blob/master/MAINTAINERS.md) primary responsibility is to provide technical oversight for the WPDK Project. The current list includes:
* [Nick Connolly](https://github.com/nconnolly1), [MayaData](https://mayadata.io/)
