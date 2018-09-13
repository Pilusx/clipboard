#!/bin/sh

./prepare_dev.sh
cd /usr/src/minix/drivers/examples/clipboard
make clean; make; make install
service up /service/clipboard -dev /dev/clipboard
