#include "network.h"
#include "mytype.h"
#include "protype.h"
#include "debug.h"
#include "sdata.h"


extern PFdProcess gFdProcess[MAX_FD];

int client_readfun(int epoll, int fd, timer_link* timer)
{
    char buff[1024] = {0};
    char * buff2 = gFdProcess[fd]->buffer;
    int buflen = gFdProcess[fd]->bufflen;

    while(true)
    {
        int len = read(fd, buff, sizeof(buff) - 1);

        if(len == -1)
        {
            buff[0] = 0;
            break;

        }
        else if(len == 0)
        {
            buff[len] = 0;
             dbgTrace("%s: read %d bytes\n", __FUNCTION__, len);
            gFdProcess[fd]->m_activeclose = true;
            return -1;
        }

        if(len > 0)
        {
            gFdProcess[fd]->idle_flag = 1;
            gFdProcess[fd]->times = 0;
            gFdProcess[fd]->m_lastread = time(NULL);

            /*如果消息长度>=1024，可能内存复制会出错*/
            memcpy(buff2 + buflen, buff, len);
            buflen += len;
            gFdProcess[fd]->bufflen = buflen;

            while(true)
            {
                if(ishdrValid(buff2, buflen) < 0)
                {
                    break;
                }

                int remain_data = proc_packet(fd, buff2, buflen);

                memcpy(buff2, buff2 + buflen - remain_data, remain_data);

                buflen = remain_data;

                gFdProcess[fd]->bufflen = buflen;

                if(buflen == 0)
                {
                    break;
                }
            }

        }
    }

    gFdProcess[fd]->idle_flag = 0;
    return 1;
}

int client_writefun(int epoll, int fd, timer_link* timer)
{
    return 1;
}

int client_closefun(int epoll, int fd, timer_link* timer)
{
    //timer->remote_timer(gFdProcess[fd]);
    gFdProcess[fd]->init();
    ::close(fd);
    close(fd);
    return 1;
}

