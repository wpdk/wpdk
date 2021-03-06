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

Both versions of WSL are fully supported, and where WSL 2 is available,
an installed distribution can be configured to run using either version.
For further information see
[Comparing WSL 2 and WSL 1](https://docs.microsoft.com/en-us/windows/wsl/compare-versions).

WSL 1 is recommended for use with SPDK because it supports
AF_UNIX sockets between Windows and Linux which are used for configuration.

## Installation

Installation instructions can be found in the
[WSL Documentation](https://docs.microsoft.com/en-us/windows/wsl/):

* [Installation Guide for Windows 10](https://docs.microsoft.com/en-us/windows/wsl/install-win10)
* [Windows Server Installation Guide](https://docs.microsoft.com/en-us/windows/wsl/install-on-server)

Multiple Linux distributions can be installed on a system at the same time.

If WSL 2 is supported, a distribution can be installed as WSL 1 using:

~~~{.sh}
wsl --set-default-version 1
~~~

## Distributions

The following distributions have been tested with SPDK:

* Ubuntu 20.04 LTS
* Debian (latest)

The following have been found not to work:

* Ubuntu 18.04 or older - MingGW package is not recent enough

The examples below assume a Debian based distribution with the *apt* package manager.
The instructions should translate readily to other distributions.

## Linux Shell

To start a Linux shell in the current directory, use:

~~~{.sh}
wsl
~~~

If a specific distribution is required, use:

~~~{.sh}
wsl -d <Distro>
~~~

## Git

If Git is not already setup on Windows, it can be installed using the Chocolatey package manager by
running the following from an elevated command prompt:

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

Git symbolic links are handled differently on Windows and Linux. To resolve this,
run the following command in the Linux environment after the source code has
been obtained:

~~~{.sh}
../wpdk/scripts/mksymlinks.sh
~~~

This can be undone using:

~~~{.sh}
../wpdk/scripts/mksymlinks.sh rm
~~~
