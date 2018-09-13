#!/bin/sh
cp *.patch /.
cd /
#patch -p1 < patch1_pm.patch
#patch -p1 < patch3_device.patch
patch -p1 < patch_all.patch
