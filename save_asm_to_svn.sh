#!/bin/sh

name=bw386389

dest=../${name}/zadanie7

copy_to() {
	echo "COPIES from" $1 to $2
	cp $1 ${dest}/$2
}

copy_to test/asm/compress.asm ${name}.asm
