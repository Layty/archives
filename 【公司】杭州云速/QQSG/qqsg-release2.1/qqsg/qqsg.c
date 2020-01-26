#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <getopt.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include <stdbool.h>

#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "profile.h"
#include "lock.h"
#include "define.h"

enum StateMachine {
	INIT = 0,
	REGISTER,
	HEARTBEAT,
	QUIT,
}sgState,lastSgState;

struct glob_arg {
	const char* configFile;
};

struct targ {
	struct glob_arg *g;
	char *username;
	char *password;
	struct in_addr beforeserverip;
	struct in_addr beforeserverip_backup;
	struct in_addr heartbeatserverip;
	struct in_addr controlserverip;
	struct in_addr host_ip;
	char host_mac[18];
	char interface[10];
	char ipkey[16];
	char *is_commversion;
	char *is_commrandom;
	char *devno;
	int heartbeatcycle;
	int	heartcount;
	char* heartlog;
	int heartlogbool;
	//统计数
	int cBeforeIp_count;
};

static struct targ targs;
struct glob_arg g;
int errNumber;
bool ib_encrypt=true; //是否加密
byte ENCRYPT_VER=ENCRYPT_VER3;


/* set advisory lock on file */
int lockfile(int fd)
{
	struct flock fl;
	fl.l_type = F_WRLCK;  /* write lock */
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;  //lock the whole file
	return(fcntl(fd, F_SETLK, &fl));
}


int already_running(const char *filename)
{
	int fd;
	char buf[16];
	fd = open(filename, O_RDWR | O_CREAT, LOCKMODE);
	if (fd < 0) {	
	  printf("can't open %s: %m\n", filename);
	  exit(1);	
	}	
	/* 先获取文件锁 */	
	if (lockfile(fd) == -1) {
	  if (errno == EACCES || errno == EAGAIN) {	
	    printf("file: %s already locked\n", filename);	
	    close(fd);
	    return 1;
	  }
	  printf("can't lock %s: %m\n", filename);
	  exit(1);	
	}	
	/* 写入运行实例的pid */	
	ftruncate(fd, 0);	
	sprintf(buf, "%ld", (long)getpid());	
	write(fd, buf, strlen(buf) + 1);
	return 0;
}

static int read_config_username(const char* file){
	char username[15]; 
	int strLength;
	
	if(GetProfileString(file, "qqsg", "username", username) < 0){
		printf("not found username ! \n"); 
		return -1;
	}
	strLength = strlen(username);
	targs.username = malloc((strLength+1)*sizeof(char));
	memcpy(targs.username,username,strLength);
	targs.username[strLength]='\0';
	return 0;
}

static int read_config_password(const char* file){
	char password[30]; 
	int strLength;
	
	if(GetProfileString(file, "qqsg", "password", password) < 0){
		printf("not found beforeserverip ! \n"); 
		return -1;
	}
	strLength = strlen(password);
	targs.password = malloc((strLength+1)*sizeof(char));
	memcpy(targs.password,password,strLength);
	targs.password[strLength]='\0';
	return 0;
}

static int read_config_beforeserverip(const char* file){
	char ip[16]; 
	
	if(GetProfileString(file, "qqsg", "beforeserverip1", ip) < 0){
		printf("not found beforeserverip ! \n"); 
		return -1;
	}
	inet_aton(ip,&targs.beforeserverip);
	return 0;
}

static int read_config_beforeserverip_backup(const char* file){
	char ip[16]; 
	
	if(GetProfileString(file, "qqsg", "beforeserverip2", ip) < 0){
		printf("not found beforeserverip backup ! \n"); 
		return -1;
	}
	inet_aton(ip,&targs.beforeserverip_backup);
	return 0;
}

static int read_config_interface(const char* file){
	char interface[10];
	
	if(GetProfileString(file, "qqsg", "interface", interface) < 0){
		printf("not found interface ! \n"); 
		return -1;
	}
	memcpy(targs.interface,interface,strlen(interface));
	return 0;
}


int get_mac(char *mac_addr)
{
	int sock;
	struct ifreq ifr;
	char *l_interface = targs.interface,mac_buf[18]={'\0'};
	struct ether_addr *mac; 
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
	   perror("[get_mac] socket fail");
	   return NULL;
	}	
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name, l_interface, sizeof(ifr.ifr_name));
	if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
	{
	   perror("[get_mac] ioctl fail");
	   return NULL;
	}
	close(sock);
	mac = ifr.ifr_hwaddr.sa_data;
	sprintf(mac_buf,"%02x:%02x:%02x:%02x:%02x:%02x",mac->ether_addr_octet[0],mac->ether_addr_octet[1],mac->ether_addr_octet[2],
																					mac->ether_addr_octet[3],mac->ether_addr_octet[4],mac->ether_addr_octet[5]);
	memcpy(mac_addr,mac_buf,17);
	return 0;
}

int get_ip(char* ip_addr)
{
	int sock,len;
	struct sockaddr_in sin;
	struct ifreq ifr;
	char* l_interface = targs.interface;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
	   perror("[get_ip] socket fail");
	   return NULL;
	}
	
	strncpy(ifr.ifr_name, l_interface, sizeof(ifr.ifr_name));
	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
	   perror("[get_ip] ioctl fail");
	   return NULL;
	}
	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	close(sock);
	len = strlen((char*)inet_ntoa(sin.sin_addr));
	memcpy(ip_addr,(char *)inet_ntoa(sin.sin_addr),strlen((char*)inet_ntoa(sin.sin_addr)));
	ip_addr[len]='\0';
	return 1;
}

