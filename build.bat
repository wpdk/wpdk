@echo off
@setlocal enableextensions enabledelayedexpansion

set CC=cl
set TYPE=debug
set ARCH=x64
set CLEAN=

set CONFIG=
if exist build-tmp\_config set /p CONFIG=<build-tmp\_config

for %%i in (%CONFIG% %*) do (
	if "%%i"=="cl" set CC=cl
	if "%%i"=="clang" set CC=clang
	if "%%i"=="mingw" set CC=gcc
	if "%%i"=="debug" set TYPE=debug
	if "%%i"=="release" set TYPE=release
	if "%%i"=="x64" set ARCH=x64
	if "%%i"=="clean" set CLEAN=clean
	if "%%i"=="rebuild" set CLEAN=y
)

if not "%CLEAN%"=="clean" if not exist build-tmp\build.ninja set CLEAN=y
if not "%CLEAN%"=="clean" if not "%CONFIG%"=="%CC% %ARCH% %TYPE%" (
	if not "%CONFIG%"=="" echo Config changed from '%CONFIG%' to '%CC% %ARCH% %TYPE%'
	set CLEAN=y
)

if not "%CLEAN%"=="" (
	echo Cleaning...
	if exist build rmdir /s /q build >nul:
	if exist build-tmp rmdir /s /q build-tmp >nul:
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
	if errorlevel 1 if exist %SystemDrive%\MinGW\mingw64\bin set PATH=%SystemDrive%\MinGW\mingw64\bin;!PATH!
	where /q gcc
	if errorlevel 1 (
		echo echo Requires the MinGW GCC compiler be present in PATH
		goto :eof
	)
)

echo Building %TYPE% with %CC%...
if not exist build-tmp meson --buildtype=%TYPE% build-tmp
echo %CC% %ARCH% %TYPE%>build-tmp\_config

set DESTDIR=%CD%\build
ninja -C build-tmp install

if not "%vcvars%"=="" echo Built using "%vcvars%" %ARCH%
