#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to push all changes to github

if [ "X$1" != "X" ]; then
	make clean && git add *.h *.cpp *.sh *.pro && git commit -m "$1" && git push
else
	echo -e "Error! Usage:\n\t./update.sh MESSAGE"
fi
