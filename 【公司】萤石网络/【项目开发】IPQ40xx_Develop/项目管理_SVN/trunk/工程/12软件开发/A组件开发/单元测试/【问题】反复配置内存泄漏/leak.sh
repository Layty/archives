#!/bin/sh

ps -w

uci show  | grep skb_recycler
uci  set skb_recycler.@skb[0].max_skbs=0
uci  set  skb_recycler.@skb[0].max_spare_skbs=0
uci  commit
uci show  | grep skb_recycler    


i=0
while [ true ]
do
	echo "~~~~~~~~~~ i=$i ~~~~~~~~~~~~~~~"
        if [ $i -gt 100 ];then
                echo "exit exit exit exit exit exit"
                exit 0
        fi
        /etc/init.d/network reload &
        sleep 30
	echo $i
        i=`expr $i + 1`;

	free
	echo 3 > /proc/sys/vm/drop_caches
	free
done

==============================================================================================
#!/bin/sh

rmmod qca_ol
rmmod smart_antenna
rmmod ath_pktlog
rmmod qca-mcs
rmmod ath_spectral
rmmod ath_dfs
rmmod umac
rmmod asf
rmmod qdf
rmmod mem_manager

================================================================================================
#!/bin/sh

insmod mem_manager
insmod qdf
insmod asf
insmod umac
insmod ath_dfs
insmod ath_spectral
insmod qca-mcs
insmod ath_pktlog
insmod smart_antenna
insmod qca_ol
