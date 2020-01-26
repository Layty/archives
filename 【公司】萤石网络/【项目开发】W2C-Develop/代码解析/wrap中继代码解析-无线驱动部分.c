/*
 * flags to be passed to ieee80211_vap_create function .
 */
#define IEEE80211_CLONE_BSSID           0x0001  /* allocate unique mac/bssid */
#define IEEE80211_CLONE_NOBEACONS       0x0002  /* don't setup beacon timers */
#define IEEE80211_CLONE_WDS             0x0004  /* enable WDS processing */
#define IEEE80211_CLONE_WDSLEGACY       0x0008  /* legacy WDS operation */
#define IEEE80211_PRIMARY_VAP           0x0010  /* primary vap */
#define IEEE80211_P2PDEV_VAP            0x0020  /* p2pdev vap */
#define IEEE80211_P2PGO_VAP             0x0040  /* p2p-go vap */
#define IEEE80211_P2PCLI_VAP            0x0080  /* p2p-client vap */
#define IEEE80211_CLONE_MACADDR         0x0100  /* create vap w/ specified mac/bssid */
#define IEEE80211_CLONE_MATADDR         0x0200  /* create vap w/ specified MAT addr */
#define IEEE80211_WRAP_VAP              0x0400  /* wireless repeater ap vap */
#define IEEE80211_WRAP_WIRED_STA        0x0800  /* wired qwrap sta */
#define IEEE80211_SPECIAL_VAP           0x1000  /* special vap mode */
#define IEEE80211_MESH_VAP              0x2000  /* mesh vap mode */
#define IEEE80211_SMART_MONITOR_VAP     0x4000  /* smart monitor vap */
#if ATH_NON_BEACON_AP
#define IEEE80211_NON_BEACON_AP         0x8000  /* non-beaconing AP */
#endif
#define IEEE80211_LP_IOT_VAP            0x10000 /* AP mode for IOT clients */

/** values for vdev_type */
#define WMI_HOST_VDEV_TYPE_AP	0x1
#define WMI_HOST_VDEV_TYPE_STA	0x2
#define WMI_HOST_VDEV_TYPE_IBSS	0x3
#define WMI_HOST_VDEV_TYPE_MONITOR	0x4

/** values for vdev_subtype */
#define WMI_HOST_VDEV_SUBTYPE_P2P_DEVICE 0x1
#define WMI_HOST_VDEV_SUBTYPE_P2P_CLIENT 0x2
#define WMI_HOST_VDEV_SUBTYPE_P2P_GO	0x3
#define WMI_HOST_VDEV_SUBTYPE_PROXY_STA  0x4
#define WMI_HOST_VDEV_SUBTYPE_MESH	0x5

#define WMI_HOST_MGMT_TID	17
/* Disable aging & learning */
#define WMI_HOST_WDS_FLAG_STATIC	0x1

======================================================  发送 ====================================================
/*
vap->iv_wrap_mat_tx = ol_if_wrap_mat_tx;或 vap->iv_wrap_mat_tx = ath_wrap_mat_tx;
*/
#define OL_WRAP_TX_PROCESS(_osdev, _vap, _skb) \
{ \
    if (qdf_unlikely(wlan_is_mpsta(_vap)))   { \
        wrap_tx_bridge (&_vap, _osdev , _skb); \
        if (*(_skb) == NULL) {\
            goto bad;\
        }\
        if (wlan_is_psta(_vap))   { \
            vap->iv_wrap_mat_tx(_vap, (wbuf_t)*_skb); \
        } \
    } \
    if (!((*_osdev)->is_up)) {\
        goto bad; \
   } \
}

