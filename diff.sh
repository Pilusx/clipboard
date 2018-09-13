#!/bin/sh

./clean.sh
./fill1_pm.sh
./fill3_device.sh

diff -rupN get new > patch_all.patch
