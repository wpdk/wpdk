@echo off
@setlocal enableextensions enabledelayedexpansion

set CC=clang
set TYPE=debug
set ARCH=x64
set CROSS=
set CLEAN=
set INTERACTIVE=

set WPDK=
set DPDK=
set SPDK=
if exist inc set WPDK=y
if exist drivers set DPDK=y
if exist dpdkbuild set SPDK=y

set "PATH=%ProgramFiles%\NASM;%ALLUSERSPROFILE%\chocolatey\bin;%SystemDrive%\tools\msys64;%PATH%"
set "PATH=%ProgramFiles%\LLVM\bin;%SystemDrive%\MinGW\mingw64\bin;%ProgramFiles%\MESON;%PATH%"

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
	if "%%i"=="xgcc" set CC=xgcc
	if "%%i"=="cross" set CC=xgcc
	if "%%i"=="debug" set TYPE=debug
	if "%%i"=="release" set TYPE=release
	if "%%i"=="x64" set ARCH=x64
	if "%%i"=="clean" set CLEAN=clean
	if "%%i"=="rebuild" set CLEAN=y
	if "%%i"=="shell" set INTERACTIVE=y
	if "%%i"=="sh" set INTERACTIVE=y
)

set SH=call msys2_shell -no-start -here -use-full-path -defterm

if "%CC%"=="xgcc" (
	if not "%ARCH%"=="x64" (
		echo Cross builds are only support for x64
		goto :eof
	)
	set "CROSS=x86_64-w64-mingw32"
	if exist config\x86\cross-mingw set "CROSS=config/x86/cross-mingw"
	if exist config\!CROSS! set "CROSS=config/!CROSS!"
)

if not "%CROSS%"=="" set SH=wsl bash

if not "%CLEAN%"=="clean" (
	if "%SPDK%"=="y" if not exist mk\config.mk set CLEAN=y
	if "%WPDK%%DPDK%"=="y" if not exist build-tmp\build.ninja set CLEAN=y
	if not "%cfg%"=="%CC% %ARCH% %TYPE%" (
		if not "%cfg%"=="" echo Config changed from '%cfg%' to '%CC% %ARCH% %TYPE%'
		set CLEAN=y
	)
)

if not "%CLEAN%"=="" (
	echo Cleaning...
	for %%i in (. dpdk wpdk) do (
		if exist %%i\build rmdir /s /q %%i\build >nul:
		if exist %%i\build-tmp rmdir /s /q %%i\build-tmp >nul:
	)
	if exist isa-l\Makefile %SH% -c "cd isalbuild; make clean" >nul: 2>&1
	if exist mk\config.mk del /q mk\config.mk >nul:
	if "%SPDK%"=="y" del /q /s *.d >nul: 2>&1
)

if "%CLEAN%"=="clean" goto :eof

set "cfg=%CC% %ARCH% %TYPE%"

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

if "%CC%"=="xgcc" set CC=gcc

set CXX=%CC%
if "%CC%"=="clang" set "CXX=clang++"
if "%CC%"=="gcc" set "CXX=g++"

set LD=
if "%CC%"=="clang" set LD=lld-link
if "%CC%"=="gcc" set LD=ld

set CFLAGS=
set CXXFLAGS=
set LDFLAGS=
set ENV=CC='%CC%' CXX='%CXX%' LD='%LD%' CFLAGS='%CFLAGS%' CXXFLAGS='%CXXFLAGS%' LDFLAGS='%LDFLAGS%'

if "%INTERACTIVE%"=="y" (
	%SH%
	goto :eof
)

echo Building %cfg%...
if not exist build mkdir build
echo %cfg%>build\_config

if "%WPDK%%DPDK%"=="y" (
	set MESON_OPTS=
	if "%DPDK%"=="y" set "MESON_OPTS=-Dexamples=helloworld"
	if not "%CROSS%"=="" (
		if not exist build-tmp %SH% -c "meson --buildtype=%TYPE% !MESON_OPTS! --cross-file=%CROSS% build-tmp"
		%SH% -c "ninja -C build-tmp -j8 && DESTDIR=`wslpath '%DESTDIR%'` meson install -C build-tmp --no-rebuild --only-changed"
	) else (
		if not exist build-tmp meson --buildtype=%TYPE% !MESON_OPTS! build-tmp
		ninja -C build-tmp -j8 && meson install -C build-tmp --no-rebuild --only-changed
	)
)

if "%SPDK%"=="y" (
	set CONFIG_OPTS=
	if "%TYPE%"=="debug" set CONFIG_OPTS=--enable-debug
	if not "%CROSS%"=="" set "CONFIG_OPTS=!CONFIG_OPTS! --cross-prefix=%CROSS%"
	if not exist mk\config.mk %SH% -c "%ENV% ./configure !CONFIG_OPTS!"
	set MAKE_OPTS=
	if "%CROSS%"=="" ( for /f "tokens=*" %%i in ('where jlibtool.exe') do set "MAKE_OPTS=LIBTOOL=`cygpath '%%i'`" ) 2>nul:
	%SH% -c "make !MAKE_OPTS! REDIRECT= -j8"
)

if not "%vcvars%"=="" echo Built using "%vcvars%" %ARCH%
