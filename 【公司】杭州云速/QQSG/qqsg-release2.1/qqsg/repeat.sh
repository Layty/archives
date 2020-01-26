#!/bin/sh
while true
do
./qqsg  -f qqsg.conf &
sleep 1
killall qqsg
sleep 5
done
