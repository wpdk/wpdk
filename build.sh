#!/usr/bin/env bash

CC=xgcc
ARCH=x64
TYPE=debug
CLEAN=

cfg=
[ -f build/_config ] && cfg=`cat build/_config | tr -d '\r'`

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

[ "$CLEAN" == "clean" ] && exit 0

cfg="$CC $ARCH $TYPE"

echo Building $cfg...
[ -d build ] || mkdir build
echo "$cfg" > build/_config

CONFIG_OPTS="--cross-prefix=x86_64-w64-mingw32"
[ "$TYPE" == "debug" ] && CONFIG_OPTS="$CONFIG_OPTS --enable-debug"

[ ! -f mk/config.mk ] && CC=gcc ./configure $CONFIG_OPTS
make -j8
