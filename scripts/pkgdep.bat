@echo off
@setlocal enableextensions enabledelayedexpansion

set MSC=
set CLANG=
set GCC=

for %%i in (%*) do (
	if "%%i"=="cl" set MSC=y
	if "%%i"=="clang" set CLANG=y
	if "%%i"=="mingw" set GCC=y
	if "%%i"=="gcc" set GCC=y
)

if "%MSC%%CLANG%%GCC%"=="" (
	set CLANG=y
	set GCC=y
)

if "%CLANG%"=="y" set MSC=y

net session >nul: 2>&1
if errorlevel 1 (
	echo %0 must be run with Administrator privileges
	goto :eof
)

set "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"
where /q choco
if errorlevel 1 (
	powershell -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))"
	if errorlevel 1 goto :eof
)

set "PATH=%PATH%;%ProgramFiles%\Git\bin"
where /q git
if errorlevel 1 (
	echo Installing Git ...
	choco install git -y -r
	if errorlevel 1 goto :eof
)

if "%GCC%"=="y" (
	where /q gcc
	if errorlevel 1 (
		echo Installing GCC ...
		choco install mingw -y -r
		if errorlevel 1 goto :eof
	)
)

set "PATH=%PATH%;%ProgramFiles%\LLVM\bin"
where /q clang
if errorlevel 1 if "%CLANG%"=="y" (
	echo Installing Clang ...
	choco install llvm -y -r
	if errorlevel 1 goto :eof
)

set vc=
set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%vswhere%" for /f "tokens=*" %%i in ('"%vswhere%" -latest -find VC') do set "vc=%%i"

if "%MSC%%vc%"=="y" (
	echo Installing Visual Studio ...
	choco install visualstudio2019community -y -r
	if errorlevel 1 goto :eof
	choco install visualstudio2019-workload-nativedesktop -y -r
	if errorlevel 1 goto :eof
)

set "PATH=%PATH%;%ProgramFiles%\MESON"
where /q meson
if errorlevel 1 (
	echo Installing Meson ...
	choco install meson -y -r
	if errorlevel 1 goto :eof
)

set "PATH=%PATH%;%ProgramFiles%\NASM"
where /q nasm
if errorlevel 1 (
	echo Installing NASM ...
	choco install nasm -y -r
	if errorlevel 1 goto :eof
)

set "PATH=%PATH%;%SystemDrive%\tools\msys64"
where /q msys2
if errorlevel 1 (
	echo Installing MSYS2 ...
	choco install msys2 -y -r
	if errorlevel 1 goto :eof
)

set MSYS2=call msys2_shell -no-start -here -use-full-path -defterm

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "make --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing make ...
	%MSYS2% -c "pacman --noconfirm -S msys/make"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "python3 --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing python ...
	%MSYS2% -c "pacman --noconfirm -S msys/python"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "cmp --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing diffutils ...
	%MSYS2% -c "pacman --noconfirm -S msys/diffutils"
)
