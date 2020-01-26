#!/bin/sh

if [ "y$1" == "y" ];then
	echo "input device name"
	exit 1
fi

#00.upan(/dev/sdb mount check)
mount -o noatime /dev/$1 /mnt/$1/
mount_upan=`mount | grep /dev/$1`
if [ "$mount_upan" = "" ]; then
    echo "sd_factory_rwtest_failed!upan_mount_failed!"
    exit 1
fi

#01.read,write,md5sum
dd if=/dev/zero of=/mnt/$1/usb.bin bs=512 count=4000
sync
echo 3 > /proc/sys/vm/drop_caches

md5=`md5sum /mnt/$1/usb.bin |cut -d ' ' -f 1`
if [ "$md5" != "a48266248c04b2ba733238a480690a1c" ]; then
    echo "sd_factory_rwtest_failed!sdb_md5sum_failed!"
    exit 2
fi
rm -rf /mnt/$1/usb.bin

echo "sd_factory_rwtest_success!"
