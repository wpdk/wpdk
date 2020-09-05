mkdir lib

cd src
cl -c -Z7 -I..\include *.c

lib /out:empty.lib empty.obj
copy empty.lib ..\lib\numa.lib
copy empty.lib ..\lib\rt.lib
copy empty.lib ..\lib\uuid.lib
copy empty.lib ..\lib\pthread.lib
move empty.lib ..\lib\crypto.lib
del empty.obj

lib /out:wpdk.lib *.obj
move wpdk.lib ..\lib\wpdk.lib
del *.obj
cd ..
