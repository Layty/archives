#ifndef NETWORK_H
#define NETWORK_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <assert.h>
#include "timers.h"

#include "des.h"
#include "mytype.h"
#include "protype.h"
#include "xorcode.h"
#include "debug.h"
#include "sdata.h"



#include <string>
#include <iostream>
#include <sched.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include "socket_util.h"
#include "http_client.h"

const int K = 1024;

// 6 万连接数
const int MAX_FD = 6 * 10 * K;

// 每次处理event 数目
const int MAXEPOLLEVENT = 10000;

//#define maxlen   65000

#define MAX_CPU 128

typedef int (*readfun)(int epoll, int fd, timer_link *);
typedef int (*writefun)(int epoll, int fd, timer_link *);
typedef int (*closefun)(int epoll, int fd, timer_link *);
typedef int (*timeoutfun)(int epoll, int fd, timer_link *, time_t tnow);

int accept_readfun(int epoll, int listenfd, timer_link *);
int accept_write(int epoll, int listenfd, timer_link *);

int client_readfun(int epoll, int fd, timer_link *timer);
int client_writefun(int epoll, int fd, timer_link *timer);
int client_closefun(int epoll, int fd, timer_link *timer);

int order_readfun(int epoll, int listenfd, timer_link *);


//extern struct client_info clientinfo;


struct worker_thread_arg
{
    int orderfd;  // listen 线程会用这个发送fd过来
    int cpuid;
};

struct FdProcess
{
    readfun m_readfun;
    writefun m_writefun;
    closefun m_closefun;
    timeoutfun m_timeoutfun;

    time_t m_lastread;
    time_t m_timeout;  // timeout的时间

    int keep_live_timeout;
    int cycle_time;

    bool m_activeclose;

    u32_t client_key;
    u32_t server_key;
    u16_t sendsn;
    char buffer[1024];
    u32_t bufflen;
    int idle_flag;
    int client_fd;
    int times;
    char name[16];
    char password[16];



    FdProcess()
    {
        init();
    }
    void init()
    {
        m_readfun = NULL;
        m_writefun = NULL;
        m_closefun = NULL;
        m_timeoutfun = NULL;
        m_lastread = time(NULL);

        client_fd = 0;
        idle_flag = 0;
        keep_live_timeout = 3;
        cycle_time = 15;

        memset(name, 0, sizeof(name));
        client_key = 0;
        server_key = 0;
        sendsn = 0;
        memset(buffer, 0, sizeof(buffer));
        bufflen = 0;
        m_activeclose = false;
    }
};

typedef struct FdProcess FdProcess;
typedef struct FdProcess *PFdProcess;
#define Log std::cerr

bool connect(int &fd, std::string host, int port);

bool addtimer(timer_link *, int fd, time_t);

bool checkontimer(timer_link *, int *fd, time_t *);

bool stoptimer(timer_link *, int fd, time_t);

time_t getnexttimer(timer_link *);

void process_event(int epollfd, struct epoll_event *m_events, int timeout,
                   timer_link *timers);

void *worker_thread(void *arg);

#endif

