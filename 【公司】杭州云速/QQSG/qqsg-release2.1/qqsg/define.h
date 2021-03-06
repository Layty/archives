#ifndef DEFINE
#define DEFINE

#define byte unsigned char

#define MAX_IPMAC_LEN 30
#define MAC_ADDR_LEN 18
#define ADDR_LEN 16

#define DATALENGTH 512
#define HEADLENGTH 16

#define MP_QUERY        0X01
#define MP_ACCEPT       0X02
#define MP_REFUSE       0X03
#define MP_ABORT        0X04
#define MP_CLIENT       0x05  //客户端通讯特殊标记

#define CM_ID   0X01
#define CM_REGISTER     0X02
#define CM_UNREGISTER   0X03
#define CM_HEARTBEAT   0x04  
#define CM_ONLINE   0x06
#define CM_PROCESSKILLLIST  0x2E
#define CM_PROXY 0x2F
#define CM_RANDOMKEY    0x30//获得通讯随机因子


#define AREALENGTH_ZHANGHAO   30
#define AREALENGTH_IP   16
#define AREALENGTH_MAC   18
#define AREALENGTH_PASSWORD   15
#define AREALENGTH_VERSION  8
#define AREALENGTH_COMPUTERCOUNT 4
#define AREALENGTH_LOG  300
#define AREALENGTH_PROCESS 3 
#define AREALENGTH_DEVNO 3

#define ENCRYPT_BZ  0X01
#define ENCRYPT_VER0  0X01
#define ENCRYPT_VER1  0X02
#define ENCRYPT_VER2  0X03   
#define ENCRYPT_VER3  0X04

//#define DEBUG 
//#define LOCK_DEBUG


#define TIME_OUT_TIME 5
#define BEFORE_TIME_OUT 5
#define HEARTBEAT_TIME_OUT 20
#define REFUSE_TIME 10
#define LOCKFILE "/var/run/qqsg.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define ERRCONNECT -1
#define ERRREFUSE -2
#define ERRDATA	-3



#endif