static int init_resource(){
	char ip[MAX_IPMAC_LEN]={'\0'},mac[MAX_IPMAC_LEN]={'\0'};
	char *commversion = "2.0"; //与密钥服务器交互数据是，使用2.0版本，然后返回通信版本1.0，其他流程均使用1.0版本
	int strLength;
	char *log = "LOG#00001:客户测试";
	
	//初始化qqsg状态机
	sgState = INIT;
	lastSgState = INIT; 
	
	//初始化统计数
	targs.cBeforeIp_count = 1;
	
	//从配置文件中得到用户名
	if(read_config_username(g.configFile) < 0 ){
		printf("read profile file,get username error !\n"); 
		return -1;		
	}
	
	//从配置文件中得到密码
	if(read_config_password(g.configFile) < 0 ){
		printf("read profile file,get password error !\n"); 
		return -1;		
	}		
	
	//从配置文件中得到前置服务器IP
	if(read_config_beforeserverip(g.configFile) < 0 ){
		printf("read profile file,get beforeserver ip error !\n"); 
		return -1;		
	}

	//从配置文件得到备份前置服务器IP
	if(read_config_beforeserverip_backup(g.configFile) < 0 ){
		printf("read profile file,get beforeserver ip error !\n"); 
		return -1;		
	}
	
	//从配置文件中得到认证接口
	if(read_config_interface(g.configFile) < 0 ){
		printf("read profile file,get interface error !\n"); 
		return -1;		
	}

	printf("[init_resource] read configfile %s %s  %s \r\n",targs.interface,targs.username,targs.password);
	printf("[init_resource] beforeserverip %s \n",inet_ntoa(targs.beforeserverip));
	printf("[init_resource] targs.beforeserverip_backup %s \n",inet_ntoa(targs.beforeserverip_backup));
	
	//获取本机IP地址
	get_ip(ip);
	if( ip==NULL ){
		return -1;
	}
	inet_aton(ip,&targs.host_ip);
	
	//获取本机MAC地址
	get_mac(targs.host_mac);
	if( targs.host_mac==NULL ){
		return -1;
	}
	targs.host_mac[18]='\0';
	
	printf("[init_resource] host ip %s mac %s\n",inet_ntoa(targs.host_ip),targs.host_mac);
	
	//设置key，用于加密密钥
  targs.ipkey[0] = 0x5f;
  targs.ipkey[1] = 0x31;
  targs.ipkey[2] = 0x48;
  targs.ipkey[3] = 0x01;
  targs.ipkey[4] = 0x61;
  targs.ipkey[5] = 0x2a;
  targs.ipkey[6] = 0x11;
  targs.ipkey[7] = 0x45;
  targs.ipkey[8] = 0x38;
  targs.ipkey[9] = 0x10;
  targs.ipkey[10] = 0x5b;
  targs.ipkey[11] = 0x27;
  targs.ipkey[12] = 0x36;
  targs.ipkey[13] = 0x1e;
  targs.ipkey[14] = 0x2d;
  targs.ipkey[15] = 0x09;
	
	//设置通信版本
	strLength = strlen(commversion);
	targs.is_commversion = (char *)malloc((strLength+1)*sizeof(char));
	memcpy(targs.is_commversion,commversion,strlen(commversion));
	targs.is_commversion[strLength]='\0';	

	//设置通信因子
	targs.is_commrandom = (char *)malloc(7*sizeof(char));
	lock_getCommRandom(targs.is_commrandom);
	targs.is_commrandom[7]='\0';
	
	//心跳服务器参数
	targs.heartcount = 1;
	strLength = strlen(log);
	targs.heartlog = malloc((strLength+1)*sizeof(char));
	memcpy(targs.heartlog,log,strLength);
	targs.heartlog[strLength] = '\0';
	targs.heartlogbool = 1;
	return 0;
}

int encrypt(char * pdata, long datalen, struct targ* args)
{   
	if(ib_encrypt){
	    switch (ENCRYPT_VER) {
	    case ENCRYPT_VER1:
	        //li_rtn=PAENCRYPT(pdata,datalen,pkey);
	        break;
	    case ENCRYPT_VER2:
	        //i_classlock->encrypt(pdata,datalen,pkey,16);
	        break;
	    case ENCRYPT_VER3:
					encrypt_data(pdata,args->is_commversion,args->is_commrandom,datalen,args->ipkey,16);
	        break;
	    default:
	        break;
	    }
	    if(datalen==(long)(HEADLENGTH+DATALENGTH)){
			  pdata[HEADLENGTH+DATALENGTH - 2]=ENCRYPT_BZ;
			  pdata[HEADLENGTH+DATALENGTH - 1]=ENCRYPT_VER;
	    }
	    
#ifdef DEBUG  
	  int i = 0;
		printf("################### [encrypt] after encrypt data\n");
		while (i < datalen)
		{
			if ((0 != (i%8)) || (0 == i))
			{
				printf("%02x ",(unsigned char)pdata[i]);
				i++;
				continue;
			}
			printf("\n");
			printf("%02x ",(unsigned char)pdata[i]);
			i++;					 
		}
		printf("############################\n");
		printf("\n\n");
#endif

	    return 0;
	}else{
	    return -1;
	} 
}

int decrypt(char * pdata, long datalen,struct targ* args)
{
	if(ib_encrypt){
	    switch (ENCRYPT_VER) {
	    case ENCRYPT_VER1:
	        //li_rtn=PADECRYPT(pdata,datalen,pkey);
	        break;
	    case ENCRYPT_VER2:
	        //i_classlock->decrypt(pdata,datalen,pkey,16);
	        break;   
	    case ENCRYPT_VER3:
	      decrypt_data(pdata,datalen,args->ipkey,16);
	      lock_getCommRandom_v2(pdata,targs.is_commrandom);   
	        break;
	    default:
	        break;
	    } 
	    if(datalen==(long)(HEADLENGTH+DATALENGTH)){
			  pdata[HEADLENGTH+DATALENGTH - 2]=ENCRYPT_BZ;
			  pdata[HEADLENGTH+DATALENGTH - 1]=ENCRYPT_VER;
	    }
	    return 0;
	        //return PADECRYPT(pdata,datalen,pkey);
	}else{
	        return 1;
	}
}

