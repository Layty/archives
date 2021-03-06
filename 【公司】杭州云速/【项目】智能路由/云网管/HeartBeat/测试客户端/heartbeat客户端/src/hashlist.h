#ifndef __HASHLIST_H
#define __HASHLIST_H

#include "protype.h"

/*用户缓存中的一条记录*/
struct listnode{
		struct client_info userinfo;
		struct listnode *next;
	};

/*用户缓存的首地址和散列表大小*/
struct listhash{
		unsigned long tablesize;
		struct listnode ** thelist;
	};


/*初始化用户缓存列表*/
struct listhash * initializetable(unsigned long tablesize);


/*插入一条数据到用户缓存*/
int insert(struct listnode * istr, struct listhash * H);


/*查找缓存中是否有用户*/
struct listnode * find(char *str, struct listhash * H);


/*在用户缓存表中删除一条记录*/
int  listnodedelete(char * str, struct listhash * H);


void printhash(struct listhash *H);

void test(struct listhash * H);

void test3(struct listhash * H);



#endif 

