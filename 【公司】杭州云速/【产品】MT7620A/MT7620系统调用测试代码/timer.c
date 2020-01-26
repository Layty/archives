#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#define CLOCKID CLOCK_REALTIME

static int count = 0;
static timer_t timerid;

void timer_thread(union sigval v)
{
	printf("timer_thread function! %d\n", v.sival_int);
}

void sig_handler(int signo)  
{  
	count++;
    printf("timer_signal function! %d count = %d\n", signo,count);
	if(count == 10){
		timer_delete(timerid);
	}
}  


int main()
{
	struct sigevent evp;
    struct sigaction act;  
    memset(&act, 0, sizeof(act));  
    act.sa_handler = sig_handler;  
    act.sa_flags = 0;  

    sigemptyset(&act.sa_mask);  
  
    if (sigaction(SIGUSR1, &act, NULL) == -1)  
    {  
        printf("fail to sigaction");  
        exit(-1);  
    }  

	memset(&evp, 0, sizeof(struct sigevent));		//清零初始化
	
	evp.sigev_value.sival_int = 111;			//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
	//evp.sigev_notify = SIGEV_THREAD;			//线程通知的方式，派驻新线程
	evp.sigev_signo = SIGUSR1;  
	evp.sigev_notify = SIGEV_SIGNAL; 
	//evp.sigev_notify_function = timer_thread;		//线程函数地址

	if (timer_create(CLOCKID, &evp, &timerid) == -1)
	{
		printf("fail to timer_create\n");
		exit(-1);
	}
	printf("create timer %d\n",&timerid);
	
	struct itimerspec it;
	it.it_interval.tv_sec = 1;
	it.it_interval.tv_nsec = 0;
	it.it_value.tv_sec = 1;
	it.it_value.tv_nsec = 0;

	if (timer_settime(timerid, 0, &it, NULL) == -1)
	{
		printf("fail to timer_settime\n");
		exit(-1);
	}

	for(;;)
	{
		pause();
	}

	return 0;
}
/*
 * int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
 * 获取timerid指定的定时器的值，填入curr_value
 *
 */