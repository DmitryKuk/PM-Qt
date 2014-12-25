#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to run all

if (( $# != 0 && $# != 1 )); then
	echo -e "Usage:\n\t$0 [run]"
	exit 1
fi

qmake -Wall && make
STATUS="$?"

if [ "X${STATUS}X" == "X0X" ] && [ "X$1X" == "XrunX" ]; then
	TARGET="$( uname )"
	case "$( uname )" in
		"Darwin") open PM.app/ ;;
		"Linux") ./PM ;;
		*) echo -e "Sorry, I don\'t know how to build it for $TARGET.\nSee \"build.sh\" for more information."
	esac
fi
