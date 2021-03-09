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

WPDKDIR=`dirname "$0"`

DESTDIR=
[ "$SPDK" != "y" ] && DESTDIR=`pwd`/build

CONFDIR=build
if [ "$SPDK" != "y" ]
then
	if [ -d ../dpdkbuild ]
	then CONFDIR=../build
	elif [ -d ../spdk/dpdkbuild ]
	then CONFDIR=../spdk/build
	fi
fi

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
	[ "$SPDK" = "y" -a ! -f mk/config.mk ] && CLEAN=y
	if [ "$cfg" != "$CC $ARCH $TYPE" ]
	then
		echo Config changed from "$cfg" to "$CC $ARCH $TYPE"
		CLEAN=y
	fi
fi

if [ -n "$CLEAN" ]
then
	echo Cleaning...
	for i in . dpdp wpdk "$WPDKDIR"
	do rm -rf $i/build $i/build-tmp
	done
	rm -f mk/config.mk
	find . -type f -name \*.d -print | xargs rm -f
	"$WPDKDIR/scripts/symlink_exe.sh" rm
	(
		cd "$WPDKDIR"
		find . -type f -name \*.d -print | xargs rm -f
		./scripts/symlink_exe.sh rm
	)
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
	"$WPDKDIR/scripts/symlink_exe.sh"
fi

if [ "$SPDK" = "y" ]
then
	CONFIG_OPTS="--cross-prefix=x86_64-w64-mingw32"
	[ "$TYPE" == "debug" ] && CONFIG_OPTS="$CONFIG_OPTS --enable-debug"
	
	if [ ! -d wpdk -a -d "$WPDKDIR" ]
	then
		(cd "$WPDKDIR"; ./build.sh )
		CONFIG_OPTS="$CONFIG_OPTS --with-wpdk=$WPDKDIR/build"
	fi

	[ ! -f mk/config.mk ] && CC=gcc ./configure $CONFIG_OPTS
	make -j8
	"$WPDKDIR/scripts/symlink_exe.sh"
fi
