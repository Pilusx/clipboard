#!/bin/sh

BASE=new

do_copy() {
	echo "COPIES" $1 to $2
	cp ${BASE}$1 ${BASE}$2
	
}

do_copy /usr/src/minix/include/minix/callnr.h /usr/include/minix/callnr.h
do_copy /usr/src/include/unistd.h /usr/include/unistd.h
