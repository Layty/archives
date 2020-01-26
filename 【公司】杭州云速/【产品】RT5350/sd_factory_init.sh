#!/bin/sh

#00.umount all partitions
sdalist=$(mount |grep /dev/sda |cut -d ' ' -f 1 )
for partitions in $sdalist
do
    umount $partitions
done

#01.del all partitions on /dev/sda
echo -e "d\n1\nd\n2\nd\n3\nd\n4\nw\n" > /etc/delpart
#cat /etc/delpart
fdisk /dev/sda < /etc/delpart 1>/dev/null 2>&1

#02.create a primary partitions(/dev/sda1) on /dev/sda
echo -e "n\np\n1\n\n+512M\nt\nb\nw\n" > /etc/primarypart
#cat /etc/primarypart
fdisk /dev/sda < /etc/primarypart 1>/dev/null 2>&1

#03.create a extended partitions(/dev/sda2) on /dev/sda
echo -e "n\ne\n2\n\n\nw\n" > /etc/extpart
#cat /etc/extpart
fdisk /dev/sda < /etc/extpart 1>/dev/null 2>&1

#04.create 3 logic partitions(/dev/sda5,sda6,sda7) on /dev/sda2
echo -e "n\nl\n\n+1024M\nt\n5\nb"   > /etc/parted
echo -e "n\nl\n\n+14336M\nt\n6\nb" >> /etc/parted
echo -e "n\nl\n\n\nt\n7\nb"        >> /etc/parted
echo -e "w\n" >> /etc/parted
#cat /etc/parted
fdisk /dev/sda < /etc/parted  1>/dev/null 2>&1

#05.format all partitions to FAT32
sda1_size=$(cat /proc/partitions  |grep sda1 |awk '{print $3}')
sda5_size=$(cat /proc/partitions  |grep sda5 |awk '{print $3}')
#sda6_size=$(cat /proc/partitions  |grep sda6 |awk '{print $3}')
#sda7_size=$(cat /proc/partitions  |grep sda7 |awk '{print $3}')
mkdosfs -F 32 /dev/sda1 $sda1_size
mkdosfs -F 32 /dev/sda5 $sda5_size
#mkdosfs -F 32 /dev/sda6 $sda6_size
#mkdosfs -F 32 /dev/sda7 $sda7_size

#06.mount all partitions to /mnt/ dir
mount -o noatime /dev/sda1  /mnt/boot/
mount -o noatime /dev/sda5  /mnt/log/
#mount -o noatime /dev/sda6  /mnt/usr_data/
#mount -o noatime /dev/sda7  /mnt/cvn_root/

#07.print "all done" to meet AMTS.exe's requirement,so don't modified the following lines.
mount_sda1=`mount | grep /dev/sda1`
mount_sda5=`mount | grep /dev/sda5`
#mount_sda6=`mount | grep /dev/sda6`
#mount_sda7=`mount | grep /dev/sda7`

if [ "$mount_sda1" = "" ]; then
    echo "sd_factory_init_failed!sda1_mount_failed!"
    exit 1 
elif [ "$mount_sda5" = "" ]; then
    echo "sd_factory_init_failed!sda5_mount_failed!"
    exit 1
#elif [ "$mount_sda6" = "" ]; then
#    echo "sd_factory_init_failed!sda6_mount_failed!"
#    exit 1
#elif [ "$mount_sda7" = "" ]; then
#    echo "sd_factory_init_failed!sda7_mount_failed!"
#    exit 1
fi

echo "fmt fat32" > /mnt/boot/sda1_fmt.info
echo "fmt fat32" > /mnt/boot/sda5_fmt.info

echo "sd_factory_init_success!"
