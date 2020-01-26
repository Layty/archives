#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<time.h>
#include<assert.h>
#include<errno.h>
#include<signal.h>


sem_t sem;
 

static void handler(int sig)
{
    write(STDOUT_FILENO, "sem_post() fromhandler\n", 24);
    if (sem_post(&sem) == -1) {
        write(STDERR_FILENO, "sem_post()failed\n", 18);
        _exit(EXIT_FAILURE);
    }
}

 

int main(int argc,char *argv[])
{
    struct sigaction sa;
    struct timespec ts;
    int s;

 

    if (argc != 3) {
        fprintf(stderr, "Usage: %s<alarm-secs> <wait-secs>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

 

    if (sem_init(&sem, 0, 0) == -1)
        printf("sem_init error");

 

    /* 安置 SIGALRM 处理器；使用 argv[1] 设置警告时钟 */


    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) ==-1)
        printf("sigaction error");

 
    alarm(atoi(argv[1]));

 

    /* 计算相关的时刻，把当前时时加上 argv[2] 指定的秒数 */

    if (clock_gettime(CLOCK_REALTIME, &ts)== -1)
       printf("clock_gettime");

    ts.tv_sec += atoi(argv[2]);
    printf("main() about to callsem_timedwait()\n");

    while ((s = sem_timedwait(&sem,&ts)) == -1 && errno == EINTR)
        continue;       /* Restart if interrupted by handler */
    /* 检查发生了什么 */
    if (s == -1) {
        if (errno == ETIMEDOUT)
            printf("sem_timedwait() timedout\n");
        else
            perror("sem_timedwait");
    } else
        printf("sem_timedwait()succeeded\n");
 
    exit((s == 0) ? EXIT_SUCCESS :EXIT_FAILURE);

}
