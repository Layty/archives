=============================================== 创建vap ===================================
/*
addr: 终端的mac地址

psta->oma表示终端的mac地址
psta->vma表示vap需要被设置的mac地址
*/
static psta_status_t
wrapd_psta_add(struct wrap_wifi_iface *aptr, const char *parent, char *child, const u8 *addr, u_int32_t flags)
{
    struct proxy_sta *psta = NULL;
    int res, ifname_num;
    int i;
    int fst_unused = -1;
#if PROXY_NOACK_WAR
    int status = -1;
    int mac_reserve_try=0;
    u_int8_t temp=0x0, temp1=0x0;
#endif
    char cmd[WRAP_MAX_CMD_LEN] = {0};
    struct ctrl_conn *hapd = NULL;

    if(child)
        hapd = global_ifname_to_ctrl_conn(aptr->wptr, child);

    if(addr == NULL) {
        wrapd_printf("IWEVREGISTERED with NULL addr");
        return PROXY_STA_CREATION_ERROR;
    }

    if((os_strcmp(parent, aptr->wifi_ifname)) != 0){
        wrapd_printf("QWRAP not enabled for this radio");
        return PROXY_STA_CREATION_ERROR;
    }

    if(addr[0] & 0x01){
        wrapd_printf("Multicast mac address should not be given for psta");
        return PROXY_STA_CREATION_ERROR;
    }
    wrapd_printf("addr(%02x:%02x:%02x:%02x:%02x:%02x)",
        addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);

    i = 0;
    while (i < WRAP_MAX_PSTA_NUM) {
       if((aptr->psta[i].added) && (os_memcmp(addr, aptr->psta[i].oma, IEEE80211_ADDR_LEN) == 0)) {
            wrapd_printf("oma already exists");
            return PROXY_STA_CREATION_ERROR;
        } else {
            if ((fst_unused == -1) && (0 == aptr->psta[i].added)) {
                if ((flags & WRAPD_PSTA_FLAG_MAT) || (0 == aptr->psta[i].vma_loaded))
                    fst_unused = i;
            }
        }
        i ++;
    }

    if (fst_unused == -1) {
        if (flags & WRAPD_PSTA_FLAG_WIRED) {
	    send_message_to_brd(0,addr,1);
        }
	global_vap_limit_flag=1;
        wrapd_printf("proxySTA num is up to limit");
        return PROXY_STA_LIMIT_EXCEEDED;
    }

    psta = &aptr->psta[fst_unused];
    psta->added = 1;

	/* 将终端的mac地址存入psta->oma */
    os_memcpy(psta->oma, addr, IEEE80211_ADDR_LEN);

	/* 按照一定规则修改mac地址,存入psta->vma */
    if (1 != psta->vma_loaded)
        os_memcpy(psta->vma, addr, IEEE80211_ADDR_LEN);

    if (flags & WRAPD_PSTA_FLAG_MAT) {
        psta->flags |= WRAPD_PSTA_FLAG_MAT;
        if (1 != psta->vma_loaded)
        {
            psta->vma[0] |= 0x02;
            if(hapd) {
                psta->vma[1] = hapd->ap_mac_addr[4];
                psta->vma[2] = hapd->ap_mac_addr[5];
            }
            else
            {
                psta->vma[1] = aptr->wifi_dev_addr[4];
                psta->vma[2] = aptr->wifi_dev_addr[5];
            }
        }
    }

    /* Parent AP interface of this client */
    psta->parent_ap = hapd;

#if PROXY_NOACK_WAR
    /* reserve proxy mac address from target */
    if (aptr->proxy_noack_war) {
        wrapd_check_proxy_macaddr_ok(aptr, psta->vma, &status);
        if(!(flags & WRAPD_PSTA_FLAG_MAT) && mac_reserve_try == 0 && status!=0) {
            psta->vma[0] |= 0x02;
            wrapd_check_proxy_macaddr_ok(aptr, psta->vma, &status);
            psta->flags |= WRAPD_PSTA_FLAG_MAT;
            mac_reserve_try++;
        }
        /* 3rd to 8th bit are used to derive incremental mac address */
        temp  = psta->vma[0] >> 2;
        temp1 = psta->vma[0] & 0x3;
        while (mac_reserve_try < 64 && status!=0) {
            temp = temp + 0x1;
            psta->vma[0] = (temp << 2 | temp1) & 0xff;
            wrapd_check_proxy_macaddr_ok(aptr, psta->vma, &status);
            mac_reserve_try++;
		if(status==0) {
                break;
            }
        }
    }
#endif

    if (flags & WRAPD_PSTA_FLAG_WIRED)
        psta->flags |= WRAPD_PSTA_FLAG_WIRED;

    if (flags & WRAPD_PSTA_FLAG_OPEN)
        psta->flags |= WRAPD_PSTA_FLAG_OPEN;

    ifname_num = fst_unused + WRAP_PSTA_START_OFF;

	/* 创建ProxySTA VAP */
    //create ProxySTA VAP
    res = wrapd_vap_create(aptr, psta, ifname_num, parent);
    if (res != 0){
        wrapd_printf("Fail to create ProxySTA VAP");
        psta->added = 0;
        psta->flags = 0;
        if (flags & WRAPD_PSTA_FLAG_WIRED) {
            send_message_to_brd(0,addr,1);
        }
	if(res==5)
	{
		global_vap_limit_flag=1;
	}
        os_memset(psta->oma, 0, IEEE80211_ADDR_LEN);
        return PROXY_STA_CREATION_ERROR;
    }
    else{
        if (flags & WRAPD_PSTA_FLAG_WIRED) {
	    send_message_to_brd(1,addr,1);
        }
    }

	/* 为ProxySTA VAP开启shortgi */
    os_snprintf(cmd, sizeof(cmd),"iwpriv ath%d%d shortgi 1",aptr->wifi_device_num, ifname_num);
    wrapd_printf("%s Setting command: %s \n", __func__, cmd);
    system(cmd);

    os_strlcpy(psta->parent, parent, IFNAMSIZ);
    if (child)
        os_strlcpy(psta->child, child, IFNAMSIZ);
    wrapd_printf("proxySTA %d is added from %s", fst_unused, aptr->wifi_ifname);

    if (1 == aptr->mpsta_conn) {
        if ((flags & WRAPD_PSTA_FLAG_OPEN) || (0 == wrapg->do_timer) ) {
            wrapd_psta_conn(aptr, fst_unused);

        } else {
            if((0 == wrapg->in_timer) ) {
                eloop_register_timeout(1, 0, wrapd_conn_timer, aptr, NULL);
                wrapg->in_timer = 1;
            }
        }
    }
    return PROXY_STA_SUCCESS;
}

