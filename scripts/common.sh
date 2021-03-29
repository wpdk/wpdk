#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

# If running on Windows, adjust uname to output WSL, Msys, or Cygwin
# as the operating system and change the kernel name to Windows.
# stderr is discarded so that the xtrace logs only show a call to uname.
if [[ "$(uname -a)" = *[Mm]icrosoft* ]]; then
	uname() (command uname "$@" | sed -e's?^Linux?Windows?;s?GNU/Linux$?WSL?') 2>/dev/null
elif [[ "$(uname -o)" = Msys ]] || [[ "$(uname -o)" = Cygwin ]]; then
	uname() (MSYSTEM=CYGWIN command uname "$@" | sed -e's?^CYGWIN_NT[^ ]*?Windows?') 2> /dev/null
fi

if [[ "$(uname -s)" = Windows ]]; then
	# On Windows, TerminateProcess causes a hard stop.
	# If wpdk_kill.sh exists, use it to call the SIGTERM handler.
	if [[ -e "${CONFIG_WPDK_DIR}/bin/wpdk_kill.sh" ]]; then
		alias kill='${CONFIG_WPDK_DIR}/bin/wpdk_kill.sh'
	fi
fi

# Define functions for MSYS and Cygwin
if [[ "$(uname -o)" = Msys ]] || [[ "$(uname -o)" = Cygwin ]]; then
	sudo() { { [[ "$1" = -E ]] && shift; [[ "$1" = -u ]] && shift 2; } 2>/dev/null; "$@"; }
	alias ps=procps
fi

export CONFIG_WPDK_DIR
