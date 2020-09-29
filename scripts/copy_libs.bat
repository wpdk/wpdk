@echo off

copy %DESTDIR%\lib\libcunit.a %DESTDIR%\lib\cunit.lib >nul:

copy %DESTDIR%\lib\libcrypto.a %DESTDIR%\lib\crypto.lib >nul:
copy %DESTDIR%\lib\libnuma.a %DESTDIR%\lib\numa.lib >nul:
copy %DESTDIR%\lib\libpthread.a %DESTDIR%\lib\pthread.lib >nul:
copy %DESTDIR%\lib\librt.a %DESTDIR%\lib\rt.lib >nul:
copy %DESTDIR%\lib\libuuid.a %DESTDIR%\lib\uuid.lib >nul:
rem copy %DESTDIR%\lib\libwpdk.a %DESTDIR%\lib\wpdk.lib >nul:
