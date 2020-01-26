#ifndef LOCK
#define LOCK

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "define.h"
#include "md5.h"

int lock_getCommRandom(char *commRandom);
int lock_getCommRandom_v2(char * pdata,char *commRandom);
int encrypt_data(char* pdata,char* as_commversion,char* as_commrandom,int datalen,char* pkey,int ai_len);  


#endif
