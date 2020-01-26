#!/bin/sh
rm qqsg
gcc -o qqsg qqsg.c profile.c lock.c md5.c
sleep 1
./qqsg  -f /workspace/qqsg/qqsg.conf