int connect_server(struct in_addr serverIp,int timeout){
  int c_socket;
  struct sockaddr_in s_add;
  unsigned short portnum = 18000;
  
  //建立socket连接
  c_socket = socket (AF_INET, SOCK_STREAM, 0);
  if (-1 == c_socket){
    printf ("[connect_server] socket fail ! \r\n");
    return -1;
  }
 
  struct timeval tm;
	fd_set set;
	unsigned long ul = 1;
	int error=-1, len;
	len = sizeof(int);
	ioctl(c_socket, FIONBIO, &ul); //设置为非阻塞模式
	bool ret = false;


  bzero(&s_add, sizeof (struct sockaddr_in));
  s_add.sin_family = AF_INET;
  s_add.sin_addr.s_addr = serverIp.s_addr;
  s_add.sin_port = htons (portnum);
  printf ("[connect_server] conncect serverip %s:%d \r\n", inet_ntoa(s_add.sin_addr),portnum);

  if (-1 == connect (c_socket, (struct sockaddr *) (&s_add), sizeof (struct sockaddr))){
		tm.tv_sec = timeout;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(c_socket, &set);
		if( select(c_socket+1, NULL, &set, NULL, &tm) > 0)
		{
		    getsockopt(c_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
		    if(error == 0) 
		    	ret = true;
				else 
					ret = false;
		} else {
			ret = false;
		}
  }else{
  	ret = true;
  }
	ul = 0;
	ioctl(c_socket, FIONBIO, &ul); //设置为阻塞模式
	if(!ret)
	{
		close(c_socket);
		fprintf(stderr , "[connect_server] Cannot Connect the server \n");
		return -1;
	}
  
  return c_socket;
}

static int register_keyserver(int timeout){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add, c_add;
  unsigned short portnum = 18000;
  
  int li_length=0;
  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));

	//报文内容，账号、本机ip、本机mac、密码、版本
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割 
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_password[AREALENGTH_PASSWORD];//2.27增加，用于密码验证 
  char ca_version[AREALENGTH_VERSION];//用于掌握客户端版本  

  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_ip, 0, sizeof(ca_ip));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_password, 0, sizeof(ca_password));   
  memset(ca_version, 0, sizeof(ca_version));
  
  sprintf(ca_zhanghao,"%s",targs.username);
	sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
	sprintf(ca_password,"%s",targs.password);
	sprintf(ca_version,"%s","5.10");
	
	printf("\t\n ############## register key server ############## \t\n");

  memcpy(Buffer + HEADLENGTH + li_length,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  li_length+=AREALENGTH_ZHANGHAO;
  memcpy(Buffer + HEADLENGTH + li_length,ca_ip,AREALENGTH_IP); //ip地址
  li_length+=AREALENGTH_IP;
  memcpy(Buffer + HEADLENGTH + li_length,ca_mac,AREALENGTH_MAC); //MAC地址
  li_length+=AREALENGTH_MAC;
  memcpy(Buffer + HEADLENGTH + li_length,ca_password,AREALENGTH_PASSWORD); //密码 
  li_length+=AREALENGTH_PASSWORD;
  memcpy(Buffer + HEADLENGTH + li_length,ca_version,AREALENGTH_VERSION);  

  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_RANDOMKEY;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }    
   
  if( -1 == ( c_socket = connect_server(targs.beforeserverip,timeout) )){
		printf ("[register_keyserver] register backup before serverip with key !\r\n");
		return -1;
  }

  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[register_keyserver] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read (c_socket, revBuffer, 1024))){
    printf ("[register_keyserver] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf("[register_keyserver]read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[register_keyserver] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
	  if(revBuffer[0]==MP_ACCEPT &&  revBuffer[1] == CM_RANDOMKEY){
			printf ("***** [register_keyserver] return accpet ! *****\r\n");
			lock_getCommVersion(revBuffer,targs.is_commversion);
			lock_getCommRandom_v2(revBuffer,targs.is_commrandom);
			printf ("[register_keyserver] fetch commversion %s commrandom %s \r\n",targs.is_commversion, targs.is_commrandom);
	  }else if(revBuffer[0]==MP_REFUSE &&  revBuffer[1] == CM_RANDOMKEY){
  		printf ("***** [register_keyserver] return refuse ! *****\r\n");
  		close(c_socket);
  		return -2;
      //sprintf(ls_message,"%s",Buffer+HEADLENGTH);
      //is_message.printf("%s",ls_message);
      //is_message+="\t";
	  }else{
	    printf ("***** [register_keyserver] return can't analyzing data ! *****\r\n");
	    close(c_socket);
	    return -3;
	  }
	}
  close (c_socket);
  return 0;	
}



static int register_beforeserver(){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add, c_add;
  unsigned short portnum = 18000;
  char *ls_message;
  int strLength,li_length=0;

  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));

	//报文内容，账号、本机ip、本机mac、版本
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割 
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_version[AREALENGTH_VERSION];//用于掌握客户端版本  

  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_ip, 0, sizeof(ca_ip));  
  memset(ca_version, 0, sizeof(ca_version));

  sprintf(ca_zhanghao,"%s",targs.username);
	sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
	sprintf(ca_version,"%s","5.10");
	
	printf("\t\n ############## connecting beforeserver server ############## \t\n");
  
  memcpy(Buffer + HEADLENGTH + li_length,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  li_length+=AREALENGTH_ZHANGHAO;
  memcpy(Buffer + HEADLENGTH + li_length,ca_ip,AREALENGTH_IP); //ip地址
  li_length+=AREALENGTH_IP;
  memcpy(Buffer + HEADLENGTH + li_length,ca_mac,AREALENGTH_MAC); //MAC地址
  li_length+=AREALENGTH_MAC+AREALENGTH_LOG+AREALENGTH_COMPUTERCOUNT;
  memcpy(Buffer + HEADLENGTH + li_length,ca_version,AREALENGTH_VERSION); //VERSION

  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_REGISTER;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }
      
  if( -1 == ( c_socket = connect_server(targs.beforeserverip,TIME_OUT_TIME) )){
		printf ("[register_beforeserver] register before serverip !\r\n");
		return -1;
  }
  
  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[register_beforeserver] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read (c_socket, revBuffer, 1024))){
    printf ("[register_beforeserver] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf(" read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[register_beforeserver] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
	    if(revBuffer[0]==MP_ACCEPT && revBuffer[1] == CM_REGISTER){
	    		printf ("[register_beforeserver] return accpet !\r\n");
	        if(revBuffer[HEADLENGTH]=='T' && revBuffer[HEADLENGTH+1]=='E' && revBuffer[HEADLENGTH+2]=='X' && revBuffer[HEADLENGTH+3]=='T'){
	        		printf ("[register_beforeserver] type is TEXT \r\n");
	            sprintf(ls_message,"%s",revBuffer+HEADLENGTH+4);
	        }else if(revBuffer[HEADLENGTH]=='H' && revBuffer[HEADLENGTH+1]=='T' && revBuffer[HEADLENGTH+2]=='T' && revBuffer[HEADLENGTH+3]=='P'){
	        		printf ("[register_beforeserver] type is HTTP \r\n");
	            sprintf(ls_message,"%s",revBuffer+HEADLENGTH+4);
	
	        }else if(revBuffer[HEADLENGTH]=='S' && revBuffer[HEADLENGTH+1]=='E' && revBuffer[HEADLENGTH+2]=='S' && revBuffer[HEADLENGTH+3]=='M'){
	        		printf ("[register_beforeserver] type is SESM \r\n");
	        		ls_message=revBuffer+HEADLENGTH+4;
	        } else{
	        		printf ("[register_beforeserver] type is't found!! *****\r\n");
	        }
      		int li_count = 1;
      		//解析报，提取skurl,心跳ip,控制ip,消息ip,后置ip,allpass地址(以/结尾),是否采用控制
      		char *tokenPtr=strtok(ls_message,"\t");
					while(tokenPtr!=NULL){
						printf("message[%d] %s ",li_count,tokenPtr);
						switch(li_count){
						    case 1:
						    case 4:
						    case 5:
						    case 8:
						        li_count++;
						        break;
						    case 2:
						    		inet_aton(tokenPtr,&targs.heartbeatserverip);
						    		li_count++;
						        break;
						    case 3:
						    		inet_aton(tokenPtr,&targs.controlserverip);
						    		li_count++;
						        break;
						    /*    
						    case 6:
						        is_allpassurl = sl->Strings[5];
						        is_allpassurl="http://"+ is_allpassurl;
						        break;
						    case 7:
						        if(sl->Strings[6]=="1") ib_controlserver=true;
						        break;
						    */
						    case 9:
						    		sscanf(tokenPtr,"%d",&targs.heartbeatcycle);
				            li_count++;
				            break;
				        /*
						    case 10:
					            ii_checkaftercycle=StrToInt(sl->Strings[9]);
					            break;
					      */
						    case 11:
						    	//sscanf(tokenPtr,"%d",&targs.devno);
						    	strLength = strlen(tokenPtr);
						    	targs.devno = malloc((strLength+1)*sizeof(char));
						    	memcpy(targs.devno,tokenPtr,strLength);
						    	targs.devno[strLength]='\0';
						    	li_count++;
					        break;
						    default:
					    		li_count++;
					        break;
						}
						tokenPtr=strtok(NULL,"\t");
					}
					printf(" \r\n ");
      }else if(revBuffer[0]==MP_REFUSE &&  revBuffer[1] == CM_REGISTER){
				printf ("***** [register_beforeserver] return refuse ! *****\r\n");
				close(c_socket);
				return -2;
			}else{
	   		printf ("***** [register_beforeserver] return can't analyzing data ! *****\r\n");
	   		close(c_socket);
	   		return -3;
	  	}
	}
  close (c_socket);
  return 0;	
}

static int getallpass(){
	int sockfd,h,i,ret;
	FILE *fp; //获取到allpass配置，写入到文件
	struct sockaddr_in servaddr;
	char strHttp[1024]={'\0'};
	//fd_set t_set1;
	//struct timeval  tv;
	char Buffer[1024];
	char *head_buffer;
	memset(Buffer, 0, sizeof(Buffer));
  
 	printf("############## connecting allpass server ##############\n");
  printf("receive data %s \n",Buffer);
 	 
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	        printf("[getallpass] create socket error!\n");
	        exit(0);
	};
	
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	servaddr.sin_addr.s_addr = inet_addr("192.168.147.1");
	
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
					printf("[getallpass] connect error!\n");
	        exit(0);
	}
	printf("connect allpass success\n");
	
	
	//使用get方法，发送数据		
	//strcat(strHttp, "GET /zzgdipclient/newupdate/lnupdate.ini HTTP/1.1\r\n");
	//strcat(strHttp, "authupdate.zzcatv.com.cn\r\n");
	//strcat(strHttp, "text/html, */*\r\n");
	//strcat(strHttp, "Mozilla/3.0 (compatible; Indy Library)\r\n");
	//strcat(strHttp, "\r\n");
	
	strcat(strHttp, "GET /qqsg/allpass.ini HTTP/1.1\r\n");
	strcat(strHttp, "Host: 192.168.13.148\r\n");
	strcat(strHttp, "Connection: Keep-Alive\r\n");
	strcat(strHttp, "Referer: http://192.168.13.148/\r\n");
	strcat(strHttp, "User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.95 Safari/537.36 SE 2.X MetaSr 1.0\r\n");
	strcat(strHttp, "Accept-Encoding: text/html, */*\r\n");
	//strcat(strHttp, "Accept-Encoding: gzip, deflate\r\n");
	strcat(strHttp, "\r\n");
	

	ret = write(sockfd,strHttp,strlen(strHttp));
	if (ret < 0) {
	  printf("write error！error code %d，error info '%s'\n",errno, strerror(errno));
	  exit(0);
	}else{
		printf("send data %d bytes！\n\n", ret);
	}
  
  if(-1 == (i = read(sockfd, Buffer, 1024))){
    printf ("[getallpass] read data fail !\r\n");
    return -1;
  } 
 	head_buffer = strstr(Buffer,"\r\n\r\n")+4;
 	
 	//将返回内容写入文件
	if((fp=fopen("allpass.ini","w"))==NULL)
	{
		printf("[getallpass]can't open file\n");
		exit(0);
	}
	fwrite(head_buffer,strlen(head_buffer),1,fp);
	fclose(fp);

	close (sockfd);
  return 0;	
}

static int register_controlserver(){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add, c_add;
  unsigned short portnum = 18000; 
  int li_length=0,strLength;
  char *ls_message;
  
  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));
	
	//报文内容，账号、本机ip、本机mac、密码、版本,设备号
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_password[AREALENGTH_PASSWORD];//2.27增加，用于密码验证
  char ca_version[AREALENGTH_VERSION];//3.4增加，用于掌握客户端版本   
  char ca_devno[AREALENGTH_DEVNO];//4.3增加，用于设备管理

  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_ip, 0, sizeof(ca_ip));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_password, 0, sizeof(ca_password));   
  memset(ca_version, 0, sizeof(ca_version));    
  memset(ca_devno, 0, sizeof(ca_devno));
 
 	sprintf(ca_zhanghao,"%s",targs.username);
  sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
	sprintf(ca_password,"%s",targs.password);
	sprintf(ca_version,"%s","5.10");
  sprintf(ca_devno,"%s",targs.devno);    
	
	printf("\t\n ############## register control server ############## \t\n");
  memcpy(Buffer + HEADLENGTH + li_length,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  li_length+=AREALENGTH_ZHANGHAO;
  memcpy(Buffer + HEADLENGTH + li_length,ca_ip,AREALENGTH_IP); //ip地址
  li_length+=AREALENGTH_IP;
  memcpy(Buffer + HEADLENGTH + li_length,ca_mac,AREALENGTH_MAC); //MAC地址
  li_length+=AREALENGTH_MAC;
  memcpy(Buffer + HEADLENGTH + li_length,ca_password,AREALENGTH_PASSWORD); //密码 
  li_length+=AREALENGTH_PASSWORD;
  memcpy(Buffer + HEADLENGTH + li_length,ca_version,AREALENGTH_VERSION);
  li_length+=AREALENGTH_VERSION;
  memcpy(Buffer + HEADLENGTH + li_length,ca_devno,AREALENGTH_DEVNO);

  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_REGISTER;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }    

  if( -1 == ( c_socket = connect_server(targs.controlserverip,TIME_OUT_TIME) )){
		printf ("[register_beforeserver] register control serverip !\r\n");
		return -1;
  }
	
  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[register_controlserver] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read(c_socket, revBuffer, 1024))){
    printf ("[register_controlserver] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf("[register_controlserver]read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[register_controlserver] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
		if(revBuffer[0]==MP_ACCEPT &&  revBuffer[1] == CM_REGISTER){
			printf ("***** [connect_controlserver] return accpet ! *****\r\n");
			/*这里应该是解析报文内容，判断是否有新版本需要升级和是否需要进行dhcp。
			*	在我们的客户端下，新版本升级肯定是不需要判断的,dhcp报文内容为false，不需要进行dhcp获取，之前的认证理应有个IP地址获取的过程。
			*/	
		}else if(revBuffer[0]==MP_REFUSE &&  revBuffer[1] == CM_REGISTER){
			//printf ("***** [register_controlserver] return refuse ! *****\r\n");
			ls_message = revBuffer+HEADLENGTH;
			char err[6] = {'\0'};
			memcpy(err,ls_message,5);
			if( strcmp(err,"ERROR") == 0 ){
				//如果发生用户名与密码错误，则就不用重复关联了，需要重新启动程序
				if((unsigned char)ls_message[6] == 0xc4){
					printf ("***** [register_controlserver] accounts error ! *****\r\n");
					exit(0);
				}else if((unsigned char)ls_message[6] == 0xc3){
					printf ("***** [register_controlserver] password error ! *****\r\n");
					exit(0);
				} 
			}
			close(c_socket);
			return -2;
		}else{
			printf ("***** [register_controlserver] return can't analyzing data ! *****\r\n");
			close(c_socket);
			return -3;
		}

	}
  close (c_socket);
  return 0;	
}


