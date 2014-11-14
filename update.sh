#!/bin/bash

# Author: Dmitry Kukovinets (d1021976@gmail.com)

# Simple script to push all changes to github

# Programs used to work
LS='/bin/ls'
GREP='grep'

GIT_ADD='git add'
GIT_COMMIT_MSG='git commit -m'
GIT_PUSH='git push'

if [ "X$1" == "X" ]; then
	echo -e "Error! Usage:\n\t./update.sh MESSAGE"
	exit 1
else
	# Adding message to commit
	GIT_COMMIT_MSG="$( echo -en $GIT_COMMIT_MSG \'$1\' )"
	echo -e "\"$GIT_COMMIT_MSG\""
fi


# Search all *.o, *.pro.user, moc_* files and Makefile and ignore them using others
# Then select only regular files (not directories, links and others!) and collect
# their names in FILES_TO_UPDATE in format 'file1 file1 ...'

FILES_TO_UPDATE=''
for I in $( $LS -1 | $GREP -v '[[:graph:]]*\.\(o\|so\|pro\.user\|app\)\|moc_[[:graph:]]*\|Makefile' ); do
	if [ "X$( stat -f '%HT' $I )" == "XRegular File" ]; then
		FILES_TO_UPDATE="$FILES_TO_UPDATE $I"
	fi
done

echo $FILES_TO_UPDATE

# Now use git to add all of them, commit with given messsage and push
$GIT_ADD $FILES_TO_UPDATE && $GIT_COMMIT_MSG && $GIT_PUSH
