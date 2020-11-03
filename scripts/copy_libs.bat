@echo off

cd %MESON_INSTALL_DESTDIR_PREFIX%

if exist lib\libcunit.a copy lib\libcunit.a lib\cunit.lib
copy lib\libcrypto.a lib\crypto.lib
copy lib\libnuma.a lib\numa.lib
copy lib\libpthread.a lib\pthread.lib
copy lib\librt.a lib\rt.lib
copy lib\libuuid.a lib\uuid.lib
