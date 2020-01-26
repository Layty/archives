/*============================================= hostapd使用的通道接口 ====================================*/
drv->ioctl_sock = socket(PF_INET, SOCK_DGRAM, 0);

drv->sock_xmit = l2_packet_init(drv->iface, NULL, ETH_P_EAPOL,
				handle_read, drv, 1);

if (params->bridge[0]) {
	wpa_printf(MSG_DEBUG, "Configure bridge %s for EAPOL traffic.",
		   params->bridge[0]);
	drv->sock_recv = l2_packet_init(params->bridge[0], NULL,
					ETH_P_EAPOL, handle_read, drv,
					1);
	if (drv->sock_recv == NULL)
		goto bad;
} else if (linux_br_get(brname, drv->iface) == 0) {
	wpa_printf(MSG_DEBUG, "Interface in bridge %s; configure for "
		   "EAPOL receive", brname);
	drv->sock_recv = l2_packet_init(brname, NULL, ETH_P_EAPOL,
					handle_read, drv, 1);
	if (drv->sock_recv == NULL)
		goto bad;
} else
	drv->sock_recv = drv->sock_xmit;	
	
static int atheros_receive_pkt(struct atheros_driver_data *drv)
#if defined(CONFIG_WPS) || defined(CONFIG_IEEE80211R)
	drv->sock_raw = l2_packet_init(drv->iface, NULL, ETH_P_80211_RAW,
				       atheros_raw_receive, drv, 1);
	if (drv->sock_raw == NULL)
		return -1;
#endif /* CONFIG_WPS || CONFIG_IEEE80211R */


static int atheros_wireless_event_init(struct atheros_driver_data *drv)
cfg->ctx = drv;
cfg->newlink_cb = atheros_wireless_event_rtm_newlink;
drv->netlink = netlink_init(cfg);



/*===================================== netlink定义 ============================*/
struct netlink_data * netlink_init(struct netlink_config *cfg)
{
	struct netlink_data *netlink;
	struct sockaddr_nl local;

	netlink = os_zalloc(sizeof(*netlink));
	if (netlink == NULL)
		return NULL;

	netlink->sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (netlink->sock < 0) {
		wpa_printf(MSG_ERROR, "netlink: Failed to open netlink "
			   "socket: %s", strerror(errno));
		netlink_deinit(netlink);
		return NULL;
	}

	os_memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_groups = RTMGRP_LINK;
	if (bind(netlink->sock, (struct sockaddr *) &local, sizeof(local)) < 0)
	{
		wpa_printf(MSG_ERROR, "netlink: Failed to bind netlink "
			   "socket: %s", strerror(errno));
		netlink_deinit(netlink);
		return NULL;
	}

	eloop_register_read_sock(netlink->sock, netlink_receive, netlink,
				 NULL);

	netlink->cfg = cfg;

	return netlink;
}


/*================================== raw socket定义 ==============================*/
struct l2_packet_data * l2_packet_init(
	const char *ifname, const u8 *own_addr, unsigned short protocol,
	void (*rx_callback)(void *ctx, const u8 *src_addr,
			    const u8 *buf, size_t len),
	void *rx_callback_ctx, int l2_hdr)
{
	struct l2_packet_data *l2;
	struct ifreq ifr;
	struct sockaddr_ll ll;

	l2 = os_zalloc(sizeof(struct l2_packet_data));
	if (l2 == NULL)
		return NULL;
	os_strlcpy(l2->ifname, ifname, sizeof(l2->ifname));
	l2->rx_callback = rx_callback;
	l2->rx_callback_ctx = rx_callback_ctx;
	l2->l2_hdr = l2_hdr;
#ifndef CONFIG_NO_LINUX_PACKET_SOCKET_WAR
	l2->fd_br_rx = -1;
#endif /* CONFIG_NO_LINUX_PACKET_SOCKET_WAR */

	l2->fd = socket(PF_PACKET, l2_hdr ? SOCK_RAW : SOCK_DGRAM,
			htons(protocol));
	if (l2->fd < 0) {
		wpa_printf(MSG_ERROR, "%s: socket(PF_PACKET): %s",
			   __func__, strerror(errno));
		os_free(l2);
		return NULL;
	}
	os_memset(&ifr, 0, sizeof(ifr));
	os_strlcpy(ifr.ifr_name, l2->ifname, sizeof(ifr.ifr_name));
	if (ioctl(l2->fd, SIOCGIFINDEX, &ifr) < 0) {
		wpa_printf(MSG_ERROR, "%s: ioctl[SIOCGIFINDEX]: %s",
			   __func__, strerror(errno));
		close(l2->fd);
		os_free(l2);
		return NULL;
	}
	l2->ifindex = ifr.ifr_ifindex;

	os_memset(&ll, 0, sizeof(ll));
	ll.sll_family = PF_PACKET;
	ll.sll_ifindex = ifr.ifr_ifindex;
	ll.sll_protocol = htons(protocol);
	if (bind(l2->fd, (struct sockaddr *) &ll, sizeof(ll)) < 0) {
		wpa_printf(MSG_ERROR, "%s: bind[PF_PACKET]: %s",
			   __func__, strerror(errno));
		close(l2->fd);
		os_free(l2);
		return NULL;
	}

	if (ioctl(l2->fd, SIOCGIFHWADDR, &ifr) < 0) {
		wpa_printf(MSG_ERROR, "%s: ioctl[SIOCGIFHWADDR]: %s",
			   __func__, strerror(errno));
		close(l2->fd);
		os_free(l2);
		return NULL;
	}
	os_memcpy(l2->own_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

	eloop_register_read_sock(l2->fd, l2_packet_receive, l2, NULL);

	return l2;
}