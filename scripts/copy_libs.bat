@echo off

cd %MESON_INSTALL_DESTDIR_PREFIX%

if exist lib\libcunit.a copy lib\libcunit.a lib\cunit.lib > nul:
copy lib\libcrypto.a lib\crypto.lib > nul:
copy lib\libnuma.a lib\numa.lib > nul:
copy lib\libpthread.a lib\pthread.lib > nul:
copy lib\librt.a lib\rt.lib > nul:
copy lib\libssl.a lib\ssl.lib > nul:
copy lib\libuuid.a lib\uuid.lib > nul:
