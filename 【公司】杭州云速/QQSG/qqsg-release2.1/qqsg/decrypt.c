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
#include "profile.h"
#include "lock.h"
#include "define.h"

struct glob_arg {
	const char* configFile;
};

struct targ {
	struct glob_arg *g;
	struct in_addr beforeserverip;
	struct in_addr host_ip;
	struct ether_addr host_mac;
	char interface[10];
	char ipkey[16];
	char *is_commversion;
	char *is_commrandom;
};

static struct targ targs;
struct glob_arg g;

static int init_resource(){
	char *commversion = "2.0";
	
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
	targs.is_commversion = (char *)malloc(strlen(commversion)*sizeof(char));
	memcpy(targs.is_commversion,commversion,strlen(commversion));	
	//printf("commversion len: %d ; targs.is_commversion %s \n",strlen(commversion),targs.is_commversion);

	//设置通信因子
	targs.is_commrandom = (char *)malloc(9*sizeof(char));
	lock_getCommRandom(targs.is_commrandom);
	targs.is_commrandom[8]='\0';
	//printf("strlen(targs.is_commrandom) : %d ; targs.is_commrandom %s \n",strlen(targs.is_commrandom),targs.is_commrandom);
	return 0;
}


int decrypt(char * pdata, long datalen,struct targ* args)
{

  	printf("prepare decrypt data !!!!\n");
    decrypt_data(pdata,datalen,args->ipkey,16);
    
    if(datalen==(long)(HEADLENGTH+DATALENGTH)){
		  pdata[HEADLENGTH+DATALENGTH - 2]=ENCRYPT_BZ;
		  pdata[HEADLENGTH+DATALENGTH - 1]=0x04;
    }
    return 0;
}

static void usage(void)
{
	const char *cmd = "qqsg";
	fprintf(stderr,
		"Usage:\n"
		"%s arguments\n"
		"\t-f --file : get qqsg config file\n"
		"\t-i --interface : set interface\n"
		"\t-h --help : usage help\n"
		"", cmd);

	exit(0);
}


int main (int argc, char **argv)
{
	int c,datalen;
	targs.g = &g;
	FILE* fp = NULL;
	char pdata[1024];

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
		printf("Can't read file and exit!\n");
		return -1;
	}
	
	printf("read config file -- %s -- \n", g.configFile);
	
	//初始化必要的资源
	if( init_resource() < 0 ){
		printf("init resource fail !\n"); 
		return -1;		
	}	
	
	fp = fopen(g.configFile,"rb");
  if( NULL == fp )
  {
      return (-1);
  }
  datalen = fread(pdata, 528, 1, fp); // 二进制读
  datalen =528;
  fclose(fp);
 
#ifdef DEBUG  
	  int i = 0;
		printf("################### brfore decrypt \n");
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

	decrypt(pdata, datalen,&targs);
	
#ifdef DEBUG  
	  i = 0;
		printf("################### after decrypt\n");
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
}
