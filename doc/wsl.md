<a id="wsl"></a>
# Windows Subsystem for Linux (WSL)

## Introduction

Windows Subsystem for Linux (WSL) provides a Linux environment running
under Windows. Windows applications can be started directly from the
Linux shell, allowing for more straightforward testing of the SPDK
executables.

## Versions

There are two versions of WSL:

* WSL 1 - runs within a Windows context that implements the Linux ABI.\
Available in 64-bit versions of Windows 10, from version 1607
and in Windows Server 2019.

* WSL 2 - runs within a lightweight virtual machine with a full Linux kernel.\
Only available in Windows 10, Version 1903, Build 18362 or higher.

Both versions of WSL are fully supported by Microsoft, and where WSL 2 is available,
an installed distribution can be configured to run using either version.
For further information see
[Comparing WSL 2 and WSL 1](https://docs.microsoft.com/en-us/windows/wsl/compare-versions).

WSL 1 is recommended for use with SPDK because it supports
AF_UNIX sockets between Windows and Linux which are used for configuration.

## Distributions

The following distributions have been tested with SPDK:

* Ubuntu 20.04 LTS
* Debian (latest)

The following have minor issues that can be resolved with workarounds:

* Ubuntu 22.04 LTS - scripts/pkgdep/debian.sh needs updating for SPDK 22.05

The following have been found not to work:

* Ubuntu 18.04 or older - MingGW package is not recent enough

The examples below assume an Ubuntu or Debian distribution with the *apt* package manager.
The instructions should translate readily to other distributions.

## Installation

WSL must be enabled before a distribution can be installed. The installation instructions are dependent upon the version of Windows and can be found in the [WSL Documentation](https://docs.microsoft.com/en-us/windows/wsl/):

* [Installation Guide for Windows 10 and 11](https://learn.microsoft.com/en-us/windows/wsl/install)
* [Windows Server Installation Guide](https://docs.microsoft.com/en-us/windows/wsl/install-on-server)

Multiple Linux distributions can be installed on a system at the same time.

The examples below should be issued from a non-elevated (non-Admin) Windows command prompt.

If WSL 2 is already installed and running, then the simplest way to add a new distribution as WSL 1 may be to change the default version before starting the installation:

~~~{.sh}
wsl --set-default-version 1
~~~

The default version can be restored once the installation has proceeded to a usable shell prompt, by running the following from the Windows command prompt:

~~~{.sh}
wsl --set-default-version 2
~~~

If WSL2 is supported, it is recommended that the version of the distribution is verified after installation with:

~~~{.sh}
wsl -l -v
~~~

If necessary, the version can be changed with _wsl --set-version_ as described in the [Installation Guide](https://learn.microsoft.com/en-us/windows/wsl/install#upgrade-version-from-wsl-1-to-wsl-2), but this can take significantly longer than setting the default version before starting the installation.

## Linux Shell

The WSL environment should be started from a non-elevated (non-Admin) Windows command prompt.
To start a Linux shell in the current directory, use:

~~~{.sh}
wsl
~~~

If a specific distribution is required, use:

~~~{.sh}
wsl -d <Distro>
~~~

Note: all development and testing has been done successfully using a WSL shell started from a non-elevated
(non-Admin) Windows command prompt. However, there is a report of an SPDK build failure 
_'Error getting authority: Error initializing authority: Could not connect: No such file or directory'_
being resolved by using an elevated shell. This is an isolated incident, which has not been fully investigated
and it is not a recommended approach.

## Git

Git must be installed on the Windows host. This can be done using the Chocolatey package manager by
running the following from an elevated (Admin) command prompt on the Windows host:

~~~{.sh}
curl -LJ -o %TEMP%\pkgdep.bat https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/pkgdep.bat
%TEMP%\pkgdep.bat wsl
~~~

To ensure that consistent line endings are used in Windows and WSL, add the definitions in
[wpdk/scripts/gitattributes](http://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes)
as global gitattributes. If this is the first time that Git is being configured, the following can be used on Windows:

~~~{.sh}
curl -LJ -o %USERPROFILE%\.gitattributes https://raw.githubusercontent.com/wpdk/wpdk/master/scripts/gitattributes
git config --global --add core.attributesFile %USERPROFILE%\.gitattributes
~~~

For further information see
[Resolving Git line endings issues](https://code.visualstudio.com/docs/remote/troubleshooting#_resolving-git-line-ending-issues-in-containers-resulting-in-many-modified-files).

## Source Code

It is recommended that the SPDK source code be located in the Windows file system, since some
WSL installations fail to run Windows executables from Linux paths.

<a id="symlinks"></a>
## Symbolic Links

Git symbolic links are handled differently on Windows and Linux. To resolve this, ensure that
Git has been installed on the Windows host as detailed above.
Then run the following command in the Linux environment, after the source code has
been obtained and before any scripts such as pkgdep.sh are run:

~~~{.sh}
../wpdk/scripts/mksymlinks.sh
~~~

The expected output will typically consist of details about hardlinks being created.
Any error messages indicate that the build environment is incorrectly set up and this must be resolved before proceeding.

The changes made by _mksymlinks.sh_ can be undone using:

~~~{.sh}
../wpdk/scripts/mksymlinks.sh rm
~~~