static int unregister_beforeserver(){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add, c_add;
  unsigned short portnum = 18000; 
  int li_length=0,strLength;
  char *ls_message;
  
  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));
	
	//报文内容，账号、本机ip、本机mac、密码、版本,设备号
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_password[AREALENGTH_PASSWORD];//2.27增加，用于密码验证
  char ca_version[AREALENGTH_VERSION];//3.4增加，用于掌握客户端版本   
  char ca_devno[AREALENGTH_DEVNO];//4.3增加，用于设备管理

  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_ip, 0, sizeof(ca_ip));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_password, 0, sizeof(ca_password));   
  memset(ca_version, 0, sizeof(ca_version));    
  memset(ca_devno, 0, sizeof(ca_devno));
 
 	sprintf(ca_zhanghao,"%s",targs.username);
  sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
	sprintf(ca_password,"%s",targs.password);
	sprintf(ca_version,"%s","5.10");
  sprintf(ca_devno,"%s",targs.devno);    
	
	printf("\t\n ############## unregister before server ############## \t\n");
  memcpy(Buffer + HEADLENGTH + li_length,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  li_length+=AREALENGTH_ZHANGHAO;
  memcpy(Buffer + HEADLENGTH + li_length,ca_ip,AREALENGTH_IP); //ip地址
  li_length+=AREALENGTH_IP;
  memcpy(Buffer + HEADLENGTH + li_length,ca_mac,AREALENGTH_MAC); //MAC地址
  li_length+=AREALENGTH_MAC;
  memcpy(Buffer + HEADLENGTH + li_length,ca_password,AREALENGTH_PASSWORD); //密码 
  li_length+=AREALENGTH_PASSWORD;
  memcpy(Buffer + HEADLENGTH + li_length,ca_version,AREALENGTH_VERSION);
  li_length+=AREALENGTH_VERSION;
  memcpy(Buffer + HEADLENGTH + li_length,ca_devno,AREALENGTH_DEVNO);

  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_UNREGISTER;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }    

  if( -1 == ( c_socket = connect_server(targs.beforeserverip,TIME_OUT_TIME) )){
		printf ("[register_beforeserver] unregister before serverip !\r\n");
		return -1;
  }
   
  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[unregister_beforeserver] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read(c_socket, revBuffer, 1024))){
    printf ("[unregister_beforeserver] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf("[unregister_beforeserver]read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[unregister_beforeserver] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
		if(revBuffer[0]==MP_ACCEPT &&  revBuffer[1] == CM_UNREGISTER){
			printf ("***** [unregister_beforeserver] return accpet ! *****\r\n");
		}else if(revBuffer[0]==MP_REFUSE &&  revBuffer[1] == CM_UNREGISTER){
			printf ("***** [unregister_beforeserver] return refuse ! *****\r\n");
			close(c_socket);
			return -2;
		}else{
			printf ("***** [unregister_beforeserver] return can't analyzing data ! *****\r\n");
			close(c_socket);
			return -3;
		}

	}
  close (c_socket);
  return 0;	
}

static int register_heartbeatserver(){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add, c_add;
  unsigned short portnum = 18000;
  char *ls_message;
  int strLength,li_length=0;

  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));

	//报文内容，账号、本机ip、本机mac、版本
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割 
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_version[AREALENGTH_VERSION];//用于掌握客户端版本  

  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_ip, 0, sizeof(ca_ip));  
  memset(ca_version, 0, sizeof(ca_version));

  sprintf(ca_zhanghao,"%s",targs.username);
	sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
	sprintf(ca_version,"%s","5.10");
	
	printf("\t\n ############## register heartbeat server ############## \t\n");
  
  memcpy(Buffer + HEADLENGTH + li_length,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  li_length+=AREALENGTH_ZHANGHAO;
  memcpy(Buffer + HEADLENGTH + li_length,ca_ip,AREALENGTH_IP); //ip地址
  li_length+=AREALENGTH_IP;
  memcpy(Buffer + HEADLENGTH + li_length,ca_mac,AREALENGTH_MAC); //MAC地址
  li_length+=AREALENGTH_MAC+AREALENGTH_LOG+AREALENGTH_COMPUTERCOUNT;
  memcpy(Buffer + HEADLENGTH + li_length,ca_version,AREALENGTH_VERSION); //VERSION

  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_REGISTER;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }

  if( -1 == ( c_socket = connect_server(targs.heartbeatserverip,TIME_OUT_TIME) )){
		printf ("[register_beforeserver] register heartbeat serverip !\r\n");
		return -1;
  }
  
  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[register_heartbeatserver] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read (c_socket, revBuffer, 1024))){
    printf ("[register_heartbeatserver] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf(" read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[register_heartbeatserver] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
	    if(revBuffer[0]==MP_ACCEPT && revBuffer[1] == CM_REGISTER){
	    		printf ("***** [register_heartbeatserver] return accpet ! *****\r\n");
      }else if(revBuffer[0]==MP_REFUSE && revBuffer[1] == CM_REGISTER){
					printf ("***** [register_heartbeatserver] return refuse ! *****\r\n");
					close(c_socket);
					return -2;
			}else{
					printf ("***** [register_heartbeatserver] return can't analyzing data ! *****\r\n");
					close(c_socket);
					return -3;
			}
	}
  close (c_socket);
  return 0;	
}

