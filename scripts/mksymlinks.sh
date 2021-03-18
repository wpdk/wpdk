#!/usr/bin/env bash

case "$1" in
	"")
		git ls-files -s | grep '^12' | cut -f2 |
		while read path
		do
			if [ -h "$path" ]; then
				rm -f "$path"
				cmd.exe /c git checkout -- "$path"
				git update-index --assume-unchanged "$path"
			fi

			if [ -f "$path" ]; then
				lines=`wc -l "$path" | cut -d\  -f1`
				
				if [ "$lines" = "0" ]; then
					dir=`dirname "$path"`
					file=`basename "$path"`
					src=`cat "$path"`
					rm -f "$path"

					(
						cd "$dir"
						_src=`wslpath -w "$src"`
						cmd.exe /c mklink /h "$file" "$_src"
					)
					git update-index --assume-unchanged "$path"
				fi
			fi
		done
		;;

	rm)
		git ls-files -s | grep '^12' | cut -f2 |
		while read path
		do
			lines=`wc -l "$path" 2>/dev/null | cut -d\  -f1`		
			if [ ! -f "$path" -o "$lines" != "0" ]; then
				rm -f "$path"
				cmd.exe /c git checkout -- "$path"
				git update-index --assume-unchanged "$path"
			fi
		done
		;;
esac

mksymlinks=`realpath "$0"`

git submodule | cut -d\  -f3 |
while read submodule
do
	if [ -d "$submodule" ]; then
	(
		cd "$submodule"
		"$mksymlinks" $*
	)
	fi
done
