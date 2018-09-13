#!/bin/sh

name=bw386389

from=new
dest=../${name}/zadanie7

copy_to() {
	echo "COPIES from" $1 to $2
	cp $1 ${dest}/$2
}


copy_to README README
copy_to ${from}/usr/src/minix/drivers/examples/clipboard/${name}.c ${name}.c
copy_to ${from}/usr/src/minix/drivers/examples/clipboard/Makefile Makefile
copy_to patch_all.patch ${name}.patch
