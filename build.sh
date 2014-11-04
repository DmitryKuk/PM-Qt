#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to run all

if (( $# != 0 && $# != 1 )); then
	echo -e "Usage:\n\t$0 [run]"
	exit 1
fi

if [ "X$1X" == "XrunX" ]; then
	qmake -Wall && make && open ../PM.app/
else
	qmake -Wall && make
fi
