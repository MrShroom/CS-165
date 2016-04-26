#!/bin/bash

if test "$1" == "" ;then
	echo "Incorrect number of arguments. Please provide a file name">&2 
	exit 1
fi

target="debug"
if [ "$2" == "" ]; then
	echo "Second argument is missing. Assuming debug build. If you wanted something else, please specify the makefile target"
else
	target=$2
fi

file=$1
if [ -f $file ]; then
	if [ -h "mysub.c" ]; then
		rm mysub.c
	fi
	ln -s $file mysub.c
	make $target
else
	echo "File does not exist"
fi
