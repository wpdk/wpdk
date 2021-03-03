#!/usr/bin/env bash

CC=xgcc
ARCH=x64
TYPE=debug
CLEAN=

WPDK=
DPDK=
SPDK=
[ -d inc ] && WPDK=y
[ -d drivers ] && DPDK=y
[ -d dpdkbuild ] && SPDK=y

DESTDIR=
[ "$SPDK" != "y" ] && DESTDIR=`pwd`/build

CONFDIR=build
[ "$SPDK" != "y" -a -d ../dpdkbuild ] && CONFDIR=../build

cfg=
[ -f $CONFDIR/_config ] && cfg=`cat $CONFDIR/_config | tr -d '\r'`

for i in $cfg $*
do
	case $i in
	debug)
		TYPE=debug
		;;
	release)
		TYPE=release
		;;
	clean)
		CLEAN=clean
		;;
	rebuild)
		CLEAN=y
		;;
	esac
done

if [ "$CLEAN" != "clean" ]
then
	[ ! -f mk/config.mk ] && CLEAN=y
	if [ "$cfg" != "$CC $ARCH $TYPE" ]
	then
		echo Config changed from "$cfg" to "$CC $ARCH $TYPE"
		CLEAN=y
	fi
fi

if [ -n "$CLEAN" ]
then
	echo Cleaning...
	for i in . dpdp wpdk
	do
		rm -rf $i/build $i/build-tmp
	done
	rm -f mk/config.mk
	find . -type f -name \*.d -print | xargs rm -f
fi

[ "$CLEAN" = "clean" ] && exit 0

cfg="$CC $ARCH $TYPE"

echo Building $cfg...

[ -d $CONFDIR ] || mkdir $CONFDIR
echo "$cfg" > $CONFDIR/_config

if [ "$WPDK$DPDK" = "y" ]
then
	MESON_OPTS=
	[ "$DPDK" = "y" ] && MESON_OPTS="-Dexamples=helloworld"
	[ -d build-tmp ] || meson --buildtype=$TYPE $MESON_OPTS --prefix='/' --cross-file=./config/x86_64-w64-mingw32 build-tmp
	ninja -C build-tmp -j8 && DESTDIR="$DESTDIR" meson install -C build-tmp --no-rebuild --only-changed
fi

if [ "$SPDK" = "y" ]
then
	CONFIG_OPTS="--cross-prefix=x86_64-w64-mingw32"
	[ "$TYPE" == "debug" ] && CONFIG_OPTS="$CONFIG_OPTS --enable-debug"
	[ ! -f mk/config.mk ] && CC=gcc ./configure $CONFIG_OPTS
	make -j8
fi
