#ifndef _SDATA_H
#define _SDATA_H

int do_challange(int fd);

int ishdrValid(char *pBuff, u32_t len);

int proc_packet(int fd, char *pBuff, int len);

int recv_chalfun(int fd, char *pbuff);

int  recv_echofun(int fd, char *pBuff);

int do_echofun(int fd, const char *name);

int recv_notifyfun(int fd, char *pbuff);

int do_notifyRespfun(int fd, u16_t server_sn); 

void testserver(int fd); 

int recv_notifyresp_fun(int fd, char *pbuff);




#endif
