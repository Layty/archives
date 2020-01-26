#include "include.h"
#include <time.h>
#include <pthread.h>
#define  FALSE 0
#define  TRUE  1


/**************************************************************************
*功能：打开串口
*参数：串口文件路径
*返回值：返回串口描述符
***************************************************************************/

int OpenDev(char *Dev)
{
    //Dev 就是设备，设备就是文件，就是给出该设备文件的路径
    int fd = open(Dev, O_RDWR | O_NOCTTY); //| O_NOCTTY | O_NDELAY
    if (-1 == fd)
    {
       perror("Can't Open Serial Port");
       return -1;
    }
    else
       return fd;
}

/********************************************************************************************
 *功能：设置串口，是一些默认的模式无效
 *参数：pNewtio：需要设置的数据地址，uBaudRate：波特率
 *返回值：无
 ********************************************************************************************/

int setTermios(struct termios * pNewtio, int uBaudRate)
{
	bzero(pNewtio, sizeof(struct termios)); /* clear struct for new port settings */
	//8N1
	pNewtio->c_cflag = uBaudRate | CS8 | CREAD | CLOCAL;
	pNewtio->c_iflag = IGNPAR;
	pNewtio->c_oflag = 0;
	pNewtio->c_lflag = 0; //non ICANON

	pNewtio->c_cc[VINTR] = 0; /* Ctrl-c */
	pNewtio->c_cc[VQUIT] = 0; /* Ctrl-\ */
	pNewtio->c_cc[VERASE] = 0; /* del */
	pNewtio->c_cc[VKILL] = 0; /* @ */
	pNewtio->c_cc[VEOF] = 4; /* Ctrl-d */
	pNewtio->c_cc[VTIME] = 5; /* inter-character timer, timeout VTIME*0.1 */
	pNewtio->c_cc[VMIN] = 0; /* blocking read until VMIN character arrives */
	pNewtio->c_cc[VSWTC] = 0; /* '\0' */
	pNewtio->c_cc[VSTART] = 0; /* Ctrl-q */
	pNewtio->c_cc[VSTOP] = 0; /* Ctrl-s */
	pNewtio->c_cc[VSUSP] = 0; /* Ctrl-z */
	pNewtio->c_cc[VEOL] = 0; /* '\0' */
	pNewtio->c_cc[VREPRINT] = 0; /* Ctrl-r */
	pNewtio->c_cc[VDISCARD] = 0; /* Ctrl-u */
	pNewtio->c_cc[VWERASE] = 0; /* Ctrl-w */
	pNewtio->c_cc[VLNEXT] = 0; /* Ctrl-v */
	pNewtio->c_cc[VEOL2] = 0; /* '\0' */
	return   TRUE;
}
/**********************************************************************************
 * 功能：接收数据
 * 参数：fd：串口描述符
 * 返回值：无
 **********************************************************************************/
void  recData(int  fd)
{
    unsigned char  caGetMsg[1024];
    int  nRead,i;
    printf("recData : fd = %d\n",fd);
    while(1)
    {
    	//清空字符串
    	memset(caGetMsg,'\0',sizeof(caGetMsg));

        //获取数据
    	nRead=read(fd, caGetMsg, 1024);
        for(i=0;i<nRead;i++)
			printf("0x%x ",caGetMsg[i]);
        printf("\n");
    }
}

//typedef pthread_rwlock_t os_rw_lock_t;
typedef timer_t os_timer_t;
int os_timer_delete(os_timer_t *timer_id)
{
    timer_delete(*timer_id);
	return 1;
}

/***************************************************************************
 * 描述：循环发送串口AT指令集，创建线程完成接收串口返回信息
 */
int  main(int agrc,char* args[])
{
    //os_rw_lock_t lock;
    printf("%d\n",agrc);
	int fd,ret;
	int nCount;
	pthread_t rec_id;
	//char caSendMsg[80];
        //unsigned char cmd1[]={0x01,0x03,0x00,0x02,0xfe};
        unsigned char cmd1[]={0x02, 0x75, 0xFF};
	struct termios oldtio, newtio;
	char *dev;
	if(agrc>1){
		printf("%s\n",args[1]);
		dev = args[1];
		printf("%s\n",dev);
	}
	fd=OpenDev(dev);
        printf("111111111111111111\n");
	if(-1==fd)
	{
		return  FALSE;
	}
	
    ret = pthread_create(&rec_id,NULL,(void *) recData,(void *)fd);
	printf("open fd:%d\n",fd);
	tcgetattr(fd, &oldtio); /* save current serial port settings */
	if(setTermios(&newtio, B115200) == TRUE){
		printf("setTermios OK \n\n");
	}
	tcflush(fd, TCIFLUSH);
	if(tcsetattr(fd, TCSANOW, &newtio)==0){
		printf("111111111111111111\n\n");
		printf("tcsetarrt return 0\n\n");
	}
	while(1)
	{
		//tcflush(fd, TCIFLUSH);    //清空输入缓存
		//tcflush(fd, TCOFLUSH);    //清空输出缓存
		//scanf("%s",caSendMsg);
		//strcat(caSendMsg,"\r\n");
		//strcpy(caSendMsg,"AT*PSFN?\r\n");
		nCount=write(fd, cmd1, 3);
		//printf("=======send data==========================================================\n");
		sleep(2);
	}
	pthread_join(rec_id,NULL);
	tcsetattr(fd, TCSANOW, &oldtio);
	close(fd);
	return 0;
}

