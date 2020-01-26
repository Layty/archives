#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "elfhash.h"
#include "hashlist.h"
#include "debug.h"


//初始化hashlist
struct listhash * initializetable(unsigned long tablesize)
{
    struct listhash * H;

    if(tablesize < 0)
    {
        warnTrace("too small\n");
        return NULL;
    }

    H = (struct listhash *)malloc(sizeof(struct listhash));

    if(H == NULL)
    {
        warnTrace("out of space\n");
        return NULL;
    }

    H->tablesize = tablesize;
    H->thelist = (struct listnode * *)malloc(sizeof(struct listnode *)
                 *H->tablesize);

    if(H->thelist == NULL)
    {
        warnTrace("out of space\n");
        free(H);
        return NULL;
    }

    memset(H->thelist, 0 , sizeof(struct listnode *) * H->tablesize);

    return H;
}

struct listnode * find(char *str, struct listhash * H)
{
    struct listnode * listh, *p;

    listh = H->thelist[ELFHash(str, H->tablesize)];

    p = listh;

    while(p != NULL && (strcmp(p->userinfo.name, str) != 0))
    {
        p = p->next;
    }

    return p;
}

int insert(struct listnode * istr, struct listhash * H)
{
    struct listnode * pos;
    struct listnode * newcell, *listf;
    char * str = istr->userinfo.name;

    pos = find(str, H);

    if(pos != NULL)
        return 0;

    newcell = (struct listnode *)malloc(sizeof(struct listnode));

    if(newcell == NULL)
    {
        warnTrace("out of space");

        return 0;
    }
    else

    {
        memcpy(&newcell->userinfo, istr, sizeof(* istr));
        int idx;
        idx = ELFHash(str, H->tablesize);
        listf = (struct listnode *)H->thelist[idx];

        newcell->next = listf;
        H->thelist[idx] = newcell;
    }

    return 1;
}

int  listnodedelete(char * str, struct listhash * H)
{
    struct listnode * listh, *p, *q;
    listh = H->thelist[ELFHash(str, H->tablesize)];
    p = listh;
    char * cmpstr = p->userinfo.name;

    while(p != NULL && (strcmp(cmpstr, str) != 0))
    {
        q = p;
        p = p->next;
    }

    if(p == NULL)
    {
        keyTrace("can not find element\n");
        return 0;
    }
    else
    {
        //delete p
        q->next = p->next;

        free(p);

        p = NULL;

        return 1;
    }
}


//hash 表中每一列的length
int getlistlen(struct listnode * list)
{
    if(list == NULL)
        return 0;

    int n = 0;

    while(list != NULL)
    {
        n++;
        list = list->next;
    }

    return n;
}

//空列的数目
void test1(struct listhash * H)
{
    unsigned int i;

    int blanks = 0;

    for(i = 0; i < H->tablesize; i++)
    {
        struct listnode * list = H->thelist[i];

        if(list == NULL)
            blanks ++;
    }

    keyTrace("blank: %d of %ld\n", blanks, H->tablesize);
    keyTrace("blank: %f%% \n", 100.0 * blanks / H->tablesize);
}

//最大冲突数
void test2(struct listhash * H)
{
    unsigned int i;

    int maxdup = 0;

    for(i = 0; i < H->tablesize; i++)
    {
        int len;

        struct listnode * list = H->thelist[i];
        len = getlistlen(list);

        if(maxdup < len)
            maxdup = len;
    }

    printf("max dup: %d\n", maxdup);
}

//前40个数里面非空的数输出
void test3(struct listhash * H)
{
    dbgTrace("run in %s\n", __FUNCTION__);
    unsigned int i;

    int sum = 0;

    for(i = 0; i < H->tablesize; i++)
    {
        if(sum > 96)
            break;

        struct listnode * list = (struct listnode *)H->thelist[i];
        struct listnode * p = list;

        if(list == NULL)
            continue;

        while(p != NULL)
        {
            sum++;
            dbgTrace("%d   %s  %s  %d\n", sum, p->userinfo.name, p->userinfo.password);
            p = p->next;

        }

    }

}

//调用三个测试函数
void test(struct listhash * H)
{
    test1(H);

    test2(H);

    test3(H);
}

//打印出表里面每一列的数目
void printhash(struct listhash *H)
{
    FILE * fp2;
    fp2 = fopen("D:\\c program\\3.txt", "w");

    if(fp2 == NULL)
    {
        printf("cannot open this file\n");
        exit(0);
    }

    unsigned int i;

    for(i = 0; i < H->tablesize; i++)
    {
        struct listnode *p;

        p = H->thelist[i];
        int number = 0;

        while(p != NULL)
        {
            number++;
            p = p->next;
        }

        fprintf(fp2, "%d\n", number);
    }

    fclose(fp2);
}



