@echo off

mkdir lib

cd src
cl /nologo -c -Z7 -I..\include *.c

lib /nologo /out:empty.lib empty.obj
copy empty.lib ..\lib\numa.lib >nul:
copy empty.lib ..\lib\rt.lib >nul:
copy empty.lib ..\lib\uuid.lib >nul:
copy empty.lib ..\lib\pthread.lib >nul:
move empty.lib ..\lib\crypto.lib >nul:
del empty.obj

lib /nologo /out:wpdk.lib *.obj
move wpdk.lib ..\lib\wpdk.lib >nul:
del *.obj
cd ..
