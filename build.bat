@echo off
@setlocal enableextensions

if "%VCINSTALLDIR%"=="" (
	echo "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
	goto :eof
)

rmdir /s /q build
rmdir /s /q build-tmp

set CC=cl
set DESTDIR=%CD%\build

meson --buildtype=debug build-tmp
ninja -C build-tmp install

goto :eof

rem **********************************
rem old script for cl
rem **********************************

mkdir build\lib

cd src
cl /nologo -c -Z7 -I..\include *.c

lib /nologo /out:empty.lib empty.obj
copy empty.lib ..\build\lib\numa.lib >nul:
copy empty.lib ..\build\lib\rt.lib >nul:
copy empty.lib ..\build\lib\uuid.lib >nul:
copy empty.lib ..\build\lib\pthread.lib >nul:
move empty.lib ..\build\lib\crypto.lib >nul:
del empty.obj

lib /nologo /out:wpdk.lib *.obj
move wpdk.lib ..\build\lib\wpdk.lib >nul:
del *.obj
cd ..

robocopy include build\include /MIR /NDL /NP /R:5 /W:5 /NJH /NJS /NFL

rem **********************************
rem old script for clang
rem **********************************

mkdir build\lib

cd src
clang -c -I..\include -g *.c

lib /nologo /out:empty.lib empty.o
copy empty.lib ..\build\lib\numa.lib >nul:
copy empty.lib ..\build\lib\rt.lib >nul:
copy empty.lib ..\build\lib\uuid.lib >nul:
copy empty.lib ..\build\lib\pthread.lib >nul:
move empty.lib ..\build\lib\crypto.lib >nul:
del empty.o

llvm-ar crDs wpdk.lib *.o
move wpdk.lib ..\build\lib\wpdk.lib >nul:
del *.o
cd ..

robocopy include build\include /MIR /NDL /NP /R:5 /W:5 /NJH /NJS /NFL
