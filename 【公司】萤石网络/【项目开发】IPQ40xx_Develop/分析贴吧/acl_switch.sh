#!/bin/sh

#MACFILTER=`uci get wireless.@wifi-iface[0].macfilter`
MACLIST=`uci get wireless.@wifi-iface[0].maclist`
MACNUM=`uci get wireless.@wifi-iface[0].maclist|awk '{print NF}'`

if [ -z ${MACNUM} ];then 
        MAC_IS_EMPTY=1
else    
        MAC_IS_EMPTY=0
fi

store_file=/tmp/wireless_acl.list.store
acl_list_file=/tmp/wireless_acl.list
#echo ${MACFILTER} > /tmp/wirless_acl.list


if [ -f ${store_file} ]; then
        rm ${store_file}
fi

touch ${store_file}

if [ ${MAC_IS_EMPTY} -eq 0 ];then 
        echo $MACNUM
        for i in `seq ${MACNUM}`
        do
                split=`echo $MACLIST|cut -d " " -f${i}`
                echo $i
                i=`expr $i + 1`
                echo ${split} >> ${store_file}
                echo $split
        done
fi

uci set wireless.@wifi-iface[0].maclist=""
uci set wireless.@wifi-iface[1].maclist=""

while read line
do 
    uci add_list wireless.@wifi-iface[0].maclist=${line}
    uci add_list wireless.@wifi-iface[1].maclist=${line}
done < ${acl_list_file}

cp ${store_file} ${acl_list_file}
rm ${store_file}




