@echo off
@setlocal enableextensions enabledelayedexpansion

set MSC=
set CLANG=
set GCC=
set WPDK=
set WSL=

for %%i in (%*) do (
	if "%%i"=="cl" set MSC=y
	if "%%i"=="clang" set CLANG=y
	if "%%i"=="mingw" set GCC=y
	if "%%i"=="gcc" set GCC=y
	if "%%i"=="wpdk" set WPDK=y
	if "%%i"=="wsl" set WSL=y
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

set "PATH=%ALLUSERSPROFILE%\chocolatey\bin;%PATH%"
where /q choco
if errorlevel 1 (
	powershell -NoProfile -InputFormat None -ExecutionPolicy Bypass -Command "[System.Net.ServicePointManager]::SecurityProtocol = 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))"
	if errorlevel 1 goto :eof
)

set "PATH=%ProgramFiles%\Git\bin;%PATH%"
where /q git
if errorlevel 1 (
	echo Installing Git ...
	choco install git -y -r
	if errorlevel 1 goto :eof
)

if "%WSL%"=="y" goto :eof

set "PATH=%SystemDrive%\MinGW\mingw64\bin;%PATH%"
if "%GCC%"=="y" (
	where /q gcc
	if not errorlevel 1 (
		gcc -v 2>&1 | find /i "model: win32" >nul:
		if not errorlevel 1 goto :gcc
	)
	if exist %SystemDrive%\MinGW\mingw64 (
		echo GCC must be installed with win32 thread model
		goto :eof
	)

	set "PATH=%ProgramFiles%\7-Zip;%PATH%"
	where /q 7z
	if errorlevel 1 (
		echo Installing 7-Zip ...
		choco install 7zip -y -r
		if errorlevel 1 goto :eof
	)

	echo Downloading GCC ...
	del /q x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z >nul:
	curl -LJO "https://sourceforge.net/projects/mingw-w64/files/Toolchains targetting Win64/Personal Builds/mingw-builds/8.1.0/threads-win32/seh/x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z/download"
	if errorlevel 1 goto :eof

	echo Installing GCC ...
	7z x -o%SystemDrive%\MinGW x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z
	if errorlevel 1 goto :eof
	del /q x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z >nul:
)
:gcc

set "PATH=%ProgramFiles%\LLVM\bin;%PATH%"
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

set "PATH=%ProgramFiles%\MESON;%PATH%"
where /q meson
if errorlevel 1 (
	echo Installing Meson ...
	choco install meson -y -r
	if errorlevel 1 goto :eof
)

if "%WPDK%"=="y" goto :eof

set "PATH=%ProgramFiles%\NASM;%PATH%"
where /q nasm
if errorlevel 1 (
	echo Installing NASM ...
	choco install nasm -y -r
	if errorlevel 1 goto :eof
)

set "PATH=%SystemDrive%\tools\msys64;%PATH%"
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
	%MSYS2% -c "pacman --noconfirm -Syy msys/make"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "python3 --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing python ...
	%MSYS2% -c "pacman --noconfirm -Syy msys/python"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "cmp --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing diffutils ...
	%MSYS2% -c "pacman --noconfirm -Syy msys/diffutils"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "yasm --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing yasm ...
	%MSYS2% -c "pacman --noconfirm -Syy msys/yasm"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "libtool --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing libtool ...
	%MSYS2% -c "pacman --noconfirm -Syy msys/libtool"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "automake --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing automake ...
	%MSYS2% -c "pacman --noconfirm -Syy msys/automake"
)

set ver=
( for /f "tokens=*" %%f in ('%MSYS2% -c "autoconf --version 2>/dev/null"') do (set ver=%%f) ) 2>nul:
if "%ver%"=="" (
	echo Installing autoconf ...
	%MSYS2% -c "pacman --noconfirm -Syy msys/autoconf"
)
