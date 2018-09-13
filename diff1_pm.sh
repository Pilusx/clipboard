#!/bin/sh

./clean.sh
./fill1_pm.sh

diff -rupN get new > patch1_pm.patch
