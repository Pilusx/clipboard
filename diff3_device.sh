#!/bin/sh

./clean.sh
./fill3_device.sh

diff -rupN get new > patch3_device.patch