/*
 使用报文原地址,从wrap桥中找到对应vap,返回
*/
inline void wrap_tx_bridge ( wlan_if_t *vap , osif_dev **osdev,  struct sk_buff **skb) {

    /* Assuming native wifi or raw mode is not
     * enabled in beeliner, to be revisted later
     */
    struct ether_header *eh = (struct ether_header *) ((*skb)->data);
    wlan_if_t prev_vap = *vap;
    osif_dev *tx_osdev;
    osif_dev *prev_osdev = *osdev;
#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
    void *nss_wifiol_ctx = prev_osdev->nss_wifiol_ctx;
#endif

	/*
	 使用报文原地址,从wrap桥中找到对应vap
	*/
    /* Mpsta vap here, find the correct tx vap from the wrap common based on src address */
    tx_osdev = osif_wrap_wdev_find(&prev_vap->iv_ic->ic_wrap_com->wc_devt,eh->ether_shost);
    if (tx_osdev) {
        if(qdf_unlikely((IEEE80211_IS_MULTICAST(eh->ether_dhost) || IEEE80211_IS_BROADCAST(eh->ether_dhost)))) {
            *skb = skb_unshare(*skb, GFP_ATOMIC);
        }
        /* since tx vap gets changed , handle tx vap synchorization */
#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
        if (!nss_wifiol_ctx)
#endif
        {
            OSIF_VAP_TX_UNLOCK(prev_osdev);
        }
		/*将找到的vap与osdev返回,vap与osdev是传入参数*/
        *vap = tx_osdev->os_if;
        *osdev  = tx_osdev;

#ifdef QCA_NSS_WIFI_OFFLOAD_SUPPORT
        if (!nss_wifiol_ctx)
#endif
        {
            OSIF_VAP_TX_LOCK(*osdev);
        }
    }
}

/*
主要工作:
1、要将报文中的源地址修改成代理vap中的地址，如下代码
IEEE80211_ADDR_COPY(src_mac,out_vap->iv_myaddr);
除了以太网协议外，三层的arp协议，四层的dhcp协议，都需要修改mac地址。
*/
/**
 * @brief WRAP MAT function for transmit.
 *
 * @param out_vap Output VAP.
 * @param buf
 *
 * @return 0 On success.
 * @return -ve On failure.
 */
