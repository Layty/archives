========================= 32M Flash ==============================================
Format: Log Type - Time(microsec) - Message - Optional Info
Log Type: B - Since Boot(Power On Reset),  D - Delta,  S - Statistic
S - QC_IMAGE_VERSION_STRING=BOOT.BF.3.1.1-00117
S - IMAGE_VARIANT_STRING=DAABANAZA
S - OEM_IMAGE_VERSION_STRING=CRM
S - Boot Config, 0x00000020
S - Reset status Config, 0x00000010
S - Core 0 Frequency, 0 MHz
B -       261 - PBL, Start
B -      1338 - bootable_media_detect_entry, Start
B -      1678 - bootable_media_detect_success, Start
B -      1692 - elf_loader_entry, Start
B -      5069 - auth_hash_seg_entry, Start
B -      7211 - auth_hash_seg_exit, Start
B -    578930 - elf_segs_hash_verify_entry, Start
B -    696731 - PBL, End
B -    696755 - SBL1, Start
B -    787479 - pm_device_init, Start
D -         7 - pm_device_init, Delta
B -    789005 - boot_flash_init, Start
D -     54260 - boot_flash_init, Delta
B -    847410 - boot_config_data_table_init, Start
D -      3838 - boot_config_data_table_init, Delta - (419 Bytes)
B -    854619 - clock_init, Start
D -      7834 - clock_init, Delta
B -    866950 - CDT version:2,Platform ID:8,Major ID:1,Minor ID:0,Subtype:0
B -    870363 - sbl1_ddr_set_params, Start
B -    875460 - cpr_init, Start
D -         2 - cpr_init, Delta
B -    879842 - Pre_DDR_clock_init, Start
D -         5 - Pre_DDR_clock_init, Delta
D -     13176 - sbl1_ddr_set_params, Delta
B -    893579 - pm_driver_init, Start
D -         2 - pm_driver_init, Delta
B -    964059 - sbl1_wait_for_ddr_training, Start
D -        28 - sbl1_wait_for_ddr_training, Delta
B -    979533 - Image Load, Start
D -    140802 - QSEE Image Loaded, Delta - (274048 Bytes)
B -   1120762 - Image Load, Start
D -      1437 - SEC Image Loaded, Delta - (2048 Bytes)
B -   1131164 - Image Load, Start
D -    209721 - APPSBL Image Loaded, Delta - (428763 Bytes)
B -   1341280 - QSEE Execution, Start
D -        60 - QSEE Execution, Delta
B -   1347493 - SBL1, End
D -    652816 - SBL1, Delta
S - Flash Throughput, 2004 KB/s  (705278 Bytes,  351778 us)
S - DDR Frequency, 537 MHz


U-Boot 2012.07 [Chaos Calmer 15.05.1,r35193] (May 25 2017 - 21:10:08)

smem ram ptable found: ver: 1 len: 3
DRAM:  256 MiB
machid : 0x8010000
NAND:  SF: Detected MX25L25635E with page size 4 KiB, total 32 MiB
ipq_spi: page_size: 0x100, sector_size: 0x1000, size: 0x2000000
32 MiB
MMC:   
In:    serial
Out:   serial
Err:   serial
machid: 8010000
flash_type: 0
Hit any key to stop autoboot:  0 
SF: Detected MX25L25635E with page size 4 KiB, total 32 MiB
## Booting kernel from FIT Image at 84000000 ...
   Using 'config@ap.dk01.1-c1' configuration
   Trying 'kernel@1' kernel subimage
     Description:  ARM OpenWrt Linux-3.14.77
     Type:         Kernel Image
     Compression:  gzip compressed
     Data Start:   0x840000e4
     Data Size:    3346645 Bytes = 3.2 MiB
     Architecture: ARM
     OS:           Linux
     Load Address: 0x80208000
     Entry Point:  0x80208000
     Hash algo:    crc32
     Hash value:   ba1b2f5b
     Hash algo:    sha1
     Hash value:   d388f1d7b66bc6385ca127d75510358607bf0983
   Verifying Hash Integrity ... crc32+ sha1+ OK
## Flattened Device Tree from FIT Image at 84000000
   Using 'config@ap.dk01.1-c1' configuration
   Trying 'fdt@ap.dk01.1-c1' FDT blob subimage
     Description:  ARM OpenWrt qcom-ipq40xx-ap.dkxx device tree blob
     Type:         Flat Device Tree
     Compression:  uncompressed
     Data Start:   0x84339b28
     Data Size:    34422 Bytes = 33.6 KiB
     Architecture: ARM
     Hash algo:    crc32
     Hash value:   2843f057
     Hash algo:    sha1
     Hash value:   2318e2776ac672e073ca1e4d9ed774b409664885
   Verifying Hash Integrity ... crc32+ sha1+ OK
   Booting using the fdt blob at 0x84339b28
   Uncompressing Kernel Image ... OK
   Loading Device Tree to 86ff4000, end 86fff675 ... OK
eth0 MAC Address from ART is not valid
eth1 MAC Address from ART is not valid
Using machid 0x8010000 from environment

