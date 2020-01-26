#!/bin/sh
echo ==============001.start=====================================
cd /tmp/
echo ==============002.backup config and factory start=============
cat /dev/mtdblock2 > mtdblock2_config.bin
cat /dev/mtdblock3 > mtdblock3_factory.bin
echo ==============002.backup config and factory done=============
echo ==============003.tftp get R603WV2.2.2.2.img start=========
tftp 192.168.1.99 -gr R603W_SDV1.0.4.8.img
echo ==============003.tftp get R603WV2.2.2.2.img done=========
echo ==============004.update R603WV2.2.2.2.img start===========

cat R603W_SDV1.0.4.8.img > /dev/mtdblock0
echo ==============004.update R603WV2.2.2.2.img done===========
echo ==============005.recover config and factory start============
cat mtdblock2_config.bin  > /dev/mtdblock2
cat mtdblock3_factory.bin > /dev/mtdblock3
echo ==============005.recover config and factory done============
#echo ==============005.Modify output_pwr start============
amts w hard_device_type R603W_SD
amts w output_pwr 2 22
amts w output_pwr 3 22
amts w output_pwr 4 22
amts w output_pwr 5 22
amts w output_pwr 6 22
amts w output_pwr 7 22
amts w output_pwr  8 22
amts w output_pwr  9 22
amts w output_pwr 10 22
amts w output_pwr 11 22
amts w output_pwr 12 22
amts w output_pwr 13 22
echo ==============007.all done===================================

