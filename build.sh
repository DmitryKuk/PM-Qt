#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to build and run all

if (( $# != 0 && $# != 1 )); then
	echo -e "Usage:\n\t$0 [run]"
	exit 1
fi

# Building cryptokernel
cd cryptokernel
STATUS="$?"
if [ "X$STATUS" != "X0" ]; then
	echo "Build failed: can't find cryptokernel directory."
	exit "$STATUS"
fi

make
STATUS="$?"
if [ "X$STATUS" != "X0" ]; then
	echo "Build failed: can't build cryptokernel."
	exit "$STATUS"
fi

cd ../

# Building interface
qmake -Wall && make
STATUS="$?"
if [ "X$STATUS" != "X0" ]; then
	echo "Build failed: can't build interface."
	exit "$STATUS"
fi

if [ "X$1" == "Xrun" ]; then
	if [ "X$STATUS" == "X0" ]; then
		TARGET_SYSTEM="$( uname )"
		case "$( uname )" in
			"Darwin") open PM.app/ ;;
			"Linux") ./PM ;;
			"Windows") echo -e "It's a joke I hope: uname says this is Windows OS.\nI can\'t run output file." ;;
			*) echo -e "Sorry, I don't know how to build it for target system \"$TARGET_SYSTEM\".\nSee \"build.sh\" for more information."
		esac
	else
		echo "Build failed, it's impossible to run output file."
		exit "$STATUS"
	fi
fi
