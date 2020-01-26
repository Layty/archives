#!/bin/sh

#00.upan(/dev/sdb mount check)
mount -o noatime /dev/sdb1 /mnt/sdb/
mount -o noatime /dev/sdb  /mnt/sdb/
mount_upan=`mount | grep /dev/sdb`
if [ "$mount_upan" = "" ]; then
    echo "sd_factory_rwtest_failed!upan_mount_failed!"
    exit 1
fi

#01.read,write,md5sum
dd if=/dev/zero of=/mnt/sdb/usb.bin bs=512 count=4000
sync
echo 3 > /proc/sys/vm/drop_caches

md5_sdb=`md5sum /mnt/sdb/usb.bin |cut -d ' ' -f 1`
if [ "$md5_sdb" != "a48266248c04b2ba733238a480690a1c" ]; then
    echo "sd_factory_rwtest_failed!sdb_md5sum_failed!"
    exit 2
fi
rm -rf /mnt/sdb/usb.bin

echo "sd_factory_rwtest_success!"
