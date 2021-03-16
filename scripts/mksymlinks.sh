#!/usr/bin/env bash

case "$1" in
	"")
		git ls-files -s | grep '^12' | cut -f2 |
		while read path
		do
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
				git update-index --no-assume-unchanged "$path"
			fi
		done
		;;
esac
