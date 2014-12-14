#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to push all changes to github

# Programs used to work
LS_CMD='/bin/ls'
GREP_CMD='grep'
STAT_CMD='stat'
FILE_CMD='file'

GIT_ADD='git add'
GIT_COMMIT_MSG='git commit -m'
GIT_PUSH='git push'

# Colors to display messages
COLOR_OK='\033[32m'
COLOR_NOTE='\033[34m'
COLOR_ERROR='\033[31m'
COLOR_RESET='\033[0m'

# Checking message
if [ "X$1" == "X" ]; then
	echo -e "${COLOR_ERROR}Error!$COLOR_RESET Usage:\n\t./update.sh MESSAGE [nopush]"
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
	local CURRENT_DIR='.'
	if [ "X$1" != "X" ]; then	# $1 is current directory
		CURRENT_DIR="$1"
	fi
	
	local FILES_TO_UPDATE=''
	for I in $( "$LS_CMD" -1 "$CURRENT_DIR" | "$GREP_CMD" -v '[[:graph:]]*\.\(o\|so\|pro\.user\|app\)\|moc_[[:graph:]]*\|Makefile' ); do
		local FILE="$CURRENT_DIR/$I"
		local TYPE="$( $STAT_CMD -f '%HT' $FILE )"
		if [ "X$TYPE" == "XRegular File" ] || [ "X$TYPE" == "XSymbolic Link" ]; then
			"$FILE_CMD" --mime-type "$FILE" | "$GREP_CMD" "text/"
			if [ "X$?" != "X0" ]; then
				echo -e "${COLOR_NOTE}File \"$FILE\" seems like not text. It will not be autocommited.$COLOR_RESET"
			else
				FILES_TO_UPDATE="$FILES_TO_UPDATE $FILE"
			fi
		elif [ "X$TYPE" == "XDirectory" ]; then
			update_all "$FILE"
		fi
	done
	
	if [ "X$FILES_TO_UPDATE" != "X" ]; then
		# Now use git to add all of files
		$GIT_ADD $FILES_TO_UPDATE
		if (( $? == 0 )); then
			echo -e "${COLOR_OK}Files from \"$CURRENT_DIR/\" added.$COLOR_RESET"
		else
			echo -e "${COLOR_ERROR}Error adding files from \"$CURRENT_DIR\".$COLOR_RESET"
			exit 2
		fi
	fi
}

update_all '.'

$GIT_COMMIT_MSG "$MESSAGE"
if (( $? == 0 )); then
	echo -e "${COLOR_OK}Changes commited with message \"$MESSAGE\".$COLOR_RESET"
else
	echo -e "${COLOR_ERROR}Commiting error.$COLOR_RESET"
	exit 3
fi

if (( $NOPUSH == 0 )); then
	$GIT_PUSH
	if (( $? == 0 )); then
		echo -e "${COLOR_OK}Commits pushed.$COLOR_RESET"
	else
		echo -e "${COLOR_ERROR}Pushing error.$COLOR_RESET"
		exit 4
	fi
fi

echo -e "${COLOR_OK}Done.$COLOR_RESET"
