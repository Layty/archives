一、创建HTC
1、使用htc_create创建。
2、HTC_TARGET *target表示htc中的target设备。
3、INIT_HTC_PACKET_QUEUE(&target->ControlBufferTXFreeList)，在host中，向target发送的空闲控制buffer队列。
4、HTC_PACKET_CONTAINER_ALLOCATION是32，表示最多可存储32个HTC包，存储在target->pHTCPacketStructPool中。
		for (i = 0; i < HTC_PACKET_CONTAINER_ALLOCATION; i++) {
			HTC_PACKET *pPacket =
				(HTC_PACKET *) qdf_mem_malloc(sizeof(HTC_PACKET));
			if (pPacket != NULL) {
				qdf_mem_zero(pPacket, sizeof(HTC_PACKET));
				free_htc_packet_container(target, pPacket);
			}
		}
		
		
void free_htc_packet_container(HTC_TARGET *target, HTC_PACKET *pPacket)
{
	LOCK_HTC_RX(target);

	if (NULL == target->pHTCPacketStructPool) {
		target->pHTCPacketStructPool = pPacket;
		pPacket->ListLink.pNext = NULL;
	} else {
		pPacket->ListLink.pNext =
			(DL_LIST *) target->pHTCPacketStructPool;
		target->pHTCPacketStructPool = pPacket;
	}

	UNLOCK_HTC_RX(target);
}
5、NUM_CONTROL_TX_BUFFERS是2，表示最多可存储2个控制HTC包，存储在target->ControlBufferTXFreeList中。
		for (i = 0; i < NUM_CONTROL_TX_BUFFERS; i++) {
			pPacket = build_htc_tx_ctrl_packet();
			if (NULL == pPacket) {
				break;
			}
			htc_free_control_tx_packet(target, pPacket);
		}
	
二、发送htc控制包
1、使用htc_alloc_control_tx_packet取出一个HTC_PACKET。
HTC_PACKET *pSendPacket = NULL;
pSendPacket = htc_alloc_control_tx_packet(target);

HTC_PACKET *htc_alloc_control_tx_packet(HTC_TARGET *target)
{
#ifdef TODO_FIXME
	HTC_PACKET *pPacket;

	LOCK_HTC(target);
	pPacket = htc_packet_dequeue(&target->ControlBufferTXFreeList);
	UNLOCK_HTC(target);

	return pPacket;
#else
	return build_htc_tx_ctrl_packet(target->osdev);
#endif
}

2、使用GET_HTC_PACKET_NET_BUF_CONTEXT宏从HTC_PACKET得到qdf_nbuf_t结构的变量pNetBufContext，qdf_nbuf_t相当于skb结构。
netbuf = (qdf_nbuf_t)GET_HTC_PACKET_NET_BUF_CONTEXT(pSendPacket)
		
3、为skb结构中的数据buffer分配空间。
qdf_nbuf_put_tail(netbuf, length)

4、使用qdf_nbuf_data宏从qdf_nbuf_t结构中得到数据buffer存储区。
pConnectMsg = (HTC_CONNECT_SERVICE_MSG *) qdf_nbuf_data(netbuf);

5、pConnectMsg为数据buffer，填充相应数据域。
HTC_SET_FIELD(pConnectMsg, HTC_CONNECT_SERVICE_MSG,
		  MESSAGEID, HTC_MSG_CONNECT_SERVICE_ID);
HTC_SET_FIELD(pConnectMsg, HTC_CONNECT_SERVICE_MSG,
		  SERVICE_ID, pConnectReq->service_id);
HTC_SET_FIELD(pConnectMsg, HTC_CONNECT_SERVICE_MSG,
		  CONNECTIONFLAGS, conn_flags);

6、使用htc_send_pkt发送htc数据。
status = htc_send_pkt((HTC_HANDLE) target, pSendPacket);

三、释放htc包
1、当发送函数调用完成后，会通过EpCallBacks.EpTxComplete来进行完成回收动作。
htc_send_pkts_multiple
{
	htc_try_send(target, pEndpoint, pPktQueue);
	。。。。
	do_send_completion(pEndpoint, pPktQueue);
}

	pEndpoint->EpCallBacks.EpTxComplete(pEndpoint->
						EpCallBacks.
						pContext,
						pPacket);
						
2、htc包释放时，需要先加入到pdev->htt_htc_pkt_freelist列表，延迟统一释放。
void
htt_h2t_send_complete(void *context, HTC_PACKET *htc_pkt)
{
	。。。。。。
    /* free the htt_htc_pkt / HTC_PACKET object */
    htt_htc_pkt_free(pdev, htt_pkt);
}

htt_htc_pkt_free(struct htt_pdev_t *pdev, struct htt_htc_pkt *pkt)
{
    struct htt_htc_pkt_union *u_pkt = (struct htt_htc_pkt_union *) pkt;

    HTT_TX_MUTEX_ACQUIRE(&pdev->htt_tx_mutex);
    u_pkt->u.next = pdev->htt_htc_pkt_freelist;
    pdev->htt_htc_pkt_freelist = u_pkt;
    HTT_TX_MUTEX_RELEASE(&pdev->htt_tx_mutex);
}