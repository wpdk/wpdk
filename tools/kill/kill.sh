#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

find_windows_pid() {
	case "$(uname -a)" in
		# Windows Subsystem for Linux (WSL)
		Linux*[Mm]icrosoft*)
			# Potentially returns multiple matches since WSL doesn't
			# identify a unique mapping. This is unlikely to cause any
			# issues since the script is intended for use during testing
			# rather than production.
			name=$(basename -s .exe "$(ps --no-headers -o comm= "$1")")
			tasklist.exe | grep "^${name}.exe " | cut -c27-34
			;;
		
		# Cygwin based including MSYS and MinGW
		*Msys | *Cygwin)
			ps -p "$1" -l | cut -c27-36 | grep -v WINPID
			;;
	esac
}

if [[ $# -ge 1 ]] && [[ "$*" != *-[lL]* ]] && [[ "${*: -1}" != -* ]]; then
	pid=$(find_windows_pid "${*: -1}")

	if [[ -n ${pid} ]]; then
		# shellcheck disable=SC2086
		exec "$(dirname "$0")/wpdk_kill.exe" "${@:1:$#-1}" $pid
	fi
fi

exec kill "$@"
