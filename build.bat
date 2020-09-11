@echo off
@setlocal enableextensions

set CC=cl
set TYPE=debug
set CLEAN=

if exist build-tmp\_clang set CC=clang
if exist build-tmp\_release set TYPE=release

if "%1"=="cl" set CC=cl&& shift /1
if "%1"=="clang" set CC=clang&& shift /1
if "%1"=="debug" set TYPE=debug&& shift /1
if "%1"=="release" set TYPE=release&& shift /1
if "%1"=="clean" set CLEAN=y&& shift /1

if not exist build-tmp\_%CC% set CLEAN=y
if not exist build-tmp\_%TYPE% set CLEAN=y

set vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe
if "%CC%%VCINSTALLDIR%"=="cl" if exist "%vswhere%" for /f "tokens=*" %%i in ('"%vswhere%" -latest -find VC') do (
	set vcvars=%%i\Auxiliary\Build\vcvarsall.bat
)
if not "%vcvars%"=="" call "%vcvars%" x64

if "%CC%%VCINSTALLDIR%"=="cl" (
	echo Needs to run from a Visual Studio Developer Command Prompt
	goto :eof
)

if not "%CLEAN%"=="" (
	echo Cleaning...
	rmdir /s /q build >nul:
	rmdir /s /q build-tmp >nul:
)

echo Building %TYPE% with %CC%...
if not exist build-tmp meson --buildtype=%TYPE% build-tmp
echo > build-tmp\_%CC%
echo > build-tmp\_%TYPE%

set DESTDIR=%CD%\build
ninja -C build-tmp install

if not "%vcvars%"=="" echo Built using "%vcvars%" x64
