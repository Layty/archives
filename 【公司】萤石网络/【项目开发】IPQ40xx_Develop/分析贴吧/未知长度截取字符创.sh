#!/bin/sh

user="mark:x:0:0:this is a test user:/var/mark:nologin"

i=1
while true
do
        split=`echo $user|cut -d ":" -f${i}`
        if [ "$split" != "" ]
        then
                i=`expr $i + 1`
                echo $split
        else
                break
        fi
done
