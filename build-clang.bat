@echo off

mkdir lib

cd src
clang -c -I..\include -g *.c

lib /nologo /out:empty.lib empty.o
copy empty.lib ..\lib\numa.lib >nul:
copy empty.lib ..\lib\rt.lib >nul:
copy empty.lib ..\lib\uuid.lib >nul:
copy empty.lib ..\lib\pthread.lib >nul:
move empty.lib ..\lib\crypto.lib >nul:
del empty.o

llvm-ar crDs wpdk.lib *.o
move wpdk.lib ..\lib\wpdk.lib >nul:
del *.o
cd ..
