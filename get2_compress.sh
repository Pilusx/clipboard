#!/bin/sh

option=$1

copy_dir()
{
	directory=$1

	mkdir -p .${directory}
	echo "("$(($# - 1))")COPYING" ${directory}

	if [ $# -eq 1 ]; then
		return
	fi

	for i in $(seq 2 $#);
	do
		file=${directory}/\$$i
		file=$(eval echo $file)

        if [ "$option" = "ECHO" ]; then
        echo $file
        elif [ "$option" = "COPY" ]; then
			if [ -f $file ]; then
				cp $file .$file
			else
				touch .$file
				echo "TOUCHED" .$file
			fi
        fi
        #copy[$index]=${!current}
    done
    #echo ${copy[*]}
}

PREFIX=get
mkdir -p ${PREFIX}
cd ${PREFIX}

short_copy_dir () {
	copy_dir ${MAIN_PATH}$@
}


MAIN_PATH=/usr

# /usr/src/include/unistd.h, który będzie kopiowany do /usr/include/unistd.h
short_copy_dir /src/include "unistd.h"

short_copy_dir /src/lib/libc/misc "copy_paste_clipboard.c" "Makefile.inc"
