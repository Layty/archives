#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

static pthread_rwlock_t rwLock;

void* readFunc(void* ptr)
{
	printf("readFunc want to get rdlock\n");
	if(pthread_rwlock_rdlock(&rwLock) != 0)
		return NULL;
	printf("readFunc get rdlock,sleep 10 and exit thread\n");
	sleep(10);
	pthread_rwlock_unlock(&rwLock);
	printf("readFunc discard rdlock\n");
	return NULL;
}

void* writeFunc(void* ptr)
{
	sleep(1);
	printf("writeFunc want to get wrlock\n");
	pthread_rwlock_wrlock(&rwLock);
	printf("writeFunc get wrlock,sleep 10 and exit thread\n");
	sleep(10);
	pthread_rwlock_unlock(&rwLock);	
	printf("writeFunc discard wrlock\n");
	return NULL;
}

int main(int argc, char* argv[]) 
{
//	pthread_rwlock_t rwLock;
	pthread_rwlock_init( &rwLock, NULL);
	pthread_t id1, id2;

	if(pthread_create(&id1, NULL, readFunc, NULL) != 0) {
		printf("pthread id1 create fail\n");
	}
	if(pthread_create(&id2, NULL, writeFunc, NULL) != 0) {
		printf("pthread id2 create fail\n");
	}

	pthread_join(id1, NULL);
	pthread_join(id2, NULL);

	return 0;
}