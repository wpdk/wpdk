mkdir lib

cd src
cl -c -I..\include -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS -D_CRT_DECLARE_NONSTDC_NAMES=0 getopt.c
lib /out:wpdk.lib getopt.obj
move wpdk.lib ..\lib\wpdk.lib
del getopt.obj

cl -c _empty.c
lib /out:empty.lib _empty.obj
copy empty.lib ..\lib\numa.lib
copy empty.lib ..\lib\rt.lib
copy empty.lib ..\lib\uuid.lib
copy empty.lib ..\lib\pthread.lib
move empty.lib ..\lib\crypto.lib
del _empty.obj
cd ..