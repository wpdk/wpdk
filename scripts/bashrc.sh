#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

wpdk_bin=$(realpath "$(dirname "${BASH_SOURCE[0]}")")

if [[ -e $wpdk_bin/wpdk_kill.sh ]]; then
	# shellcheck disable=SC2139
	alias kill="$wpdk_bin/wpdk_kill.sh"
fi

if [ -n "$MSYSTEM" ]; then
	alias sudo=
fi

alias uname='echo Windows ;:'
