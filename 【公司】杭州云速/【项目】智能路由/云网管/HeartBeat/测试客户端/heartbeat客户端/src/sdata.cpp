#include "network.h"
#include "sdata.h"

extern PFdProcess gFdProcess[MAX_FD];

#define ARRAYSIZE  100*16

int ishdrValid(char *pBuff, u32_t len)
{
    THDR *pHdr;
    pHdr = (THDR *)pBuff;

    if(len < sizeof(THDR))
    {
        dbgTrace("recv invalid pkt, len = %d\n", len);
        return -1;
    }

    if(pHdr->flag != PKT_HDR_MAGIC)
    {
        dbgTrace("recv invalid pkt, recv magic=0x%X, right=0x%X\n", pHdr->flag, PKT_HDR_MAGIC);
        return -2;
    }

    unsigned int needlen = sizeof(THDR) + pHdr->pktlen;

    if(len < needlen)
    {
        return -3;
    }

    return 1;
}

int proc_packet(int fd, char *pBuff, int len)
{
    THDR *pHdr;
    pHdr = (THDR *)pBuff;
    int datalen = sizeof(THDR) + pHdr->pktlen;

    dbgTrace("recv packet, type = %d\n", pHdr->pktType);

    if(pHdr->pktType == PKT_CHALLENGE_RESPONSE)
    {
        recv_chalfun(fd, pBuff);
    }
    else if(pHdr->pktType == PKT_ECHO_RESPONSE)
    {
        recv_echofun(fd, pBuff);
    }
    else if(pHdr->pktType == PKT_NOTIFY_REQUEST)
    {
        recv_notifyfun(fd, pBuff);
    }
    else if(pHdr->pktType == PKT_NOTIFY_RESPONSE)
    {
        recv_notifyresp_fun(fd, pBuff);

    }
    else
    {
        dbgTrace("invalid pocket\n");
    }

    int remaindata = len - datalen;

    if(remaindata > 0)
        return remaindata;
    else
        return 0;
}

int recv_notifyfun(int fd, char *pbuff)
{
    THDR *pHdr;
    pHdr = (THDR *)pbuff;

    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];
    u32_t serverKey = pCnxt->client_key;

    TNOTIFYREQ notifyReqMsg;
    memset(&notifyReqMsg, 0, sizeof(notifyReqMsg));

    XORencode(pbuff + sizeof(THDR), &notifyReqMsg, serverKey, pHdr->pktlen);

    return 1;
}
int send_notifyfun(int fd, TNOTIFYREQ *pnotifyReqMsg)
{
    if(NULL == pnotifyReqMsg)
    {
        infoTrace("函数%s:第%d 行消息长度为0\n", __FUNCTION__, __LINE__);
        return -1;
    }

    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];
    pCnxt->sendsn++;
    u32_t serverKey = pCnxt->server_key;

    THDR notifyRequestHdr;
    THDR *pHdr = &notifyRequestHdr;
    memset(pHdr, 0, sizeof(*pHdr));

    pHdr->flag = PKT_HDR_MAGIC;
    pHdr->version = PKT_VERSION;
    pHdr->pktType = PKT_NOTIFY_REQUEST;
    pHdr->sn = pCnxt->sendsn;

    TNOTIFYREQ sendNotifyReqMsg;

    pHdr->pktlen = sizeof(sendNotifyReqMsg);

    XORencode(pnotifyReqMsg, &sendNotifyReqMsg, serverKey, pHdr->pktlen);

    int sendLen = 0;
    int needLen = sizeof(sendNotifyReqMsg) + sizeof(THDR);

    sendLen = send(fd, pHdr, sizeof(THDR), 0);

    sendLen += send(fd, &sendNotifyReqMsg, pHdr->pktlen, 0);

    if(sendLen == needLen)
    {
        return 1;
    }
    else
    {
        return -2;
    }
}

int   recv_echofun(int fd, char *pbuff)
{
    THDR *pHdr;
    pHdr = (THDR *)pbuff;
    TECHORESP   Recho_reqmsg;

    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];

    u32_t key = pCnxt->client_key;

    XORencode(pbuff + sizeof(THDR), &Recho_reqmsg, key, pHdr->pktlen);

    return 1;

}

