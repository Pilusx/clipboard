#!/bin/sh

# Serwer PM
cd /usr/src/minix/servers/pm
make -k; make -k install

# Funkcja biblioteczna
cd /usr/src/lib/libc
make -k; make -k install


