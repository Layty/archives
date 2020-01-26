#include "network.h"
#include "socket_util.h"

#include "timers.h"

#include <pthread.h>
#include "hashlist.h" 


void *main_thread(int num, int listenport);

PFdProcess gFdProcess[MAX_FD];

int main(int argc, char *argv[])
{

#if 1
    setvbuf(stdout, NULL, _IONBF, 0);
#endif

    int listenport = 20020;
    int num = 1;

    if(argc >= 2)
    {
        num = atol(argv[1]);
    }

    if(argc >= 3)
    {
        listenport = atol(argv[2]);
    }

    signal(SIGPIPE, SIG_IGN);  // sigpipe ÐÅºÅÆÁ±Î

    for(int i = 0; i < MAX_FD; ++i)
    {
        PFdProcess p = new FdProcess();

        assert(p);

        gFdProcess[i] = p;
    }

    main_thread(num, listenport);

    return 0;
}

void *main_thread(int num, int listenport)
{
    int epollfd;

    epollfd = epoll_create(MAXEPOLLEVENT);
    int lastfd = 0;

    for(int i = 0; i < num; ++i)
    {
        int fd = Simconnect((char *)"10.9.200.11", listenport);

        if(fd < 0)
        {
			continue;
        }


        set_noblock(fd);

        add_epoll(epollfd, fd);

        gFdProcess[fd]->client_fd = fd;

        do_challange(fd);
		
        gFdProcess[fd]->m_readfun = client_readfun;
        gFdProcess[fd]->m_writefun = client_writefun;
        gFdProcess[fd]->m_closefun = client_closefun;
        lastfd = fd;
    }

    struct epoll_event *m_events;

    m_events = (struct epoll_event *)malloc(MAXEPOLLEVENT *
                                            sizeof(struct epoll_event));

    timer_link global_timer;
    int outime = 1000;

    while(true)
    {
        process_event(epollfd, m_events, outime, &global_timer);
        outime = global_timer.get_mintimer();

        if(outime == 0)
            outime = 1000;
    }
}