int ol_if_wrap_mat_tx(struct ieee80211vap *out_vap, wbuf_t buf)
{
    struct ol_ath_vap_net80211 *avn;
    struct ol_ath_softc_net80211 *scn;
    struct ieee80211com *ic;
    struct ether_header *eh;
    uint16_t ether_type;
    int contig_len = sizeof(struct ether_header);
    int pktlen = wbuf_get_pktlen(buf);
    uint8_t *src_mac,*des_mac,*p,ismcast;
    uint8_t *arp_smac = NULL;
    uint8_t *arp_dmac = NULL;
    struct eth_arphdr *parp = NULL;

	if(!(OL_ATH_VAP_NET80211(out_vap)->av_use_mat))
		return 0;

    if ((pktlen < contig_len))
        return -EINVAL;

    eh = (struct ether_header *)(wbuf_header(buf));
    p = (uint8_t*)(eh+1);

    ether_type = eh->ether_type;
    src_mac = eh->ether_shost;
    des_mac = eh->ether_dhost;
    ismcast = IEEE80211_IS_MULTICAST(des_mac);

#ifdef ATH_MAT_TEST
	printk(KERN_ERR "%s: src %s type 0x%x",__func__,ether_sprintf(src_mac),ether_type);
	printk(KERN_ERR "des %s\n",ether_sprintf(des_mac));
#endif

	if (ether_type == htons(ETH_P_PAE))
			return 0;

	if (ether_type == htons(ETHERTYPE_ARP)) {
			parp = (struct eth_arphdr *)p ;
			contig_len += sizeof(struct eth_arphdr);

    		if ((pktlen < contig_len))
        		return -EINVAL;

			if(parp->ar_hln == ETH_ALEN && parp->ar_pro == htons(ETH_P_IP)) {
				arp_smac = parp->ar_sha;
				arp_dmac = parp->ar_tha;
			} else {
				parp = NULL;
			}
	}

	avn = OL_ATH_VAP_NET80211(out_vap);
	ic = out_vap->iv_ic;
	scn = OL_ATH_SOFTC_NET80211(ic);
	if(parp){
            if (parp->ar_op == htons(ARPOP_REQUEST) ||  parp->ar_op == htons(ARPOP_REPLY)) {
                IEEE80211_ADDR_COPY(arp_smac,out_vap->iv_myaddr);
            }
	}else if (ether_type == htons(ETHERTYPE_IP)) {
		struct iphdr *p_ip = (struct iphdr *)(p);
		int16_t ip_hlen;

		contig_len += sizeof(struct iphdr);
		if ((pktlen < contig_len))
    		return -EINVAL;

		ip_hlen = p_ip->ihl * 4;

		/* If Proto is UDP */
		if (p_ip->protocol == IPPROTO_UDP) {
			struct udphdr *p_udp = (struct udphdr *) (((uint8_t *)p_ip) + ip_hlen);
			uint16_t udplen;


			contig_len += sizeof(struct udphdr);
			if ((pktlen < contig_len))
    				return -EINVAL;

			udplen = p_ip->tot_len - (p_ip->ihl * 4);
#ifdef ATH_DEBUG_MAT
			printk(KERN_ERR "%s:%d sport %d dport %d\n",__func__,__LINE__,p_udp->source,p_udp->dest);
#endif
			/*
			* DHCP request UDP Client SP = 68 (bootpc), DP = 67 (bootps).
			*/
			if ((p_udp->dest == htons(67))) {
				struct dhcp_packet *p_dhcp = (struct dhcp_packet *)(((u_int8_t *)p_udp)+sizeof(struct udphdr));
#if 0
				struct dhcp_option *option = (struct dhcp_option *)p_dhcp->options;
				u_int8_t *value;
#endif
				contig_len += sizeof(struct dhcp_packet);
				if ((pktlen < contig_len))
    					return -EINVAL;

#ifdef ATH_DEBUG_MAT
				printk(KERN_ERR "%s:%d sport %d dport %d len %d chaddr %s\n",__func__,__LINE__,p_udp->source,p_udp->dest,udplen,ether_sprintf(p_dhcp->chaddr));
#endif
				if(p_dhcp->magic_cookie[0] == 0x63 && p_dhcp->magic_cookie[1] == 0x82 && p_dhcp->magic_cookie[2] == 0x53 && p_dhcp->magic_cookie[3] == 0x63) {
#ifdef ATH_DEBUG_MAT
					printk(" Found DHCP cookie .. \n");
#endif
					if (p_dhcp->op == 1) {
                                                /* dhcp REQ or DISCOVER*/

#ifdef BIG_ENDIAN_HOST
						if((p_dhcp->flags & 0x8000) == 0) {
							p_dhcp->flags |= 0x8000;
#else
						if((p_dhcp->flags & 0x0080) == 0) {
							p_dhcp->flags |= 0x0080;
#endif

#ifdef ATH_DEBUG_MAT
							printk(" Make the dhcp flag to broadcast ... \n");
#endif
							compute_udp_checksum((qdf_net_iphdr_t *)p_ip, (unsigned short *)p_udp);
						}
					}
				} /* Magic cookie for DHCP*/

#if 0
                                /* When MAC address is changed in DHCP payload, Apple devices are showing DHCP issue
                                   So, not doing MAT translation on DHCP payload and only making DHCP flag as broadcast.
                                   Refer IR-131533 */

				if (IEEE80211_ADDR_EQ(p_dhcp->chaddr,src_mac)) {
					/* replace the Client HW address with the VMA */
					IEEE80211_ADDR_COPY(p_dhcp->chaddr, out_vap->iv_myaddr);
					/*
					 * Since the packet was modified, do incremental checksum
					 * update for the UDP frame.
					 */
					p_udp->check = update_checksum_addr(p_udp->check,
									src_mac, out_vap->iv_myaddr);
				}
				/* replace the client HW address with the VMA in option field*/
				while (option->type != 0xFF) {
					/*Not an end option*/

					contig_len += (option->len + 2);
					if ((pktlen < contig_len))
						return -EINVAL;

					value = option->value;
					if(option->type == 0x3D) {
						/*client identifier option*/
						if(value[0] == 1) {
							/*Hw type is ethernet*/
							value++;
							if (IEEE80211_ADDR_EQ(value,src_mac)) {
								IEEE80211_ADDR_COPY(value, out_vap->iv_myaddr);
							}
							/*
							 * Since the packet was modified, do incremental checksum
							 * update for the UDP frame.
							 */
							p_udp->check = update_checksum_addr(p_udp->check,
											src_mac, out_vap->iv_myaddr);
						}
						break;
					} else {
						option = (struct dhcp_option *)(value + option->len);
					}
				}
#endif
			}
		}
	}else if (ether_type == htons(ETHERTYPE_IPV6)) {
		struct ipv6hdr *p_ip6 = (struct ipv6hdr *)(p);
        eth_icmp6_lladdr_t  *phwaddr;
        int change_packet = 1;

        contig_len += sizeof(struct ipv6hdr);
        if (pktlen < contig_len)
    		return -EINVAL;

        if (p_ip6->nexthdr == IPPROTO_ICMPV6) {
            struct icmp6hdr *p_icmp6 = (struct icmp6hdr *)(p_ip6 + 1);
            uint16_t icmp6len = p_ip6->payload_len;

            contig_len += sizeof(struct icmp6hdr);
            if (pktlen < contig_len)
    			return -EINVAL;

            /*
             * It seems that we only have to modify IPv6 packets being
             * sent by a Proxy STA. Both the solicitation and advertisement
             * packets have the STA's OMA. Flip that to the VMA.
             */
            switch (p_icmp6->icmp6_type) {
            	case NDISC_NEIGHBOUR_SOLICITATION:
            	case NDISC_NEIGHBOUR_ADVERTISEMENT:
                {
                    contig_len += 16 + sizeof(eth_icmp6_lladdr_t);
                    if (pktlen < contig_len)
    					return -EINVAL;

                    phwaddr = (eth_icmp6_lladdr_t *)((uint8_t *)(p_icmp6+1)+16);
                    IEEE80211_ADDR_COPY(phwaddr->addr, out_vap->iv_myaddr);
                    p_icmp6->icmp6_cksum = 0;
                    p_icmp6->icmp6_cksum = htons(checksum((uint16_t)IPPROTO_ICMPV6, icmp6len,
					   p_ip6->saddr.s6_addr, p_ip6->daddr.s6_addr,

				           16 /* IPv6 has 32 byte addresses */, (uint8_t *)p_icmp6));
                    break;
                }
            	case NDISC_ROUTER_SOLICITATION:
                {
                    contig_len += sizeof(eth_icmp6_lladdr_t);
                    if (pktlen < contig_len)
    					return -EINVAL;

                    /* replace the HW address with the VMA */
                    phwaddr = (eth_icmp6_lladdr_t *)((uint8_t *)(p_icmp6 + 1));
                    break;
                }
            	default:
                	change_packet = 0;
                break;
            }

            if (change_packet) {
                IEEE80211_ADDR_COPY(phwaddr->addr, out_vap->iv_myaddr);
                p_icmp6->icmp6_cksum = 0;
                p_icmp6->icmp6_cksum = htons(checksum((uint16_t)IPPROTO_ICMPV6, icmp6len,
                            p_ip6->saddr.s6_addr, p_ip6->daddr.s6_addr,
                            16 /* IPv6 has 32 byte addresses */, (uint8_t *)p_icmp6));
            }
        }
	}
	IEEE80211_ADDR_COPY(src_mac,out_vap->iv_myaddr);
	return 0;
}
