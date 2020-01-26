#!/bin/sh

echo $$ > /tmp/longping_qqsg.pid

while [ true ]
do
	sleep 60
	ping www.baidu.com -c 3
	wget http://www.sohu.com/ -O /dev/null 
done
