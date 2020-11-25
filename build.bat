@echo off
@setlocal enableextensions enabledelayedexpansion

set CC=clang
set TYPE=debug
set ARCH=x64
set CLEAN=

set WPDK=
set DPDK=
set SPDK=
if exist inc set WPDK=y
if exist drivers set DPDK=y
if exist dpdkbuild set SPDK=y

set "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin;%ProgramFiles%\LLVM\bin;%ProgramFiles%\MESON"
set "PATH=%PATH%;%ProgramFiles%\NASM;%SystemDrive%\tools\msys64;%SystemDrive%\MinGW\mingw64\bin"

set MSYS2=call msys2_shell -no-start -here -use-full-path -defterm

set DESTDIR=
if not "%SPDK%"=="y" set "DESTDIR=%CD%\build"
if "%WPDK%"=="y" if exist ..\dpdkbuild set "DESTDIR=%CD%\..\dpdk\build"

set cfg=
if exist build\_config set /p cfg=<build\_config

for %%i in (%cfg% %*) do (
	if "%%i"=="cl" set CC=cl
	if "%%i"=="clang" set CC=clang
	if "%%i"=="mingw" set CC=gcc
	if "%%i"=="gcc" set CC=gcc
	if "%%i"=="debug" set TYPE=debug
	if "%%i"=="release" set TYPE=release
	if "%%i"=="x64s" set ARCH=x64
	if "%%i"=="clean" set CLEAN=clean
	if "%%i"=="rebuild" set CLEAN=y
)

if not "%CLEAN%"=="clean" (
	if "%SPDK%"=="y" if not exist mk\config.mk set CLEAN=y
	if "%WPDK%%DPDK%"=="y" if not exist build-tmp\build.ninja set CLEAN=y
	if not "%cfg%"=="%CC% %ARCH% %TYPE%" (
		if not "%cfg%"=="" echo Config changed from '%CONFIG%' to '%CC% %ARCH% %TYPE%'
		set CLEAN=y
	)
)

if not "%CLEAN%"=="" (
	echo Cleaning...
	for %%i in (. dpdk wpdk) do (
		if exist %%i\build rmdir /s /q %%i\build >nul:
		if exist %%i\build-tmp rmdir /s /q %%i\build-tmp >nul:
	)
	if exist mk\config.mk del /q mk\config.mk >nul:
)

if "%CLEAN%"=="clean" goto :eof

set vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
if "%CC%%VCINSTALLDIR%"=="cl" if exist "%vswhere%" for /f "tokens=*" %%i in ('"%vswhere%" -latest -find VC') do (
	set vcvars=%%i\Auxiliary\Build\vcvarsall.bat
)
if not "%vcvars%"=="" call "%vcvars%" %ARCH%

if "%CC%%VCINSTALLDIR%"=="cl" (
	echo Requires a Visual Studio Developer Command Prompt
	goto :eof
)

if "%CC%"=="gcc" (
	where /q gcc
	if errorlevel 1 (
		echo echo Requires the MinGW GCC compiler be present in PATH
		goto :eof
	)
)

if "%CC%"=="clang" (
	where /q clang
	if errorlevel 1 (
		echo echo Requires the Clang compiler be present in PATH
		goto :eof
	)
)

set CXX=%CC%
if "%CC%"=="clang" set "CXX=clang++"
if "%CC%"=="gcc" set "CXX=g++"

set LD=
if "%CC%"=="clang" set LD=lld-link
if "%CC%"=="gcc" set LD=ld

set CFLAGS=
set CXXFLAGS=
set LDFLAGS=

echo Building %TYPE% with %CC%...
if not exist build mkdir build
echo %CC% %ARCH% %TYPE%>build\_config

if "%WPDK%%DPDK%"=="y" (
	set MESON_OPTS=
	if "%DPDK%"=="y" set "MESON_OPTS=-Dexamples=helloworld"
	if not exist build-tmp meson --buildtype=%TYPE% !MESON_OPTS! build-tmp
	ninja -C build-tmp -j8 && meson install -C build-tmp --no-rebuild --only-changed
)

if "%SPDK%"=="y" (
	set CONFIG_OPTS=
	if "%TYPE%"=="debug" set CONFIG_OPTS=--enable-debug
	if not exist mk\config.mk %MSYS2% -c "./configure !CONFIG_OPTS! --without-isal --without-vhost --without-virtio"
	%MSYS2% -c "make -j8"
)

if not "%vcvars%"=="" echo Built using "%vcvars%" %ARCH%
