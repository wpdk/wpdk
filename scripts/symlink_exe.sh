#!/usr/bin/env bash

case "$1" in
	"")
		find . -name \*.exe -print | sed -e's/\.exe$//' |
		while read x
			do ln -sf "`basename $x`.exe" "$x"
		done
		;;

	rm)
		find . -name \*.exe -print | sed -e's/\.exe$//' |
		while read x
		do rm -f "$x"
		done

		find . -type l -print |
		while read x
		do [ -f "$x" ] || rm -f "$x"
		done
		;;
esac
