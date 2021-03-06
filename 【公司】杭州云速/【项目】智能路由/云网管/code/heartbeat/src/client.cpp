#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#include "debug.h"
#include "receive.h"
#include "network.h"
#include "hashlist.h"
#include "operateredis.h"

extern pthread_mutex_t mac_cache_mutex;
extern pthread_cond_t mac_cache_cond;

extern struct listhash *mac_cache_name;

extern PFdProcess gFdProcess[MAX_FD];


void * recvMalloc(u32_t len)
{
    void *p = malloc(len);

    if(p == NULL)
    {
        return NULL;
    }
    else
    {
        memset(p, 0, len);
        return p;
    }
}

int client_readfun(int epoll, int fd, timer_link *timer)
{
    char buff[1024] = {0};

    char * oldBuff = gFdProcess[fd]->gbuf;
    int dataLen = gFdProcess[fd]->dataLen;
    int maxLen = gFdProcess[fd]->maxLen;

    while(true)
    {
        int len = read(fd, buff, 256);

        if(len < 0)
        {
            buff[0] = 0;

            if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
            {
                break;
            }
            else
            {
                infoTrace("%s: read< 0 bytes\n", __FUNCTION__);
                gFdProcess[fd]->m_activeclose = true;
                return -1;
            }
        }
        else if(len == 0)
        {
            infoTrace("%s: read= 0 bytes\n", __FUNCTION__);
            buff[len] = 0;
            gFdProcess[fd]->m_activeclose = true;
            return -1;
        }

        int totalLen = dataLen + len;

        if(oldBuff == NULL && maxLen == 0)
        {
            oldBuff = (char *)recvMalloc(512);

            if(oldBuff == 0)
            {
                gFdProcess[fd]->m_activeclose = true;
                return -1;
            }

            gFdProcess[fd]->gbuf = oldBuff ;
            gFdProcess[fd]->maxLen = 512;
            maxLen = 512;
            dbgTrace("first allocate  %d\n", maxLen);
        }

        if(totalLen <= maxLen)
        {
            memcpy(oldBuff + dataLen, buff, len);
            dataLen += len;
            gFdProcess[fd]->dataLen = dataLen;
            dbgTrace("收到的数据长度=  %d 不用重新分配空间\n", dataLen);
        }
        else
        {
            char *newBuff = (char *)recvMalloc(totalLen);

            if(newBuff == 0)
            {
                gFdProcess[fd]->m_activeclose = true;
                return -1;
            }

            gFdProcess[fd]->gbuf = newBuff ;
            gFdProcess[fd]->maxLen = totalLen;
            maxLen = totalLen;

            memcpy(newBuff, oldBuff, dataLen);
            memcpy(newBuff + dataLen, buff, len);

            dataLen += len;
            gFdProcess[fd]->dataLen = dataLen;
            dbgTrace(" 接收缓存空间不足，重新分配缓存=  %d ,收到数据长度为 %d\n", maxLen, dataLen);
            free(oldBuff);
            oldBuff = newBuff;

        }

        char *recvBuff = gFdProcess[fd]->gbuf;

        while(true)
        {
            if(dataLen == 0)
            {
                break;
            }

            int invalidFlag = ishdrValid(recvBuff, dataLen);

            if(invalidFlag < 0 && invalidFlag != -3)
            {
                break;
            }

            if(invalidFlag == -3)
            {
                THDR *pHdr;
                pHdr = (THDR *)recvBuff;
                int needLen = 0;
                dbgTrace("接收到的数据长度为 :%d\n", dataLen);
                needLen = sizeof(THDR) + pHdr->pktlen;
                dataLen = dataLen - needLen;
                dbgTrace("丢弃无效包后，剩下数据长度为 :%d\n", dataLen);
                memcpy(recvBuff, recvBuff + needLen, dataLen);
                gFdProcess[fd]->dataLen = dataLen;
                continue;
            }

            int remainLen = proc_packet(fd, recvBuff, dataLen);

            if(remainLen > 0)
            {
                memcpy(recvBuff, recvBuff + dataLen - remainLen, remainLen);
            }

            dataLen = remainLen;
            gFdProcess[fd]->dataLen = dataLen;

        }

    }

    return 1;
}

int client_writefun(int epoll, int fd, timer_link *timer)
{
    return 1;
}

int client_closefun(int epoll, int fd, timer_link *timer)
{
    dbgTrace("%s  %d close(%d)\n", __FUNCTION__, __LINE__, fd);

    timer->remote_timer(gFdProcess[fd]);

    char *deviceName = gFdProcess[fd]->equipmentSn;

    char judgeDevice[8] = {0};

    char formatMac[24] = {0};

    strncpy(judgeDevice, deviceName, 6);
    int recvLen = strlen(judgeDevice) ;

    dbgTrace("%s:the equipmentSnlen is %d\n", __FUNCTION__ , recvLen);

    pthread_mutex_lock(&mac_cache_mutex);

    struct listnode *pos = NULL;
    pos = findNode(deviceName, mac_cache_name);
    struct mac_info * pdevice = NULL;

    if(pos != NULL)
    {
        pdevice = &pos->macInfo;

        dbgTrace("%s  %d  fd=%d   online=%d\n", __FUNCTION__,
                 __LINE__, pdevice->sessionFd, pdevice->onLineStatus);

        if(pdevice->sessionFd == fd)
        {
            pdevice->sessionFd = 0;
            pdevice->onLineStatus = OFFLINE;
        }
    }

    pthread_mutex_unlock(&mac_cache_mutex);

    if(6 == recvLen)
    {
        int setFlag;
        char command[64] = "set online-";

        changeMac(deviceName, formatMac, recvLen);

        strcat(command, formatMac);
        strcat(command, "  0");

        setFlag = setKeyValueToRedis(command);

        if(setFlag == 0)
            dbgTrace("%s:success set the key\n", __FUNCTION__);


        char delCommade[64] = "del heartbeat-";
        strcat(delCommade, formatMac);
        delKeyValueInRedis(delCommade);
    }

    gFdProcess[fd]->init();

    close(fd);

    return 1;
}

int client_timeoutfun(int epoll, int fd, timer_link *timers, time_value tnow)
{
    
    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];

    int interval = tnow - pCnxt->m_lastecho;

	int setInterval=getInterval();

	dbgTrace("%s  %d  fd=%d  setInterval=%d\n", __FUNCTION__, __LINE__, fd,setInterval);
	
    if(pCnxt->echoFlag == 1 && interval > setInterval * 1000)
    {
        client_closefun(epoll, fd, timers);
		
        dbgTrace("%s  heartbeat timeout  %d\n", __FUNCTION__, fd);
    }

    timers->add_timer(gFdProcess[fd], tnow + 10 * 1000);

    return 0;
}



