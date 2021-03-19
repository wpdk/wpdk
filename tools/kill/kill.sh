#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

pid="$1"

if [ -z "$pid" ]
then
	echo "usage: wpdk_kill <pid>"
	exit 1
fi

if [ -n "$MSYSTEM" ]
then
	# Find the Windows PID for MSYS2
	_pid=`ps -p $pid -l | grep -v WINPID | cut -c27-36`
fi

if [ -z "$_pid" ]
then
	# Try and find the Windows PID
	name=`ps --no-headers -o comm= $pid`
	exe=`basename -s .exe "$name"`".exe"
	_pid=`tasklist.exe | grep "^${exe} " | cut -c27-34`
fi

if [ -z "$_pid" ]
then
	exec kill "$pid"
fi

# Terminate any Windows PIDs that match. Potentially killing multiple
# processes is not ideal but WSL doesn't identify the mapping. The
# script is intended for use during testing rather than production.

for i in $_pid
do
	PATH=`dirname "$0"`:$PATH
	wpdk_terminate.exe $i || kill $pid
done

exit 0