int do_echofun(int fd, const char *name)
{
    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];

    pCnxt->times ++;
    pCnxt->m_lastread = time(NULL);
    pCnxt->sendsn++;

    u32_t key = pCnxt->server_key;

    THDR echo_hdr;
    THDR *pHdr = &echo_hdr;

    pHdr->flag = PKT_HDR_MAGIC;
    pHdr->pktlen = sizeof(TECHOREQ);
    pHdr->version = PKT_VERSION;
    pHdr->pktType = PKT_ECHO_REQUEST;
    pHdr->sn = pCnxt->sendsn;

    TECHOREQ echo_reqmsg;
    TECHOREQ *pReq = &echo_reqmsg;

    strncpy((char *)pReq->equipmentSn, name, 6);

    TECHOREQ secho_reqmsg;
    int bytes = sizeof(TECHOREQ);

    XORencode(&echo_reqmsg, &secho_reqmsg, key, bytes);

    send(fd, pHdr, sizeof(THDR), 0);
    send(fd, &secho_reqmsg, bytes, 0);

    return 1;
}

int do_challange(int fd)
{

    dbgTrace("%s  fd=%d\n", __FUNCTION__,fd);
	
    FdProcess *pCnxt;

    if(fd <= 0)
    {
        printf("%s error fd = %d\n", __FUNCTION__, fd);
        return -1;
    }

    pCnxt = gFdProcess[fd];
    pCnxt->sendsn++;

    THDR chal_header;
    THDR *pHdr = &chal_header;

    pHdr->flag = PKT_HDR_MAGIC;
    pHdr->pktlen = sizeof(TCHALREQ);
    pHdr->version = PKT_VERSION;
    pHdr->pktType = PKT_CHALLENGE_REQUEST;
    pHdr->sn = pCnxt->sendsn;

    TCHALREQ chal_reqmsg;
    TCHALREQ *pReq = &chal_reqmsg;

    pReq->magic = PKT_CHALLENGE_MAGIC;
    pReq->key = (u32_t)time(NULL);
	
    memset(pReq->u8res, 0x08, sizeof(pReq->u8res));

    pCnxt->client_key = chal_reqmsg.key;

    unsigned char deschal[64];
    memset(deschal, 0, sizeof(deschal));

    int bytes = sizeof(TCHALREQ);

    des_encode((const void *)pReq, deschal, CHANLLENGE_KEY, bytes);

    pHdr->pktlen = bytes;

    send(fd, pHdr, sizeof(THDR), 0);
    send(fd, deschal, bytes, 0);

    return 1;
}


int recv_chalfun(int fd, char *pbuff)
{
    dbgTrace("%s\n", __FUNCTION__);

    THDR *pHdr;
    pHdr = (THDR *)pbuff;

    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];

    TCHALRESP  Rchal_respmsg;
    TCHALRESP  *pReq = &Rchal_respmsg;

    des_decode((void *)(pbuff + sizeof(THDR)), pReq, CHANLLENGE_KEY, sizeof(*pReq));

    u32_t * pchage = (u32_t *)&Rchal_respmsg.key;
    pCnxt->server_key = *pchage;

    TNOTIFYREQ notifyReqMsg;
    memset(&notifyReqMsg, 0, sizeof(notifyReqMsg));
    strncpy(notifyReqMsg.equipmentSn, "601234", 6);

    send_notifyfun(fd, &notifyReqMsg);

    return 1;
}
int recv_notifyresp_fun(int fd, char *pbuff)
{

    THDR *pHdr;
    pHdr = (THDR *)pbuff;

    FdProcess *pCnxt;
    pCnxt = gFdProcess[fd];
    u32_t key = pCnxt->server_key;

    TNOTIFYRESP  notifyReqMsg;

    XORencode(pbuff + sizeof(THDR), &notifyReqMsg, key, pHdr->pktlen);
    dbgTrace("returnCode:%d \n", notifyReqMsg.returnCode);
    return 1;
}


