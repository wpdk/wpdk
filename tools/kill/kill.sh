#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

unalias kill 2>/dev/null

pid=
sig=
sigflag=
fail=

for i in "$@"
do
	if [ -n "$sigflag" ]
	then
		sig="-$i"
		sigflag=
		continue
	fi

	case "$i" in
	-s)
		sigflag=true
		;;

	-[0-9]|-[0-2][0-9]|-[A-Z]*)
		sig=$i
		;;

	[0-9]*)
		[ -n "$pid" ] && fail=true
		pid="$i"
		;;

	*)
		fail=true
		break
		;;
	esac
done

if [ -z "$pid" ] || [ -n "$fail" ]
then
	echo "usage: wpdk_kill.sh [options] <pid>"
	exit 1
fi

if [ -n "$MSYSTEM" ]
then
	# Find the Windows PID for MSYS2
	# shellcheck disable=SC2009
	_pid=$(ps -p "$pid" -l | grep -v WINPID | cut -c27-36)
fi

if [ -z "$_pid" ]
then
	# Try and find the Windows PID
	name=$(ps --no-headers -o comm= "$pid")
	exe=$(basename -s .exe "$name")".exe"
	_pid=$(tasklist.exe | grep "^${exe} " | cut -c27-34)
fi

[ -z "$_pid" ] && exec kill "$@"

# Terminate any Windows PIDs that match. Potentially killing multiple
# processes is not ideal but WSL doesn't identify the mapping. The
# script is intended for use during testing rather than production.

exec "$(dirname "$0")/wpdk_kill.exe" "$sig" "$_pid"
