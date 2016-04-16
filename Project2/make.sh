#!/bin/bash

if test "$1" == "" ;then
	echo "Incorrect number of arguments. Please provide a file name">&2 
	exit 1
fi

file=$1
if [ -f $file ]; then
	if [ -h "mysub.c" ]; then
		rm mysub.c
	fi
	ln -s $file mysub.c
	ls
	make debug
else
	echo "File does not exist"
fi
# create symbolic link for the compiling file
