#include "network.h"

int Simconnect(char *host, int port)
{
    struct sockaddr_in addr;

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    if(clientfd <= 0)
    {
        return -1;
    }

     printf(" socket new fd %d \n", clientfd);
    
    if(clientfd >= MAX_FD)     // ̫������ ϵͳ�޷�����
    {
        close(clientfd);
        return -2;
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    int i = connect(clientfd, (struct sockaddr *)&addr, sizeof(addr));

    if(i < 0 && errno != EINPROGRESS)
    {
        printf("%s: ", __FUNCTION__);
        printf("errno =%d", errno);
        printf(", %s", strerror(errno));
        printf("\n");
        close(clientfd);
        return -1;
    }
    else
    {
        return clientfd;
    }
}

int add_epoll(int epollfd, int fd, bool r, bool w, bool add_or_mod)
{
    struct epoll_event ev = {0};
    ev.events |= EPOLLRDHUP;

    if(r)
        ev.events |= EPOLLIN | EPOLLET;

    if(w)
        ev.events |= EPOLLOUT | EPOLLET;

    ev.data.fd = fd;

    if(add_or_mod)
    {
        if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1)
        {
            perror("epoll_ctl: fd ");
            exit(-1);
        }
    }
    else
    {
        if(epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev) == -1)
        {
            perror("epoll_ctl: fd ");
            exit(-1);
        }
    }

    return 0;
}




