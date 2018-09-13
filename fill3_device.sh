#!/bin/sh

BASE=new

do_copy() {
	echo "COPIES" $1 to $2
	cp ${BASE}$1 ${BASE}$2

}

do_copy /usr/src/minix/include/minix/dmap.h /usr/include/minix/dmap.h
do_copy /usr/src/minix/drivers/examples/clipboard/clipboard_protocol.h /usr/src/lib/libc/misc/clipboard_protocol.h
