#!/bin/sh

rm -rf "get"
echo "GET 1 (PM)"
./get1_pm.sh $1
echo "GET 2 (COMPRESS)"
./get2_compress.sh $1
echo "GET 3 (DEVICE)"
./get3_device.sh $1
