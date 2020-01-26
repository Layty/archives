#!/bin/sh
######import config######
. /sbin/config.sh
######import config######

cvn3gdir="/tmp"
logfile="/tmp/recheck.log"
modeswitch_dir="/etc/usb_modeswitch.d"
LAST3G_FILE="/tmp/last_match3G"
dial3G_fail_file="/tmp/dail3G_fail"
LOCK_FILE=/var/lock/LOCK.monitor3G
LOG_FIlE=/tmp/3Ginfo.log
DEBUG_LEVEL=0

debug_print()
{          
    level=$1
	print_log=$2
    if [ $level -le ${DEBUG_LEVEL} -a ${DEBUG_LEVEL} -ne 0 ]; then  
		message="[$(date +'%Y-%m-%d %H:%M:%S')] [monitor3G.sh] ${print_log}"    
		echo $message                                                          
		echo $message >> ${LOG_FIlE}  
	fi                            
}

func_init_resource()
{
	if [ $(mount|grep jffs2|wc -l) -ge 1 ]; then
		jffs2_dir=$(mount|grep jffs2|awk -F' ' '{print $3}')
		LAST3G_FILE=${jffs2_dir}/last_match3G
	fi
}

func_psExitPro()
{
	pName=$1
	if [ $(ps|grep ${pName}|grep -v grep|wc -l) -ge 1 ]; then
		return 0
	fi
	return 1
}

func_wait_time()
{
	command=$1
	pName=$2
	timeout=$3
	count=0
	debug_print "3" "$command $pName $timeout"
	while [ $count -lt ${timeout} ]
	do
		if [ $(${command} | grep ${pName} | wc -l) -ge 1 ]; then
			return 0
		fi
		count=`expr $count + 1`
		sleep 1
	done
	return 1
}


func_dial()
{
	if [ "y$CONFIG_USER_LEDTU" != "y" ]; then
		pppd 1 file /etc/cdmaUSB0 -d &
	fi
	#pppd 1 file /etc/cdmaUSB1 -d &
	#pppd 1 file /etc/wcdmaUSB0 -d &
	#pppd 1 file /etc/wcdmaUSB1 -d &
	
	func_wait_time ifconfig ppp0 60
	if [ $? -eq 0 ]; then
		debug_print "1"  "Dial Success"
		exit
	fi

	debug_print "1" "Dial Fail"

}

func_checkWanLink()
{
	######## check wan port link ############
	wanlink=$(switch reg r 80|awk -F'=' '{print $3}')
	wanlink=`expr substr $wanlink 7 8`
	wanlink=`expr $wanlink / 10`
	
	if [ $wanlink -eq 0 -a $oldlink -ne $wanlink ]; then
		killall udhcpc
		sleep 2
	fi
	
	if [ $wanlink -eq 1 -a $oldlink -ne $wanlink ]; then
		killall udhcpc
		sleep 2
		udhcpc -W 1 -i wangw.1 -s /sbin/udhcpc.sh -p /var/run/udhcpc_wangw.1.pid
	fi
	oldlink=$wanlink
	
	sleep 10
}

func_check3G()
{
	exsit_3g=0
	exit_switched_3g=0
	match_vendor=0
	match_product=0

	if [ "y$CONFIG_USER_LEDTU" != "yy" ]; then
		sleep 60
	fi	
	
	cat /proc/bus/usb/devices|grep Vendor|awk -F'=' '{print $2,$3}'|awk -F' ' '{print $1,$3}'|while read vendor product
	do
		debug_print "3" "$vendor $product"
		matchfile="${vendor}_${product}"
		if [ $(ls ${modeswitch_dir}|grep $matchfile|wc -l) -ge 1 ]; then
			debug_print "2" "######### find 3g card, not switchmode ###########"
			exsit_3g=1
			match_vendor=$vendor
			match_product=$product
		elif [ $(cat ${LAST3G_FILE}|grep $matchfile|wc -l) -eq 1 ]; then
			debug_print "2" "######### find 3g card, switched mode ###########"
			exit_switched_3g=1
			match_vendor=$vendor
			match_product=$product
		else
			continue
		fi
		
		func_psExitPro ppp
		if [ $? -eq 0 ]; then
			debug_print "2" "######### exsit pppp ###########"
			exitppp=1
			#存在ppp网络接口，直接退出这次检测。
			return 1
		else
			debug_print "2" "######### not exsit pppp ###########"
			exitppp=0
		fi		

		debug_print "3" "#### exsit_3g=${exsit_3g} exit_switched_3g =${exit_switched_3g} exitppp=$exitppp  match_vendor=${match_vendor}   match_product=${match_product} ######"

		if [ $exitppp -eq 0 -a ${exsit_3g} -eq 1 -o ${exit_switched_3g} -eq 1 -a $exitppp -eq 0 ]; then
			debug_print "3" "############### connect3G.sh -v ${match_vendor} -p ${match_product} -d ${DEBUG_LEVEL}  ....................."
			connect3G.sh -v ${match_vendor} -p ${match_product} -d ${DEBUG_LEVEL}
			if [ $? -ne 0 ]; then
				debug_print "3" "############### ret is not 0 after connect3G.sh ....................."
				dial3G_fail_count=$(cat ${dial3G_fail_file})
				dial3G_fail_count=`expr ${dial3G_fail_count} + 1`
				echo ${dial3G_fail_count} > ${dial3G_fail_file}
				debug_print "1" "############# Dail Fail; dial3G_fail_count=${dial3G_fail_count}######################"
				if [ ${dial3G_fail_count} -eq 2 ]; then
					reboot
				fi
			else
				debug_print "1" "############# Dail success; set dial3G_fail_count=0 ######################"
				echo 0 > ${dial3G_fail_file}
			fi
		fi
	done
	
	if [ "y$CONFIG_USER_LEDTU" != "y" ]; then
		sleep 60
	fi	
}


func_exit()
{	
	exit_code=$1
	rm -f ${LOCK_FILE}
	#exit ${exit_code}
}

usage () {
  echo "usage: monitor3G.sh [option]..."
  echo "options:"
  echo "  -h              : print this help"
  echo "  -d debug_level  : set debug_level"
  func_exit 0
}

for arg in $*
do
    if [ "$1" != "" ]; then
      case "$1" in
        "-d")
          DEBUG_LEVEL=$2
          shift ;;
        "-h")
          usage ;;
        *)
          echo "illegal option -- $2" 
          usage ;;
      esac
      shift
	fi
done

func_init_resource

oldlink=0
if [ "y$CONFIG_USER_LEDTU" != "y" ]; then
	echo 0 > ${dial3G_fail_file}
fi

while [ true ]
do
	if [ "y$CONFIG_USER_LEDTU" != "y" ]; then
		func_check3G
	else
		func_checkWanLink
		func_check3G
	fi
done

func_exit 0
