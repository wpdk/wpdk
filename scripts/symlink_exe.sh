#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

case "$1" in
	"")
		find . -name \*.exe -print | sed -e's/\.exe$//' |
		while read -r x
			do ln -sf "$(basename "$x").exe" "$x"
		done
		;;

	rm)
		find . -name \*.exe -print | sed -e's/\.exe$//' |
		while read -r x
		do rm -f "$x"
		done

		find . -type l -print |
		while read -r x
		do [ -f "$x" ] || rm -f "$x"
		done
		;;
esac