struct ieee80211_clone_params {
        char icp_name[IFNAMSIZ];                /* device name */
        u_int16_t icp_opmode;                   /* operating mode */
        u_int16_t icp_flags;                    /* see below */
};

/*
1、调用ioctl创建vap
*/
static int
wrapd_vap_create(struct wrap_wifi_iface *aptr, struct proxy_sta *psta, int ifname_num, const char *parent)
{
    struct ieee80211_clone_params cp;
    struct ifreq ifr;
	int res,i;

    os_memset(&ifr, 0, sizeof(ifr));
    os_memset(&cp, 0, sizeof(cp));

	/* 指定vap的接口名,例如ath03 0表示WiFi接口,3表示第几个接口*/
    res = os_snprintf(cp.icp_name, sizeof(cp.icp_name), "ath%d%d", aptr->wifi_device_num, ifname_num);
    if (res < 0 || res >= sizeof(cp.icp_name)) {
        wrapd_printf("os_snprintf err");
        return -1;
    }
    cp.icp_name[IFNAMSIZ - 1] = '\0';
	/* 指定vap的模式为sta模式*/
    cp.icp_opmode = IEEE80211_M_STA;
    cp.icp_flags = 0;

    os_strlcpy(ifr.ifr_name, parent, IFNAMSIZ);

	/* 无线接入终端常常打上WRAPD_PSTA_FLAG_MAT标签,IEEE80211_CLONE_MACADDR表示vap的地址依靠传入指定 */
    if (psta->flags & WRAPD_PSTA_FLAG_MAT) {
        os_memcpy(cp.icp_bssid, psta->vma, IEEE80211_ADDR_LEN);
        os_memcpy(cp.icp_mataddr, psta->oma, IEEE80211_ADDR_LEN);
        cp.icp_flags |= IEEE80211_CLONE_MACADDR;
        cp.icp_flags |= IEEE80211_CLONE_MATADDR;
    } else {
        os_memcpy(cp.icp_bssid, psta->oma, IEEE80211_ADDR_LEN);
        cp.icp_flags |= IEEE80211_CLONE_MACADDR;
    }

    if(psta->flags & WRAPD_PSTA_FLAG_WIRED) {
		/* 连接的是有线客户端 */
        cp.icp_flags |= IEEE80211_WRAP_WIRED_STA;
    }

    ifr.ifr_data = (void *) &cp;

/*
    wrapd_printf("cp.icp_name(%s), cp.icp_opmode(%d), cp.icp_flags(0x%04x), ifr.ifr_name(%s)",
        cp.icp_name, cp.icp_opmode, cp.icp_flags, ifr.ifr_name);
    wrapd_printf("cp.icp_bssid(%02x:%02x:%02x:%02x:%02x:%02x) icp_mataddr(%02x:%02x:%02x:%02x:%02x:%02x)",
        cp.icp_bssid[0],cp.icp_bssid[1],cp.icp_bssid[2],cp.icp_bssid[3],cp.icp_bssid[4],cp.icp_bssid[5],
        cp.icp_mataddr[0],cp.icp_mataddr[1],cp.icp_mataddr[2],cp.icp_mataddr[3],cp.icp_mataddr[4],cp.icp_mataddr[5]);
*/

	/* 通过ioctl的SIOC80211IFCREATE命令,通知无线驱动创建VAP */
	if ((i = ioctl(wrapg->ioctl_sock, SIOC80211IFCREATE, &ifr)) != 0) {
        wrapd_printf("ioctl(SIOC80211IFCREATE)");
        return i;
    }
    return 0;
}
