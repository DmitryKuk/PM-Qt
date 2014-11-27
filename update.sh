#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to push all changes to github

# Programs used to work
LS='/bin/ls'
GREP='grep'
STAT='stat'

GIT_ADD='git add'
GIT_COMMIT_MSG='git commit -m'
GIT_PUSH='git push'

# Checking message
if [ "X$1" == "X" ]; then
	echo -e "Error! Usage:\n\t./update.sh MESSAGE [nopush]"
	exit 1
else
	MESSAGE="$1"
fi

# Checking for "nopush" command
NOPUSH=0
if [ "X$2" != "X" ]; then
	NOPUSH=1
fi


# Search all *.o, *.pro.user, moc_* files and Makefile and ignore them using others
# Then select only regular files (not directories, links and others!) and collect
# their names in FILES_TO_UPDATE in format 'file1 file1 ...'

function update_all() {
	if [ "X$1" == "X" ]; then	# $1 is current directory
		$1='.'
	fi
	
	local FILES_TO_UPDATE=''
	for I in $( "$LS" "$1" -1 | "$GREP" -v '[[:graph:]]*\.\(o\|so\|pro\.user\|app\)\|moc_[[:graph:]]*\|Makefile' ); do
		local FILE="$1/$I"
		local TYPE="$( $STAT -f '%HT' $FILE )"
		if [ "X$TYPE" == "XRegular File" ]; then
			FILES_TO_UPDATE="$FILES_TO_UPDATE $FILE"
		elif [ "X$TYPE" == "XDirectory" ]; then
			update_all "$FILE"
		fi
	done
	
	if [ "X$FILES_TO_UPDATE" != "X" ]; then
		# Now use git to add all of files
		$GIT_ADD $FILES_TO_UPDATE
		if (( $? == 0 )); then
			echo 'Files added.'
		else
			echo 'Addind error.'
			exit 2
		fi
	fi
}

update_all '.'

$GIT_COMMIT_MSG "$MESSAGE"
if (( $? == 0 )); then
	echo 'Changes commited.'
else
	echo 'Commiting error.'
	exit 3
fi

if (( $NOPUSH == 0 )); then
	$GIT_PUSH
	if (( $? == 0 )); then
		echo 'Commits pushed.'
	else
		echo 'Pushing error.'
		exit 4
	fi
fi

echo 'Done.'
