#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "elfhash.h"
#include "hashlist.h"
#include "debug.h"

struct listhash * initializetable(unsigned long tablesize)
{
    struct listhash * H;

    if(tablesize <= 0)
    {
        warnTrace("%s:user cache size too small\n", __FUNCTION__);
        return NULL;
    }

    H = (struct listhash *)malloc(sizeof(struct listhash));

    if(H == NULL)
    {
        warnTrace("%s:user cache allocation fail\n", __FUNCTION__);
        return NULL;
    }

    H->tablesize = tablesize;
    H->thelist = (struct listnode * *)malloc(sizeof(struct listnode *)*
                 H->tablesize);

    if(H->thelist == NULL)
    {
        warnTrace("%s:out of space\n", __FUNCTION__);
        free(H);
        return NULL;
    }

    memset(H->thelist, 0 , sizeof(struct listnode *) * H->tablesize);

    return H;
}

struct listnode * findNode(char *str, struct listhash * H)
{
    struct listnode * listh, *p;

    listh = H->thelist[ELFHash(str, H->tablesize)];

    p = listh;

    while(p != NULL && (strncmp(p->macInfo.equipmentSn, str, 6) != 0))
    {
        p = p->next;
    }

    return p;
}

//如果没有就插入结点，如果有，就修改状态
int insertNode(struct listnode * istr, struct listhash * H)
{
    struct listnode * pos;
    struct listnode * newcell, *listf;
    char * str = istr->macInfo.equipmentSn;

    pos = findNode(str, H);

    if(pos != NULL)
    {
        pos->macInfo.onLineStatus = istr->macInfo.onLineStatus;
        pos->macInfo.sessionFd = istr->macInfo.sessionFd;
        return 0;
    }

    newcell = (struct listnode *)malloc(sizeof(struct listnode));

    if(newcell == NULL)
    {
        warnTrace("%s:out of space\n", __FUNCTION__);

        return 0;
    }
    else

    {
        memcpy(&newcell->macInfo, istr, sizeof(* istr));
        int idx;
        idx = ELFHash(str, H->tablesize);
        listf = (struct listnode *)H->thelist[idx];

        newcell->next = listf;
        H->thelist[idx] = newcell;
    }

    return 1;
}


int deleteNode(char * str, struct listhash * H)
{
    struct listnode * listh, *p, *q;
    p = q = NULL;
    listh = H->thelist[ELFHash(str, H->tablesize)];
    p = listh;

    if(p == NULL)
    {
        return 0;
    }
    else
    {
        char * cmpstr = p->macInfo.equipmentSn;

        if(strncmp(cmpstr, str, 6) == 0)
        {
            H->thelist[ELFHash(str, H->tablesize)] = p->next;

            free(p);
            p = NULL;
            return 1;
        }
        else
        {
            while(p != NULL && (strncmp(cmpstr, str, 6) != 0))
            {
                q = p;
                p = p->next;

                cmpstr = p->macInfo.equipmentSn;
            }

            if(p == NULL)
            {
                keyTrace("%s:can not find element\n", __FUNCTION__);
                return 0;
            }
            else
            {
                q->next = p->next;

                struct listnode *testp;
                testp = findNode(str, H);

                if(testp == NULL)
                {
                    dbgTrace("%s:  success delete\n", __FUNCTION__);
                }
                else
                {
                    dbgTrace("%s: p:%p\n", __FUNCTION__, testp);
                }

                free(p);

                p = NULL;

                return 1;

            }

        }

    }

    return 0;

}


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

void test3(struct listhash * H)
{
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
            dbgTrace("%s:%d   %s  %d  %d\n", __FUNCTION__, sum, p->macInfo.equipmentSn, p->macInfo.onLineStatus, p->macInfo.sessionFd);
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