int send_heartbeat(){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add;
  unsigned short portnum = 18000;
  char *ls_message;
  int strLength,li_length=0;
  
  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));

	//报文内容，账号、本机ip、本机mac、版本
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割 
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_log[AREALENGTH_LOG];
  char ca_computercount[AREALENGTH_COMPUTERCOUNT];

  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_ip, 0, sizeof(ca_ip));
  memset(ca_log, 0, sizeof(ca_log));
  memset(ca_computercount, 0, sizeof(ca_computercount));

  sprintf(ca_zhanghao,"%s",targs.username);
	sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
  sprintf(ca_computercount,"C%d",targs.heartcount);
  if(targs.heartlogbool){
  	targs.heartlogbool = 0;
  } else{
  	targs.heartlog = "";
  }
	sprintf(ca_log,"%s",targs.heartlog);
	
	printf("\t\n ############## send heartbeat ############## \t\n");
	printf("[send_heartbeat] caip %s ; camac %s\n",ca_ip,ca_mac);


  memcpy(Buffer + HEADLENGTH,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  memcpy(Buffer + HEADLENGTH + AREALENGTH_ZHANGHAO,ca_ip,AREALENGTH_IP); //ip地址
  memcpy(Buffer + HEADLENGTH + AREALENGTH_ZHANGHAO + AREALENGTH_IP ,ca_mac,AREALENGTH_MAC); //MAC地址
  memcpy(Buffer + HEADLENGTH + AREALENGTH_ZHANGHAO + AREALENGTH_IP + AREALENGTH_MAC ,ca_log,AREALENGTH_LOG); //log
  memcpy(Buffer + HEADLENGTH + AREALENGTH_ZHANGHAO + AREALENGTH_IP + AREALENGTH_MAC + AREALENGTH_LOG ,ca_computercount,AREALENGTH_COMPUTERCOUNT); //log
           
  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_HEARTBEAT;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }
      
  if( -1 == ( c_socket = connect_server(targs.heartbeatserverip,HEARTBEAT_TIME_OUT) )){
		printf ("[register_beforeserver] send heartbeat !\r\n");
		return -1;
  }

  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[send_heartbeat] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read (c_socket, revBuffer, 1024))){
    printf ("[send_heartbeat] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf("[send_heartbeat] read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[send_heartbeat] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
	    if(revBuffer[0]==MP_ACCEPT && revBuffer[1] == CM_HEARTBEAT){
	    		printf ("***** [send_heartbeat] return accpet ! *****\r\n");
      }else if(revBuffer[0]==MP_REFUSE && revBuffer[1] == CM_REGISTER){
					printf ("***** [send_heartbeat] return refuse ! *****\r\n");
					close(c_socket);
					return -2;
			}else{
					printf ("***** [send_heartbeat] return can't analyzing data ! *****\r\n");
					close(c_socket);
					return -3;
			}
	}
  close (c_socket);
  return 0;	
}


void unregister_heartbeatserver(int signo){
  int c_socket;
  int recbytes;
  struct sockaddr_in s_add, c_add;
  unsigned short portnum = 18000; 
  int li_length=0,strLength;
  char *ls_message;
  
  //tcp数据负载，一共16+512=528字节
  char Buffer[HEADLENGTH + DATALENGTH];
  memset(Buffer, 0, sizeof(Buffer));
  char revBuffer[HEADLENGTH + DATALENGTH];
  memset(revBuffer, 0, sizeof(revBuffer));
	
	//报文内容，账号、本机ip、本机mac、密码、版本,设备号
  char ca_zhanghao[AREALENGTH_ZHANGHAO];
  char ca_ip[AREALENGTH_IP]; //.分割
  char ca_mac[AREALENGTH_MAC]; //：分割
  char ca_password[AREALENGTH_PASSWORD];//2.27增加，用于密码验证
  char ca_version[AREALENGTH_VERSION];//3.4增加，用于掌握客户端版本   
  char ca_devno[AREALENGTH_DEVNO];//4.3增加，用于设备管理

	if( signo==SIGINT ){
		printf("catch signal SIGINT and exit!! \n");
	} else if( signo==SIGTERM ){
		printf("catch signal SIGTERM and exit!! \n");
	} else {
		printf("catch signal %d!! \n",signo);
	}
	
  memset(ca_zhanghao, 0, sizeof(ca_zhanghao));
  memset(ca_ip, 0, sizeof(ca_ip));
  memset(ca_mac, 0, sizeof(ca_mac));
  memset(ca_password, 0, sizeof(ca_password));   
  memset(ca_version, 0, sizeof(ca_version));    
  memset(ca_devno, 0, sizeof(ca_devno));
 
 	sprintf(ca_zhanghao,"%s",targs.username);
  sprintf(ca_ip,"%s",inet_ntoa(targs.host_ip));
	sprintf(ca_mac,"%s",targs.host_mac);
	sprintf(ca_password,"%s",targs.password);
	sprintf(ca_version,"%s","5.10");
  sprintf(ca_devno,"%s",targs.devno);    
	
	printf("\t\n ############## unregister heartbeat server ############## \t\n");
  memcpy(Buffer + HEADLENGTH + li_length,ca_zhanghao,AREALENGTH_ZHANGHAO); //账号
  li_length+=AREALENGTH_ZHANGHAO;
  memcpy(Buffer + HEADLENGTH + li_length,ca_ip,AREALENGTH_IP); //ip地址
  li_length+=AREALENGTH_IP;
  memcpy(Buffer + HEADLENGTH + li_length,ca_mac,AREALENGTH_MAC); //MAC地址
  li_length+=AREALENGTH_MAC;
  memcpy(Buffer + HEADLENGTH + li_length,ca_password,AREALENGTH_PASSWORD); //密码 
  li_length+=AREALENGTH_PASSWORD;
  memcpy(Buffer + HEADLENGTH + li_length,ca_version,AREALENGTH_VERSION);
  li_length+=AREALENGTH_VERSION;
  memcpy(Buffer + HEADLENGTH + li_length,ca_devno,AREALENGTH_DEVNO);

  Buffer[0]=MP_QUERY;
  Buffer[1]=CM_UNREGISTER;

  if(encrypt(Buffer,HEADLENGTH + DATALENGTH,&targs)<0){
      return -1;
  }    

  if( -1 == ( c_socket = connect_server(targs.heartbeatserverip,TIME_OUT_TIME) )){
		printf ("[register_beforeserver] unregister heartbeat serverip !\r\n");
		return -1;
  }

  if(-1 == write(c_socket, Buffer, HEADLENGTH + DATALENGTH)){
    printf ("[unregister_heartbeatserver] send data fail !\r\n");
    close(c_socket);
    return -1;  
  }
  if(-1 == (recbytes = read(c_socket, revBuffer, 1024))){
    printf ("[unregister_heartbeatserver] read data fail !\r\n");
    close(c_socket);
    return -1;
  }
  printf("[unregister_heartbeatserver]read bytes %d revBuffer %d\n",recbytes,sizeof(revBuffer));
  
	if(decrypt(revBuffer,HEADLENGTH + DATALENGTH,&targs)<0){
	  printf ("[unregister_heartbeatserver] decrypt data fail !\r\n");
	  close(c_socket);
	  return -1;
	}else{
		if(revBuffer[0]==MP_ACCEPT &&  revBuffer[1] == CM_UNREGISTER){
			printf ("***** [unregister_heartbeatserver] return accpet ! *****\r\n");
		}else if(revBuffer[0]==MP_REFUSE &&  revBuffer[1] == CM_UNREGISTER){
			printf ("***** [unregister_heartbeatserver] return refuse ! *****\r\n");
			close(c_socket);
			return -2;
		}else{
			printf ("***** [unregister_heartbeatserver] return can't analyzing data ! *****\r\n");
			close(c_socket);
			return -3;
		}

	}
  close (c_socket);
  exit(0);
}


