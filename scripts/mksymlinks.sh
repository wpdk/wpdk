#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

case "$1" in
	"")
		git ls-files -s | grep '^12' | cut -f2 |
		while read -r path
		do
			if [ -h "$path" ]; then
				rm -f "$path"
				cmd.exe /c git checkout -- "$path"
				git update-index --assume-unchanged "$path"
			fi

			if [ -f "$path" ]; then
				lines=$(wc -l "$path" | cut -d\  -f1)
				
				if [ "$lines" = "0" ]; then
					dir=$(dirname "$path")
					file=$(basename "$path")
					src=$(cat "$path")
					rm -f "$path"

					( cd "$dir" && cmd.exe /c mklink /h "$file" "$(wslpath -w "$src")" )
					git update-index --assume-unchanged "$path"
				fi
			fi
		done
		;;

	rm)
		git ls-files -s | grep '^12' | cut -f2 |
		while read -r path
		do
			lines=$(wc -l "$path" 2>/dev/null | cut -d\  -f1)
			if [ ! -f "$path" ] || [ "$lines" != "0" ]; then
				rm -f "$path"
				cmd.exe /c git checkout -- "$path"
				git update-index --assume-unchanged "$path"
			fi
		done
		;;
esac

mksymlinks=$(realpath "$0")

git submodule | cut -d\  -f3 |
while read -r submodule
do
	if [ -d "$submodule" ]; then
		( cd "$submodule" && "$mksymlinks" "$@" )
	fi
done
