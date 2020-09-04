mkdir lib

cd src
clang -c -I..\include -D_WPDK_BUILD_LIB_ -g *.c

lib /out:empty.lib empty.o
copy empty.lib ..\lib\numa.lib
copy empty.lib ..\lib\rt.lib
copy empty.lib ..\lib\uuid.lib
copy empty.lib ..\lib\pthread.lib
move empty.lib ..\lib\crypto.lib
del empty.o

llvm-ar crDs wpdk.lib *.o
move wpdk.lib ..\lib\wpdk.lib
del *.o
cd ..
