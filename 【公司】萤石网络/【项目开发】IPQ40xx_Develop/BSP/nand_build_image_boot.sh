#!/bin/sh

####[    1.631265] 0x000000000000-0x000000100000 : "0:SBL1"
####[    1.638079] 0x000000100000-0x000000200000 : "0:MIBIB"
####[    1.643917] 0x000000200000-0x000000300000 : "0:BOOTCONFIG"
####[    1.650150] 0x000000300000-0x000000400000 : "0:QSEE"
####[    1.655788] 0x000000400000-0x000000500000 : "0:QSEE_1"
####[    1.661603] 0x000000500000-0x000000580000 : "0:CDT"
####[    1.666816] 0x000000580000-0x000000600000 : "0:CDT_1"
####[    1.672174] 0x000000600000-0x000000680000 : "0:BOOTCONFIG1"
####[    1.677937] 0x000000680000-0x000000700000 : "0:APPSBLENV"
####[    1.683589] 0x000000700000-0x000000900000 : "0:APPSBL"
####[    1.689999] 0x000000900000-0x000000b00000 : "0:APPSBL_1"
####[    1.696659] 0x000000b00000-0x000000b80000 : "0:ART"
####[    1.701844] 0x000000b80000-0x000004c80000 : "rootfs"
####[    1.767753] 0x000004c80000-0x000007f80000 : "rootfs_1"


#[ddr-AP-DK06.1-C1]-cdt-AP.DK06.1-C1.bin-0x8010005
#[ddr-AP-DK07.1-C1] -cdt-AP.DK07.1-C1.bin 0x8010006
#[ddr-AP-DK07.1-C2] -cdt-AP.DK07.1-C2.bin  0x8010106
#[ddr-DB-DK01.1-C1] -cdt-DB.DK01.1-C1.bin 0x1010002
#[ddr-DB.DK02.1-C1] -cdt-DB.DK02.1-C1.bin  0x1010003

#Image1
#sbl1_nor.mbn
SBL1_OFFSET=0x0
SBL1_PARTION_SIZE=0x100000
SBL1_FILE="sbl1_nand.mbn"
##SBL1_FILE="sbl1_nand.mbn"

#image2
MIB_OFFSET=0x100000
MIB_PARTION_SIZE=0x100000
MIB_FILE="nand-system-partition-ipq40xx.bin"


#image3
#[bootconfig]
BOOTCONFIG_OFFSET=0x200000
BOOTCONFIG_PARTION_SIZE=0x100000
BOOTCONFIG_FILE="nand_bootconfig.bin"

#image4
#QSEE
QSEE_OFFSET=0x300000
QSEE_PARTION_SIZE=0x100000
QSEE_FILE="tz.mbn"
#image5
QSEE_1_OFFSET=0x400000
QSEE_1_PARTION_SIZE=0x100000
QSEE_1_FILE="NULL"

#image6
#CDT
CDT_OFFSET=0x500000
CDT_PARTION_SIZE=0x80000
CDT_FILE=cdt-AP.DK04.1-C1.bin

#image7
#CDT1
CDT_1_OFFSET=0x580000
CDT_1_PARTION_SIZE=0x80000
CDT_1_FILE="NULL"

#image8
##[bootconfig1]
BOOTCONFIG_1_OFFSET=0x600000
BOOTCONFIG_1_PARTION_SIZE=0x80000
BOOTCONFIG_1_FILE="nand_bootconfig1.bin"

#image9
#padding----APPSBLENV
APPSBLENV_OFFSET=0x680000
APPSBLENV_PARTION_SIZE=0x80000
APPSBLENV_FILE="NULL"

#image10
#APPSBL
#openwrt-ipq40xx-u-boot-stripped.elf
APPSBL_OFFSET=0x700000
APPSBL_PARTION_SIZE=0x200000
APPSBL_FILE="openwrt-ipq40xx-u-boot-stripped.elf"

#image11
#padding-APPSBL_1
APPSBL_1_OFFSET=0x00900000
APPSBL_1_PARTION_SIZE=0x200000
APPSBL_1_FILE="NULL"

#image12
ART_OFFSET=0x00b00000
ART_PARTION_SIZE=0x80000
ART_FILE="NULL"
#image13
#rootfs
ROOTFS_OFFSET=0x00b80000
ROOTFS_PARTION_SIZE=0x4100000
ROOTFS_FILE="openwrt-ipq806x-ipq40xx-ubi-root.img"
#image14
ROOTFS_1_OFFSET=0x00b80000
ROOTFS_1_PARTION_SIZE=0x3300000
ROOTFS_1_FILE="NULL"

boot_file="raw_nand_flash.bin"




function make_image() {

# $1 number
# #2 partion_file
# $3 partion_size
# $
echo "image-$1-->"
################################################################################
if [ -f "$2" ]; then
	#echo "Flashing $2: Started"
	echo -e "\033[43;37m Flashing $2: Started \033[0m" 
	newsize=$(($3))
	filesize=$(stat -c "%s" $2)
	padcount=$((newsize -filesize))
	dd if=/dev/zero ibs=1 count=$padcount | tr "\000" "\377" > paddedfile.bin
	cat $SBL1_FILE paddedfile.bin > parttion_$1
	return 1
else

	if [ "$2" == "NULL" ];then
		echo "ALternate..Padding ......."
		newsize=$(($3))
		filesize=0
		padcount=$((newsize -filesize))
		dd if=/dev/zero ibs=1 count=$padcount | tr "\000" "\377" > paddedfile.bin
		cat paddedfile.bin > parttion_$1
		return 1
	else
		##echo "$2-------------------->no founded!!"
		echo -e "\033[41;37m $2-------------------->no founded!! \033[0m" 
		return 0 
	fi

fi
	rm -frv paddedfile.bin
    echo "Flashing $2: Done"
	
	return 1

}







#########################################################################
echo "Build image...."
##########################################################################

	
	make_image 1 	$SBL1_FILE  $SBL1_PARTION_SIZE
	make_image 2 	$MIB_FILE  $MIB_PARTION_SIZE
	make_image 3 	$BOOTCONFIG_FILE  $BOOTCONFIG_PARTION_SIZE
    make_image 4 	$QSEE_FILE  $QSEE_PARTION_SIZE
	make_image 5 	$QSEE_1_FILE  $QSEE_1_PARTION_SIZE
    make_image 6 	$CDT_FILE  $CDT_PARTION_SIZE
	make_image 7 	$CDT_1_FILE  $CDT_1_PARTION_SIZE

    make_image 8 	$BOOTCONFIG_1_FILE  $BOOTCONFIG_1_PARTION_SIZE
	make_image 9 	$APPSBLENV_FILE  $APPSBLENV_PARTION_SIZE
	make_image 10 	$APPSBL_FILE  $APPSBL_PARTION_SIZE
	make_image 11 	$APPSBL_1_FILE  $APPSBL_1_PARTION_SIZE
	make_image 12 	$ART_FILE  $ART_PARTION_SIZE
	make_image 13 	$ROOTFS_FILE  $ROOTFS_PARTION_SIZE

	make_image 14 	$ROOTFS_1_FILE  $ROOTFS_1_PARTION_SIZE


	echo "make finished!!!!"


echo ' --------------------------->create Flashing image ....'
cat parttion_1 parttion_2 parttion_3 parttion_4 parttion_5 parttion_6 parttion_7 parttion_8 parttion_9 parttion_10 > boot_1.bin
cat boot_1.bin parttion_11 parttion_12 parttion_13 parttion_14  > uboot.bin 

cp -frv  uboot.bin  $boot_file.bin
rm -fr parttion_*

#fi