root@OpenWrt:/# cat /proc/mtd 
dev:    size   erasesize  name
mtd0: 00040000 00010000 "0:SBL1"
mtd1: 00020000 00010000 "0:MIBIB"
mtd2: 00060000 00010000 "0:QSEE"
mtd3: 00010000 00010000 "0:CDT"
mtd4: 00010000 00010000 "0:DDRPARAMS"
mtd5: 00010000 00010000 "0:APPSBLENV"
mtd6: 00080000 00010000 "0:APPSBL"
mtd7: 00010000 00010000 "0:ART"
mtd8: 00400000 00010000 "0:HLOS"
mtd9: 01600000 00010000 "rootfs"
mtd10: 00780000 00010000 "rootfs_data"
root@OpenWrt:/# 
root@OpenWrt:/# 
root@OpenWrt:/# cat /proc/pa
pagetypeinfo  partitions
root@OpenWrt:/# cat /proc/partitions 
major minor  #blocks  name

  31        0        256 mtdblock0
  31        1        128 mtdblock1
  31        2        384 mtdblock2
  31        3         64 mtdblock3
  31        4         64 mtdblock4
  31        5         64 mtdblock5
  31        6        512 mtdblock6
  31        7         64 mtdblock7
  31        8       4096 mtdblock8
  31        9      22528 mtdblock9
  31       10       7680 mtdblock10
  
  
  
  
  
  
 ========================== 16M ====================
 [AP.DK01.1-S1]
dirname=AP.DK01.1-S1
nand_available=false
nor_available=true
emmc_available=false
spi_nand_available=false
norplusnand_available=false
norplusemmc_available=false
nand_pagesize=2048
nand_pages_per_block=64
nand_total_blocks=4096
nor_pagesize=256
nor_pages_per_block=256
nor_total_blocks=256
nor_partition=nor-partition-standard.xml
bootconfig=bootconfig.xml
nand_bootconfig=nand_bootconfig.bin
nor_bootconfig=nor_bootconfig.bin
bootconfig1=bootconfig1.xml
nand_bootconfig1=nand_bootconfig1.bin
nor_bootconfig1=nor_bootconfig1.bin
smem_info=["smem-AP.DK01.1-S1.xml"]
cdt_info=["pcddr_AP.DK01.1-S1_64M16.xml"]
cdt_info_default=pcddr_AP.DK01.1-S1.xml
nor_partition_mbn=nor-system-partition-ipq40xx-s.bin
nor_flash_conf=nor-flash.conf
machid=0x8010200


================== debug ==================
Destroy Hash Table: 8736dee4 table = (null)
Create Hash Table: N=72
INSERT: table 8736dee4, filled 1/73 rv 870700f0 ==> name="bootcmd" value="bootipq"
INSERT: table 8736dee4, filled 2/73 rv 87070180 ==> name="bootdelay" value="2"
INSERT: table 8736dee4, filled 3/73 rv 870703cc ==> name="baudrate" value="115200"
INSERT: table 8736dee4, filled 4/73 rv 870703fc ==> name="ipaddr" value="192.168.1.11"
INSERT: free(data = 87070068)
INSERT: done
In:    serial
Out:   serial
Err:   serial
machid: 8010000
flash_type: 0
### main_loop entered: bootdelay=2

### main_loop: bootcmd="bootipq"
Hit any key to stop autoboot:  0 
SF: Detected MX25L25635E with page size 4 KiB, total 32 MiB
dram_bank_mmu_setup: bank: 0
Can't get node offset for configuration unit name: 'config@4' (FDT_ERR_NOTFOUND)
## Current stack ends at 0x8706fa80 *  kernel: config 'config@ap.dk01.1-c1' from image at 0x84000000
## Booting kernel from FIT Image at 84000000 ...
   Using 'config@ap.dk01.1-c1' configuration
   Trying 'kernel@1' kernel subimage
     Description:  ARM OpenWrt Linux-3.14.77
     Type:         Kernel Image
     Compression:  gzip compressed
     Data Start:   0x840000e4
     Data Size:    3346700 Bytes = 3.2 MiB
     Architecture: ARM
     OS:           Linux
     Load Address: 0x80208000
     Entry Point:  0x80208000
     Hash node:    'hash@1'
     Hash algo:    crc32
     Hash value:   a8f16dcf
     Hash len:     4
     Hash node:    'hash@2'
     Hash algo:    sha1
     Hash value:   450746ab3b7b99e6722246f145043cc16b1ae210
     Hash len:     20
   Verifying Hash Integrity ... crc32 error!
Bad hash value for 'hash@1' hash node in 'kernel@1' image node
Bad Data Hash
ERROR: can't get kernel image!

==================== 修改目录 =================================
~/jl/standard/qca-networking-2017-spf-5-0_qca_oem.git/BOOT.BF.3.1.1/boot_images/build/ms/bin/40xx/misc/cdt 【修改了cdt-AP.DK01.1-S1.bin文件】


fdt addr 0x84000000
fdt header
fdt list
fdt rsvmem print
fdt print
fdt p


fdt addr 0x8432AF00


mkimage -l xxx.img
dtc
fdtget
