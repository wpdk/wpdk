mkdir lib

cd src
cl -c -I..\include -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -D_CRT_DECLARE_NONSTDC_NAMES=0 -D_WPDK_BUILD_LIB_ *.c

lib /out:empty.lib _empty.obj
copy empty.lib ..\lib\numa.lib
copy empty.lib ..\lib\rt.lib
copy empty.lib ..\lib\uuid.lib
copy empty.lib ..\lib\pthread.lib
move empty.lib ..\lib\crypto.lib
del _empty.obj

lib /out:wpdk.lib *.obj
move wpdk.lib ..\lib\wpdk.lib
del *.obj
cd ..