static int doProcess(){
	long swap;
	int timeout;
	
	if(sgState == INIT){
		timeout = BEFORE_TIME_OUT*targs.cBeforeIp_count;
		timeout = timeout>60?60:timeout;
		//连接key服务器（地址为前置服务器，端口同样为18000）
		if((errNumber = register_keyserver(timeout)) < 0 ){
			if(errNumber == ERRCONNECT){
				printf("Can't connect before server %s timeout %d count %d!\r\n",inet_ntoa(targs.beforeserverip),timeout,targs.cBeforeIp_count);
				//循环访问主服务器和备份服务器，直到连接上为止
		  	swap = targs.beforeserverip.s_addr;
		  	targs.beforeserverip.s_addr = targs.beforeserverip_backup.s_addr;
		  	targs.beforeserverip_backup.s_addr = swap;
		  	targs.cBeforeIp_count++;
			}
			if( errNumber == ERRREFUSE || errNumber == ERRDATA )
				sleep(REFUSE_TIME); 
		} else {
			lastSgState = sgState;
			sgState = REGISTER;
			printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
		}

	} else if(sgState == REGISTER){
		
		//连接前置服务器
		if(register_beforeserver() < 0 ){
			printf("register before server error !\n"); 
			if( errNumber == ERRREFUSE || errNumber == ERRDATA )
				sleep(REFUSE_TIME); 
			lastSgState = sgState;
			sgState = INIT;
			printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
			return 1;	
		}
		
		//连接Allpass服务器
		//if(getallpass() < 0 ){
		//	printf("connect allpass server error !\n"); 
		//	return -1;				
		//}
		
		//连接控制服务器
		if(register_controlserver() < 0 ){
			printf("register control server error !\n"); 
			if( errNumber == ERRREFUSE || errNumber == ERRDATA )
				sleep(REFUSE_TIME); 
			lastSgState = sgState;
			sgState = INIT;
			printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
			return 1;	
		}
		
		//反注册前置服务器
		if(unregister_beforeserver() < 0 ){
			printf("unregister before server error !\n");
			if( errNumber == ERRREFUSE || errNumber == ERRDATA )
				sleep(REFUSE_TIME);  
			lastSgState = sgState;
			sgState = INIT;
			printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
			return 1;			
		}
	
		//注册心跳服务器
		if( ( errNumber =register_heartbeatserver() ) < 0 ){
			printf("register heartbeat server error !\n");
			//如果两个主机使用同一帐号登录，注册心跳服务器会导致返回拒绝。
			if( errNumber == ERRREFUSE || errNumber == ERRDATA )
				sleep(REFUSE_TIME); 
			lastSgState = sgState;
			sgState = INIT;
			printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
			return 1;	
		}
		
		lastSgState = sgState;
		sgState = HEARTBEAT;
		printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
		
	} else if(sgState == HEARTBEAT){
				
		//发送心跳报文
		if( ( errNumber = send_heartbeat() ) < 0 ){
			printf("register heartbeat server error !\n"); 
			if( errNumber == ERRREFUSE || errNumber == ERRDATA )
				sleep(REFUSE_TIME);
			lastSgState = sgState;
			sgState = INIT;
			printf("\r\n[doProcess] : StateMachine Change %d -> %d \r\n",lastSgState,sgState);
		} else {
			sleep(targs.heartbeatcycle);
		}
		return 1;
		
	} else if(sgState == QUIT){
		return 1;
	} else {
		printf("\r\n[doProcess] Don't recognise sgState and exit! \r\n",lastSgState,sgState);
		return -1;
	}
	
	return 1;		
}

static void usage(void)
{
	const char *cmd = "qqsg";
	fprintf(stderr,
		"Usage:\n"
		"%s arguments\n"
		"\t-f --file : get qqsg config file\n"
		"\t-h --help : usage help\n"
		"", cmd);

	exit(0);
}



int main (int argc, char **argv)
{
	int c;
	targs.g = &g;

	if(already_running(LOCKFILE)){
		printf("QQSG already running !!!\n");
		return 0;
	}

	//解析参数
	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"file", 1, NULL, 'f'},
			{"help", 0, NULL, 'h'},
			{0, 0, 0, 0}
		};
	
		c = getopt_long(argc, argv, "f:h",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'f':
			g.configFile = optarg;
			break;
		case 'h':
			usage();
			break;
			
	  default:
		  usage();
		}
	}
	
	if(!g.configFile){
		printf("Can't read config file and exit!\n");
		return -1;
	}
	
	printf("QQSG read config file << %s >> \n", g.configFile);
	
	//初始化必要的资源
	if( init_resource() < 0 ){
		printf("init resource fail !\n"); 
		return -1;		
	}
	
	signal(SIGINT, unregister_heartbeatserver);
	signal(SIGTERM, unregister_heartbeatserver);

	while(1){
		if( doProcess() < 0 )
			exit(0);
	}
}
