cd src
clang -c -I..\include getopt.c
llvm-ar crDs libwpdk.a getopt.o
move libwpdk.a ..\lib\libwpdk.a
rem cl -c -I..\include getopt.c
cd ..