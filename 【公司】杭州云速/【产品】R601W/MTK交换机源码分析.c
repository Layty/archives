0、R601W交换模块内核配置
#
# Ralink Module
#
CONFIG_RALINK_RDM=y
CONFIG_RAETH=m
# CONFIG_RA_NETWORK_TASKLET_BH is not set
# CONFIG_RA_NETWORK_WORKQUEUE_BH is not set
CONFIG_RAETH_NAPI=y
# CONFIG_RAETH_SKB_RECYCLE_2K is not set
# CONFIG_RAETH_SPECIAL_TAG is not set
CONFIG_RAETH_CHECKSUM_OFFLOAD=y
# CONFIG_RAETH_LRO is not set
CONFIG_RT_3052_ESW=y
CONFIG_LAN_WAN_SUPPORT=y
# CONFIG_ETH_MEMORY_OPTIMIZATION is not set
# CONFIG_ETH_ONE_PORT_ONLY is not set
CONFIG_WAN_AT_P4=y
# CONFIG_WAN_AT_P0 is not set
CONFIG_ESW_DOUBLE_VLAN_TAG=y


/**
重点函数
1、中断函数
# cat /proc/interrupts 
           CPU0       
  3:     392473          Ralink  eth2
  4:     676723          Ralink  ra1
  5:    1731844          Ralink  timer
  6:          0          Ralink  ralink_gpio
 17:          5          Ralink  Ralink_ESW
 18:       1691          Ralink  ehci_hcd:usb1, ohci_hcd:usb2
 20:         78          Ralink  serial
 
可以看到关于交换模块有两个终端，一个是irq3，用于交换机收包；另一个是irq17，用与link状态改变。

err = request_irq( dev->irq, ei_interrupt, IRQF_DISABLED, dev->name, dev);	// try to fix irq in open
若设置了IRQF_DISABLED（老版本中的SA_INTERRUPT，本版zhon已经不支持了），则表示中断处理程序是快速处理程序，快速处理程序被调用时屏蔽所有中断，慢速处理程序不屏蔽；若设置了IRQF_SHARED（老版本中的SA_SHIRQ），则表示多个设备共享中断，若设置了IRQF_SAMPLE_RANDOM（老版本中的SA_SAMPLE_RANDOM），表示对系统熵有贡献，对系统获取随机数有好处。（这几个flag是可以通过或的方式同时使用的）

err = request_irq(SURFBOARDINT_ESW, esw_interrupt, IRQF_DISABLED, "Ralink_ESW", dev);
*/

1、创建虚拟接口Vthx、wanbr、wangw
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
static const struct net_device_ops VirtualIF_netdev_ops_vthx = {
        .ndo_open               = VirtualIF_open_vthx,
        .ndo_stop               = VirtualIF_close_vthx,
        .ndo_start_xmit         = VirtualIFSendPackets_vthx,
        .ndo_get_stats          = VirtualIF_get_stats_vthx,
        .ndo_set_mac_address    = ei_set_mac2_addr_vthx,
        //.ndo_change_mtu         = ei_change_mtu_vthx,  //huangcd-tobefix-newvth
        .ndo_do_ioctl           = VirtualIF_ioctl_vthx,
        //.ndo_validate_addr      = eth_validate_addr,       //huangcd-tobefix-newvth
};
#endif


// Register pseudo interface
void RAETH_Init_PSEUDO_vthx(pEND_DEVICE pAd, struct net_device *net_dev)
{
    int index = 0;
    struct net_device *dev = NULL;
    PSEUDO_ADAPTER_VTHX *pPseudoAd = NULL;
    int i = 0;
    struct sockaddr addr;
    unsigned char zero1[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    unsigned char zero2[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	char ifname[IFNAMSIZ] = {0};	

    for (index = 0; index < MAX_PSEUDO_ENTRY_VTHX; index++) 
	{
        dev = alloc_etherdev(sizeof(PSEUDO_ADAPTER_VTHX));

        if (4 == index)
        {	
            sprintf(ifname, "%s", DEV_NAME_WANGW);

            /*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.24*/
            /*BEGIN:Modified by huangcd for mantis0001364(wanqos_merge2one_qdisc),2015.02.11*/
            nd_wangw = dev;
            /*END:  Modified by huangcd for mantis0001364(wanqos_merge2one_qdisc),2015.02.11*/
            /*END:  Sync from T5214EWL-OM(RT5350),2015.06.24*/            
        }
        else if(5 == index)
        {
            sprintf(ifname, "%s", DEV_NAME_WANBR);

            /*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.24*/            
			/*BEGIN:Modified by huangcd for mantis0001364(wanqos_merge2one_qdisc),2015.02.11*/
			nd_wanbr = dev;
			/*END:  Modified by huangcd for mantis0001364(wanqos_merge2one_qdisc),2015.02.11*/
            /*END:  Sync from T5214EWL-OM(RT5350),2015.06.24*/            
        }
        else
        {
            sprintf(ifname, "%s%d", DEV_NAME_VTHX, index);		    
        }
        strcpy(dev->name, ifname);

    	//Get mac0 address from flash
        #ifdef RA_MTD_RW_BY_NUM
    	i = ra_mtd_read(2, GMAC0_OFFSET, 6, addr.sa_data);
        #else
    	i = ra_mtd_read_nm("Factory", GMAC0_OFFSET, 6, addr.sa_data);
        #endif		

    	//If reading mtd failed or mac0 is empty, generate a mac address        
    	if (i < 0 || ((memcmp(addr.sa_data, zero1, 6) == 0) || (addr.sa_data[0] & 0x1)) || 
    	    (memcmp(addr.sa_data, zero2, 6) == 0)) {
    		unsigned char mac_addr01234[5] = {0x00, 0x0C, 0x43, 0x28, 0x80};
    		net_srandom(jiffies);
    		memcpy(addr.sa_data, mac_addr01234, 5);
    		addr.sa_data[5] = net_random()&0xFF;
    	}

		ei_set_mac2_addr_vthx(dev, &addr);
		ether_setup(dev);
		pPseudoAd = netdev_priv(dev);

		if ((4 == index) || (5 == index))
		{
    		pPseudoAd->vlan_id = 4095;
		}	
		else
		{
    		pPseudoAd->vlan_id = 4089 + index;			
		}

		pPseudoAd->PseudoDev = dev;
		pPseudoAd->RaethDev = net_dev;
		virtif_setup_statistics_vthx(pPseudoAd);

		pAd->vth[index] = dev;

		#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
		dev->netdev_ops		= &VirtualIF_netdev_ops_vthx;
		#else
		dev->hard_start_xmit = VirtualIFSendPackets_vthx;
		dev->stop            = VirtualIF_close_vthx;
		dev->open            = VirtualIF_open_vthx;
		dev->do_ioctl        = VirtualIF_ioctl_vthx;
		dev->set_mac_address = ei_set_mac2_addr_vthx;
		dev->get_stats       = VirtualIF_get_stats_vthx;
		//dev->change_mtu      = ei_change_mtu;  //huangcd-tobefix-newvth
		dev->mtu             = 1500;
		#endif

		// Register this device
		register_netdevice(dev);
    }

    /*BEGIN:Modified by huangcd for mantis0001508(l2fp_opt),2015.02.28*/
    l2fp_en_init_proc();
    l2fp_t1_init_proc();
    l2fp_t2_init_proc();    
    l2fp_dump_init_proc();    
    l2fp_dbgen_init_proc();

    return;
    /*END:  Modified by huangcd for mantis0001508(l2fp_opt),2015.02.28*/

}

2、交换机模块初始化

typedef struct end_device
{

    unsigned int        tx_cpu_owner_idx0;
    unsigned int        rx_cpu_owner_idx0;
    unsigned int        fe_int_status;
    unsigned int        tx_full; 
    
#if !defined (CONFIG_RAETH_QDMA)
    unsigned int	phy_tx_ring0;
#else
    /* QDMA Tx  PTR */
    struct sk_buff *free_skb[NUM_TX_DESC];
    unsigned int tx_dma_ptr;
    unsigned int tx_cpu_ptr;
    unsigned int free_txd_num;
    unsigned int free_txd_head;
    unsigned int free_txd_tail;	
    struct QDMA_txdesc *txd_pool;
    dma_addr_t phy_txd_pool;
    unsigned int txd_pool_info[NUM_TX_DESC];
    struct QDMA_txdesc *free_head;
    unsigned int phy_free_head;
    unsigned int *free_page_head;
    unsigned int phy_free_page_head;
    struct PDMA_rxdesc *qrx_ring;
    unsigned int phy_qrx_ring;
#endif

    unsigned int	phy_rx_ring0, phy_rx_ring1;

#if defined (CONFIG_RALINK_RT3052) || defined (CONFIG_RALINK_RT3352) || \
    defined (CONFIG_RALINK_RT5350) || defined (CONFIG_RALINK_RT6855) || \
    defined(CONFIG_RALINK_RT6855A) || defined (CONFIG_RALINK_MT7620) || \
    defined(CONFIG_RALINK_MT7621) || defined (CONFIG_RALINK_MT7628)
    //send signal to user application to notify link status changed
    struct work_struct  kill_sig_wq;
#endif

    struct work_struct  reset_task;
#ifdef WORKQUEUE_BH
    struct work_struct  rx_wq;
#else
#if defined (TASKLET_WORKQUEUE_SW)
    struct work_struct  rx_wq;
#endif
    struct              tasklet_struct     rx_tasklet;
    struct              tasklet_struct     tx_tasklet;
#endif // WORKQUEUE_BH //

#if defined(CONFIG_RAETH_QOS)
    struct		sk_buff *	   skb_free[NUM_TX_RINGS][NUM_TX_DESC];
    unsigned int	free_idx[NUM_TX_RINGS];
#else
    struct		sk_buff*	   skb_free[NUM_TX_DESC];
    unsigned int	free_idx;
#endif

    struct              net_device_stats stat;  /* The new statistics table. */
    spinlock_t          page_lock;              /* Page register locks */
    struct PDMA_txdesc *tx_ring0;
#if defined(CONFIG_RAETH_QOS)
    struct PDMA_txdesc *tx_ring1;
    struct PDMA_txdesc *tx_ring2;
    struct PDMA_txdesc *tx_ring3;
#endif
    struct PDMA_rxdesc *rx_ring0;
    struct sk_buff     *netrx0_skbuf[NUM_RX_DESC];
#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
    struct PDMA_rxdesc *rx_ring1;
    struct sk_buff     *netrx1_skbuf[NUM_RX_DESC];
#endif
#ifdef CONFIG_RAETH_NAPI
    atomic_t irq_sem;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
    struct napi_struct napi;
#endif
#endif
#ifdef CONFIG_PSEUDO_SUPPORT
    struct net_device *PseudoDev;
    unsigned int isPseudo;
#endif
#if defined (CONFIG_ETHTOOL) /*&& defined (CONFIG_RAETH_ROUTER)*/
	struct mii_if_info	mii_info;
#endif
#ifdef CONFIG_RAETH_LRO
    struct lro_counters lro_counters;
    struct net_lro_mgr lro_mgr;
    struct net_lro_desc lro_arr[8];
#endif
#ifdef CONFIG_RAETH_HW_VLAN_RX
    struct vlan_group *vlgrp;
#endif

    /*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.01*/
    struct net_device * vth[4];	
    struct net_device * wangw;
    struct net_device * wanbr;
    /*END:  Sync from T5214EWL-OM(RT5350),2015.06.01*/	

} END_DEVICE, *pEND_DEVICE;

/**
 * ra2882eth_init - Module Init code
 *
 * Called by kernel to register net_device
 *
 */
int __init ra2882eth_init(void)
{
	int ret;
	struct net_device *dev = alloc_etherdev(sizeof(END_DEVICE));

#ifdef CONFIG_RALINK_VISTA_BASIC
	int sw_id=0;
	mii_mgr_read(29, 31, &sw_id);
	is_switch_175c = (sw_id == 0x175c) ? 1:0;
#endif 

	if (!dev)
		return -ENOMEM;

	strcpy(dev->name, DEV_NAME);
	dev->irq  = IRQ_ENET0;
	dev->addr_len = 6;
	dev->base_addr = RALINK_FRAME_ENGINE_BASE;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	rather_probe(dev);
#else
	dev->init =  rather_probe;
#endif
	ra2880_setup_dev_fptable(dev);

	/* net_device structure Init */
	ethtool_init(dev);
	printk("Ralink APSoC Ethernet Driver Initilization. %s  %d rx/tx descriptors allocated, mtu = %d!\n", RAETH_VERSION, NUM_RX_DESC, dev->mtu);
#ifdef CONFIG_RAETH_NAPI
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	printk("NAPI enable, Tx Ring = %d, Rx Ring = %d\n", NUM_TX_DESC, NUM_RX_DESC);
#else
	printk("NAPI enable, weight = %d, Tx Ring = %d, Rx Ring = %d\n", dev->weight, NUM_TX_DESC, NUM_RX_DESC);
#endif
#endif

	/* Register net device for the driver */
	if ( register_netdev(dev) != 0) {
		printk(KERN_WARNING " " __FILE__ ": No ethernet port found.\n");
		return -ENXIO;
	}


#ifdef CONFIG_RAETH_NETLINK
	csr_netlink_init();
#endif
	ret = debug_proc_init();

	dev_raether = dev;
	return ret;
}



/**
 * rather_probe - pick up ethernet port at boot time
 * @dev: network device to probe
 *
 * This routine probe the ethernet port at boot time.
 *
 *
 */

int __init rather_probe(struct net_device *dev)
{
	int i;
	END_DEVICE *ei_local = netdev_priv(dev);
	struct sockaddr addr;
	unsigned char zero1[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned char zero2[6]={0x00,0x00,0x00,0x00,0x00,0x00};

	fe_reset();

	//Get mac0 address from flash
#ifdef RA_MTD_RW_BY_NUM
	i = ra_mtd_read(2, GMAC0_OFFSET, 6, addr.sa_data);
#else
	i = ra_mtd_read_nm("Factory", GMAC0_OFFSET, 6, addr.sa_data);
#endif

	//If reading mtd failed or mac0 is empty, generate a mac address
	if (i < 0 || ((memcmp(addr.sa_data, zero1, 6) == 0) || (addr.sa_data[0] & 0x1)) || 
	    (memcmp(addr.sa_data, zero2, 6) == 0)) {
		unsigned char mac_addr01234[5] = {0x00, 0x0C, 0x43, 0x28, 0x80};
		net_srandom(jiffies);
		memcpy(addr.sa_data, mac_addr01234, 5);
		addr.sa_data[5] = net_random()&0xFF;
	}

#ifdef CONFIG_RAETH_NAPI
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	netif_napi_add(dev, &ei_local->napi, raeth_clean, 128);
#endif
#endif

	ei_set_mac_addr(dev, &addr);
	spin_lock_init(&ei_local->page_lock);
	ether_setup(dev);

#ifdef CONFIG_RAETH_LRO
	ei_local->lro_mgr.dev = dev;
        memset(&ei_local->lro_mgr.stats, 0, sizeof(ei_local->lro_mgr.stats));
        ei_local->lro_mgr.features = LRO_F_NAPI;
        ei_local->lro_mgr.ip_summed = CHECKSUM_UNNECESSARY;
        ei_local->lro_mgr.ip_summed_aggr = CHECKSUM_UNNECESSARY;
        ei_local->lro_mgr.max_desc = ARRAY_SIZE(ei_local->lro_arr);
        ei_local->lro_mgr.max_aggr = 64;
        ei_local->lro_mgr.frag_align_pad = 0;
        ei_local->lro_mgr.lro_arr = ei_local->lro_arr;
        ei_local->lro_mgr.get_skb_header = rt_get_skb_header;
#endif

	setup_statistics(ei_local);

	return 0;
}

void ra2880_setup_dev_fptable(struct net_device *dev)
{
	RAETH_PRINT(__FUNCTION__ "is called!\n");

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	dev->netdev_ops		= &ei_netdev_ops;
#else
	dev->open		= ei_open;
	dev->stop		= ei_close;
	dev->hard_start_xmit	= ei_start_xmit_fake;
	dev->get_stats		= ra_get_stats;
	dev->set_mac_address	= ei_set_mac_addr;
	dev->change_mtu		= ei_change_mtu;
	dev->mtu		= 1500;
	dev->do_ioctl		= ei_ioctl;
//	dev->tx_timeout		= ei_tx_timeout;

#ifdef CONFIG_RAETH_NAPI
        dev->poll = &raeth_clean;
#if defined (CONFIG_RAETH_ROUTER)
	dev->weight = 32;
#elif defined (CONFIG_RT_3052_ESW)
	dev->weight = 32;
#else
	dev->weight = 128;
#endif
#endif
#endif
#if defined (CONFIG_ETHTOOL) /*&& defined (CONFIG_RAETH_ROUTER)*/
	dev->ethtool_ops	= &ra_ethtool_ops;
#endif
#define TX_TIMEOUT (5*HZ)
	dev->watchdog_timeo = TX_TIMEOUT;

}


/**
 * hard_init - Called by raeth_probe to inititialize network device
 * @dev: device pointer
 *
 * ethdev_init initilize dev->priv and set to END_DEVICE structure
 *
 */
void ethtool_init(struct net_device *dev)
{
#if defined (CONFIG_ETHTOOL) /*&& defined (CONFIG_RAETH_ROUTER)*/
	END_DEVICE *ei_local = netdev_priv(dev);

	// init mii structure
	ei_local->mii_info.dev = dev;
	ei_local->mii_info.mdio_read = mdio_read;
	ei_local->mii_info.mdio_write = mdio_write;
	ei_local->mii_info.phy_id_mask = 0x1f;
	ei_local->mii_info.reg_num_mask = 0x1f;
	ei_local->mii_info.supports_gmii = mii_check_gmii_support(&ei_local->mii_info);
	// TODO:   phy_id: 0~4
	ei_local->mii_info.phy_id = 1;
#endif
	return;
}


3、使能交换机


/**
 * ei_open - Open/Initialize the ethernet port.
 * @dev: network device to initialize
 *
 * This routine goes all-out, setting everything
 * up a new at each open, even though many of these registers should only need to be set once at boot.
 */
int ei_open(struct net_device *dev)
{
	int i, err;
	unsigned long flags;
	END_DEVICE *ei_local;

#ifdef CONFIG_RAETH_LRO
	const char *lan_ip_tmp; 
#ifdef CONFIG_DUAL_IMAGE
#define RT2860_NVRAM	1
#else
#define RT2860_NVRAM	0
#endif
#endif // CONFIG_RAETH_LRO //

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0)
	if (!try_module_get(THIS_MODULE))
	{
		printk("%s: Cannot reserve module\n", __FUNCTION__);
		return -1;
	}
#else
	MOD_INC_USE_COUNT;
#endif

	printk("Raeth %s (",RAETH_VERSION);
#if defined (CONFIG_RAETH_NAPI)
	printk("NAPI\n");
#elif defined (CONFIG_RA_NETWORK_TASKLET_BH)
	printk("Tasklet");
#elif defined (CONFIG_RA_NETWORK_WORKQUEUE_BH)
	printk("Workqueue");
#endif

#if defined (CONFIG_RAETH_SKB_RECYCLE_2K)
	printk(",SkbRecycle");
#endif
	printk(")\n");


  	ei_local = netdev_priv(dev); // get device pointer from System
	// unsigned int flags;

	if (ei_local == NULL)
	{
		printk(KERN_EMERG "%s: ei_open passed a non-existent device!\n", dev->name);
		return -ENXIO;
	}

        /* receiving packet buffer allocation - NUM_RX_DESC x MAX_RX_LENGTH */
        for ( i = 0; i < NUM_RX_DESC; i++)
        {
#if defined (CONFIG_RAETH_SKB_RECYCLE_2K)
                ei_local->netrx0_skbuf[i] = skbmgr_dev_alloc_skb2k();
#else
                ei_local->netrx0_skbuf[i] = dev_alloc_skb(MAX_RX_LENGTH + NET_IP_ALIGN);
#endif
                if (ei_local->netrx0_skbuf[i] == NULL ) {
                        printk("rx skbuff buffer allocation failed!");
		} else {
#if !defined (CONFIG_RAETH_SCATTER_GATHER_RX_DMA)
		    skb_reserve(ei_local->netrx0_skbuf[i], NET_IP_ALIGN);
#endif
		}
		

#if defined (CONFIG_RAETH_MULTIPLE_RX_RING) 
		ei_local->netrx1_skbuf[i] = dev_alloc_skb(MAX_RX_LENGTH + NET_IP_ALIGN);
                if (ei_local->netrx1_skbuf[i] == NULL ) {
                        printk("rx1 skbuff buffer allocation failed!");
		} else {
#if !defined (CONFIG_RAETH_SCATTER_GATHER_RX_DMA)
		    skb_reserve(ei_local->netrx1_skbuf[i], NET_IP_ALIGN);
#endif
		}
#endif
        }


#if defined (CONFIG_GE1_TRGMII_FORCE_1200) && defined (CONFIG_MT7621_ASIC)
	trgmii_set_7621(); //reset FE/GMAC in this function
#endif
	
        fe_dma_init(dev);
	fe_sw_init(); //initialize fe and switch register
	err = request_irq( dev->irq, ei_interrupt, IRQF_DISABLED, dev->name, dev);	// try to fix irq in open
	
	if (err)
	    return err;

	if ( dev->dev_addr != NULL) {
	    ra2880MacAddressSet((void *)(dev->dev_addr));
	} else {
	    printk("dev->dev_addr is empty !\n");
	} 
#if defined (CONFIG_RT_3052_ESW)
	err = request_irq(SURFBOARDINT_ESW, esw_interrupt, IRQF_DISABLED, "Ralink_ESW", dev);
	if (err)
		return err;
	INIT_WORK(&ei_local->kill_sig_wq, kill_sig_workq);
#if defined (CONFIG_RALINK_MT7621)
        mii_mgr_write(31, 0x7008, 0x1f); //enable switch link change intr
	
#else
	*((volatile u32 *)(RALINK_INTCL_BASE + 0x34)) = (1<<17);
	*((volatile u32 *)(ESW_IMR)) &= ~(ESW_INT_ALL);

#if defined (CONFIG_RALINK_RT6855) || defined(CONFIG_RALINK_RT6855A) || \
    defined(CONFIG_RALINK_MT7620)
	*((volatile u32 *)(ESW_P0_IntMn)) &= ~(MSK_CNT_INT_ALL);
	*((volatile u32 *)(ESW_P1_IntMn)) &= ~(MSK_CNT_INT_ALL);
	*((volatile u32 *)(ESW_P2_IntMn)) &= ~(MSK_CNT_INT_ALL);
	*((volatile u32 *)(ESW_P3_IntMn)) &= ~(MSK_CNT_INT_ALL);
	*((volatile u32 *)(ESW_P4_IntMn)) &= ~(MSK_CNT_INT_ALL);
	*((volatile u32 *)(ESW_P5_IntMn)) &= ~(MSK_CNT_INT_ALL);
	*((volatile u32 *)(ESW_P6_IntMn)) &= ~(MSK_CNT_INT_ALL);
#endif
#if    defined(CONFIG_RALINK_MT7620)
	*((volatile u32 *)(ESW_P7_IntMn)) &= ~(MSK_CNT_INT_ALL);
#endif

#endif	
#endif // CONFIG_RT_3052_ESW //

	spin_lock_irqsave(&(ei_local->page_lock), flags);

#ifdef DELAY_INT
        sysRegWrite(RAETH_DLY_INT_CFG, DELAY_INT_INIT);
    	sysRegWrite(RAETH_FE_INT_ENABLE, RAETH_FE_INT_DLY_INIT);
#else
    	sysRegWrite(RAETH_FE_INT_ENABLE, RAETH_FE_INT_ALL);
#endif
#ifdef CONFIG_RAETH_QDMA
#ifdef DELAY_INT
        sysRegWrite(QDMA_DELAY_INT, DELAY_INT_INIT);
    	sysRegWrite(QFE_INT_ENABLE, QFE_INT_DLY_INIT);
#else
    	sysRegWrite(QFE_INT_ENABLE, QFE_INT_ALL);

#endif
#endif

 	INIT_WORK(&ei_local->reset_task, ei_reset_task);

#ifdef WORKQUEUE_BH
#ifndef CONFIG_RAETH_NAPI
 	INIT_WORK(&ei_local->rx_wq, ei_receive_workq);
#endif // CONFIG_RAETH_NAPI //
#else
#ifndef CONFIG_RAETH_NAPI
#if defined (TASKLET_WORKQUEUE_SW)
	working_schedule = init_schedule;
 	INIT_WORK(&ei_local->rx_wq, ei_receive_workq);
	tasklet_init(&ei_local->rx_tasklet, ei_receive_workq, 0);
#else
	tasklet_init(&ei_local->rx_tasklet, ei_receive, 0);
#endif
#endif // CONFIG_RAETH_NAPI //
#endif // WORKQUEUE_BH //

	netif_start_queue(dev);

#ifdef CONFIG_RAETH_NAPI

	atomic_dec(&ei_local->irq_sem);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
        napi_enable(&ei_local->napi);
#else
        netif_poll_enable(dev);
#endif
#endif

	spin_unlock_irqrestore(&(ei_local->page_lock), flags);
#ifdef CONFIG_PSEUDO_SUPPORT
	if(ei_local->PseudoDev == NULL) {
	    RAETH_Init_PSEUDO(ei_local, dev);
	}
 
	if(ei_local->PseudoDev == NULL) 
		printk("Open PseudoDev failed.\n");
	else
		VirtualIF_open(ei_local->PseudoDev);

#endif

#ifdef CONFIG_RAETH_LRO
	lan_ip_tmp = nvram_get(RT2860_NVRAM, "lan_ipaddr");
	str_to_ip(&lan_ip, lan_ip_tmp);
	lro_para.lan_ip1 = lan_ip = htonl(lan_ip);
#endif // CONFIG_RAETH_LRO //

	/*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.01*/
	RAETH_Init_PSEUDO_vthx(ei_local, dev);
	VirtualIF_open_vthx(ei_local->vth[0]);
	VirtualIF_open_vthx(ei_local->vth[1]);
	VirtualIF_open_vthx(ei_local->vth[2]);
	VirtualIF_open_vthx(ei_local->vth[3]);
	VirtualIF_open_vthx(ei_local->wangw);
	VirtualIF_open_vthx(ei_local->wanbr);	
       /* BEGIN: added by wuhaiding for BUG0002235, 20150923 */
       raeth_vthx_done = 1;
       /* END: added by wuhaiding for BUG0002235, 20150923 */
	/*END:  Sync from T5214EWL-OM(RT5350),2015.06.01*/	
    
	forward_config(dev);
	return 0;
}

4、中断函数
static irqreturn_t esw_interrupt(int irq, void *dev_id)
{
	unsigned long flags;
	unsigned long reg_int_val;
#if defined (CONFIG_RALINK_RT6855) || defined(CONFIG_RALINK_RT6855A) || \
    defined(CONFIG_RALINK_MT7620)
	unsigned long acl_int_val;
	unsigned long mib_int_val;
#else
	static unsigned long stat;
	unsigned long stat_curr;
#endif
	
	struct net_device *dev = (struct net_device *) dev_id;
	END_DEVICE *ei_local = netdev_priv(dev);


	spin_lock_irqsave(&(ei_local->page_lock), flags);
	reg_int_val = (*((volatile u32 *)(ESW_ISR))); //Interrupt Status Register

#if defined (CONFIG_RALINK_RT6855) || defined(CONFIG_RALINK_RT6855A) || \
    defined(CONFIG_RALINK_MT7620)
	if (reg_int_val & P5_LINK_CH) {
	    esw_link_status_changed(5, dev_id);
	}
	if (reg_int_val & P4_LINK_CH) {
	    esw_link_status_changed(4, dev_id);
	}
	if (reg_int_val & P3_LINK_CH) {
	    esw_link_status_changed(3, dev_id);
	}
	if (reg_int_val & P2_LINK_CH) {
	    esw_link_status_changed(2, dev_id);
	}
	if (reg_int_val & P1_LINK_CH) {
	    esw_link_status_changed(1, dev_id);
	}
	if (reg_int_val & P0_LINK_CH) {
	    esw_link_status_changed(0, dev_id);
	}
	if (reg_int_val & ACL_INT) {
	    acl_int_val = sysRegRead(ESW_AISR);
	    sysRegWrite(ESW_AISR, acl_int_val);
	}
	if (reg_int_val & MIB_INT) {

	    mib_int_val = sysRegRead(ESW_P0_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P0_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p0_rx_good_cnt ++;	
		if(mib_int_val & TX_GOOD_CNT)
			p0_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p0_rx_byte_cnt ++;
		if(mib_int_val & TX_GOCT_CNT)
			p0_tx_byte_cnt ++;
	    }

	    mib_int_val = sysRegRead(ESW_P1_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P1_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p1_rx_good_cnt ++;		
		if(mib_int_val & TX_GOOD_CNT)
			p1_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p1_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p1_tx_byte_cnt ++;	
	    }

	    mib_int_val = sysRegRead(ESW_P2_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P2_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p2_rx_good_cnt ++;		
		if(mib_int_val & TX_GOOD_CNT)
			p2_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p2_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p2_tx_byte_cnt ++;	
	    }


	    mib_int_val = sysRegRead(ESW_P3_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P3_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p3_rx_good_cnt ++;		
		if(mib_int_val & TX_GOOD_CNT)
			p3_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p3_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p3_tx_byte_cnt ++;	
	    }

	    mib_int_val = sysRegRead(ESW_P4_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P4_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p4_rx_good_cnt ++;	
		if(mib_int_val & TX_GOOD_CNT)
			p4_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p4_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p4_tx_byte_cnt ++;	
	    }	

	    mib_int_val = sysRegRead(ESW_P5_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P5_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p5_rx_good_cnt ++;		
		if(mib_int_val & TX_GOOD_CNT)
			p5_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p5_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p5_tx_byte_cnt ++;	
	    }

	    mib_int_val = sysRegRead(ESW_P6_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P6_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p6_rx_good_cnt ++;		
		if(mib_int_val & TX_GOOD_CNT)
			p6_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p6_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p6_tx_byte_cnt ++;	
	    }
#if defined (CONFIG_RALINK_MT7620)
	    mib_int_val = sysRegRead(ESW_P7_IntSn);
	    if(mib_int_val){
		sysRegWrite(ESW_P7_IntSn, mib_int_val);
		if(mib_int_val & RX_GOOD_CNT)
			p7_rx_good_cnt ++;		
		if(mib_int_val & TX_GOOD_CNT)
			p7_tx_good_cnt ++;	
		if(mib_int_val & RX_GOCT_CNT)
			p7_rx_byte_cnt ++;	
		if(mib_int_val & TX_GOCT_CNT)
			p7_tx_byte_cnt ++;	
  
	    }
#endif	    
	}

#else // not RT6855
	if (reg_int_val & PORT_ST_CHG) {
		printk("RT305x_ESW: Link Status Changed\n");

		stat_curr = *((volatile u32 *)(RALINK_ETH_SW_BASE+0x80));
#ifdef CONFIG_WAN_AT_P0
		//link down --> link up : send signal to user application
		//link up --> link down : ignore
		if ((stat & (1<<25)) || !(stat_curr & (1<<25)))
#else
		if ((stat & (1<<29)) || !(stat_curr & (1<<29)))
#endif
			goto out;

		schedule_work(&ei_local->kill_sig_wq);
out:
		stat = stat_curr;
	}

#endif // defined(CONFIG_RALINK_RT6855) || defined(CONFIG_RALINK_RT6855A)//

	sysRegWrite(ESW_ISR, reg_int_val);

	spin_unlock_irqrestore(&(ei_local->page_lock), flags);
	return IRQ_HANDLED;
}

/**
 * ei_interrupt - handle controler interrupt
 *
 * This routine is called at interrupt level in response to an interrupt from
 * the controller.
 *
 * RETURNS: N/A.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
static irqreturn_t ei_interrupt(int irq, void *dev_id)
{
#if !defined(CONFIG_RAETH_NAPI)
	unsigned long reg_int_val;
	unsigned long reg_int_mask=0;
	unsigned int recv = 0;
	unsigned int transmit __maybe_unused = 0;
	unsigned long flags;
#endif

	struct net_device *dev = (struct net_device *) dev_id;
	END_DEVICE *ei_local = netdev_priv(dev);

	//Qwert
	/*
	unsigned long old,cur,dcycle;
	static int cnt = 0;
	static unsigned long max_dcycle = 0,tcycle = 0;
	old = read_c0_count();
	*/
	if (dev == NULL)
	{
		printk (KERN_ERR "net_interrupt(): irq %x for unknown device.\n", IRQ_ENET0);
		return IRQ_NONE;
	}

#ifdef CONFIG_RAETH_NAPI
	gsw_delay_setting();
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
        if(napi_schedule_prep(&ei_local->napi)) {
#else
        if(netif_rx_schedule_prep(dev)) {
#endif
                atomic_inc(&ei_local->irq_sem);
		sysRegWrite(RAETH_FE_INT_ENABLE, 0);
#ifdef CONFIG_RAETH_QDMA		
		sysRegWrite(QFE_INT_ENABLE, 0);
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
		__napi_schedule(&ei_local->napi);
#else
                __netif_rx_schedule(dev);
#endif
        }
#else

	spin_lock_irqsave(&(ei_local->page_lock), flags);
	reg_int_val = sysRegRead(RAETH_FE_INT_STATUS);
#ifdef CONFIG_RAETH_QDMA	
	reg_int_val |= sysRegRead(QFE_INT_STATUS);
#endif

#if defined (DELAY_INT)
	if((reg_int_val & RX_DLY_INT))
		recv = 1;
	
	if (reg_int_val & RAETH_TX_DLY_INT)
		transmit = 1;
#else
	if((reg_int_val & RX_DONE_INT0))
		recv = 1;

#if defined (CONFIG_RAETH_MULTIPLE_RX_RING) 
	if((reg_int_val & RX_DONE_INT1))
		recv = 1;
#endif

	if (reg_int_val & RAETH_TX_DONE_INT0)
		transmit |= RAETH_TX_DONE_INT0;
#if defined (CONFIG_RAETH_QOS)
	if (reg_int_val & TX_DONE_INT1)
		transmit |= TX_DONE_INT1;
	if (reg_int_val & TX_DONE_INT2)
		transmit |= TX_DONE_INT2;
	if (reg_int_val & TX_DONE_INT3)
		transmit |= TX_DONE_INT3;
#endif //CONFIG_RAETH_QOS

#endif //DELAY_INT

#if defined (DELAY_INT)
	sysRegWrite(RAETH_FE_INT_STATUS, RAETH_FE_INT_DLY_INIT);
#else
	sysRegWrite(RAETH_FE_INT_STATUS, RAETH_FE_INT_ALL);
#endif
#ifdef CONFIG_RAETH_QDMA
#if defined (DELAY_INT)
	sysRegWrite(QFE_INT_STATUS, QFE_INT_DLY_INIT);
#else
	sysRegWrite(QFE_INT_STATUS, QFE_INT_ALL);
#endif
#endif	

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,0)
	if(transmit)
		ei_xmit_housekeeping(0);
#else
		ei_xmit_housekeeping(0);
#endif

	if (((recv == 1) || (pending_recv ==1)) && (tx_ring_full==0))
	{
		reg_int_mask = sysRegRead(RAETH_FE_INT_ENABLE);
#if defined (DELAY_INT)
		sysRegWrite(RAETH_FE_INT_ENABLE, reg_int_mask & ~(RX_DLY_INT));
#else
		sysRegWrite(RAETH_FE_INT_ENABLE, reg_int_mask & ~(RX_DONE_INT0 | RX_DONE_INT1));
#endif //DELAY_INT
#ifdef CONFIG_RAETH_QDMA		
		reg_int_mask = sysRegRead(QFE_INT_ENABLE);
#if defined (DELAY_INT)
		sysRegWrite(QFE_INT_ENABLE, reg_int_mask & ~(RX_DLY_INT));
#else
		sysRegWrite(QFE_INT_ENABLE, reg_int_mask & ~(RX_DONE_INT0 | RX_DONE_INT1));
#endif //DELAY_INT
#endif

		pending_recv=0;
#ifdef WORKQUEUE_BH
		schedule_work(&ei_local->rx_wq);
#else
#if defined (TASKLET_WORKQUEUE_SW)
		if (working_schedule == 1)
			schedule_work(&ei_local->rx_wq);
		else
#endif
		tasklet_hi_schedule(&ei_local->rx_tasklet);
#endif // WORKQUEUE_BH //
	} 
	else if (recv == 1 && tx_ring_full==1) 
	{
		pending_recv=1;
	}
	else if((recv == 0) && (transmit == 0))
	{
		gsw_delay_setting();
	}
	spin_unlock_irqrestore(&(ei_local->page_lock), flags);
#endif

	return IRQ_HANDLED;
}

5、NAPI收包
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
static int raeth_clean(struct napi_struct *napi, int budget)
{
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	struct net_device *netdev=dev_raether;
        int work_to_do = budget;
#else
        int work_to_do = min(*budget, netdev->quota);
#endif
	END_DEVICE *ei_local =netdev_priv(netdev);
        int work_done = 0;
	unsigned long reg_int_mask=0;

	ei_xmit_housekeeping(0);

	rt2880_eth_recv(netdev, &work_done, work_to_do);

        /* this could control when to re-enable interrupt, 0-> mean never enable interrupt*/
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35)
        *budget -= work_done;
        netdev->quota -= work_done;
#endif
        /* if no Tx and not enough Rx work done, exit the polling mode */
        if(( (work_done < work_to_do)) || !netif_running(netdev)) {
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
		napi_complete(&ei_local->napi);
#else
                netif_rx_complete(netdev);
#endif
		atomic_dec_and_test(&ei_local->irq_sem);

		sysRegWrite(RAETH_FE_INT_STATUS, RAETH_FE_INT_ALL);		// ack all fe interrupts
    		reg_int_mask=sysRegRead(RAETH_FE_INT_ENABLE);

#ifdef DELAY_INT
		sysRegWrite(RAETH_FE_INT_ENABLE, reg_int_mask |RAETH_FE_INT_DLY_INIT);  // init delay interrupt only
#else
		sysRegWrite(RAETH_FE_INT_ENABLE,reg_int_mask | RAETH_FE_INT_SETTING);
#endif

#ifdef CONFIG_RAETH_QDMA
		sysRegWrite(QFE_INT_STATUS, QFE_INT_ALL);
		reg_int_mask=sysRegRead(QFE_INT_ENABLE);
#ifdef DELAY_INT
                sysRegWrite(QFE_INT_ENABLE, reg_int_mask |QFE_INT_DLY_INIT);  // init delay interrupt only
#else
                sysRegWrite(QFE_INT_ENABLE,reg_int_mask | (RX_DONE_INT0 | RX_DONE_INT1 | RLS_DONE_INT));
#endif
#endif // CONFIG_RAETH_QDMA //

                return 0;
        }

        return 1;
}

/**
1、for ( ; ; )大循环中，将报文从rx_ring中将报文取出，可就收总数为work_to_do，最终完成收包数为work_done。
2、
if (ei_local->rx_ring0[rx_dma_owner_idx0].rxd_info2.DDONE_bit == 1)  {
	rx_ring = ei_local->rx_ring0;
	rx_dma_owner_idx = rx_dma_owner_idx0;
} else {
	break;
}
rxd_info2.DDONE_bit等于1，表示该收包描述符下数据区收包成功，否则跳出for大循环。
3、
rx_dma_owner_idx0 = (sysRegRead(RAETH_RX_CALC_IDX0) + 1) % NUM_RX_DESC;
......
for( ; ; ){
	rx_dma_owner_idx = rx_dma_owner_idx0;
	......
	
	rx_skb = ei_local->netrx0_skbuf[rx_dma_owner_idx];
	rx_skb->data = ei_local->netrx0_skbuf[rx_dma_owner_idx]->data;
	......
	
	netif_receive_skb(rx_skb);
	......
	
	skb = __dev_alloc_skb(MAX_RX_LENGTH + NET_IP_ALIGN, GFP_ATOMIC);
	......
	
	rx_ring[rx_dma_owner_idx].rxd_info2.DDONE_bit = 0;
	rx_ring[rx_dma_owner_idx].rxd_info1.PDP0 = dma_map_single(NULL, skb->data, MAX_RX_LENGTH, PCI_DMA_FROMDEVICE);
	......
	
	sysRegWrite(RAETH_RX_CALC_IDX0, rx_dma_owner_idx);
	ei_local->netrx0_skbuf[rx_dma_owner_idx] = skb;
	......
	
	rx_dma_owner_idx0 = (sysRegRead(RAETH_RX_CALC_IDX0) + 1) % NUM_RX_DESC;
}
RAETH_RX_CALC_IDX0表示被软件接收的收包索引号。
整个流程是
读出索引号-》取出报文数据-》报文上送协议栈-》分配新的skb->data到rx_ring-》索引号+1
4、
my_veth = (struct vlan_ethhdr *)(rx_skb->data);
vlan_id = (ntohs(my_veth->h_vlan_TCI)) & VLAN_VID_MASK;

if(((my_veth->h_vlan_proto & 0xFF) == 0x81) && (1 == raeth_vthx_done)) 

{
switch (vlan_id)
{
	case RESERVE_VLAN_ID_FOR_P0:
		port_num = 0;
		pkt_to_dev = ei_local->vth[port_num];
		flag = FLAG_LAN_PORT;
		break;
	case RESERVE_VLAN_ID_FOR_P1:
		port_num = 1;
		pkt_to_dev = ei_local->vth[port_num];
		flag = FLAG_LAN_PORT;
		break;
	case RESERVE_VLAN_ID_FOR_P2:
		port_num = 2;
		pkt_to_dev = ei_local->vth[port_num];
		flag = FLAG_LAN_PORT;
		break;
	case RESERVE_VLAN_ID_FOR_P3:
		port_num = 3;
		pkt_to_dev = ei_local->vth[port_num];
		flag = FLAG_LAN_PORT;
		break;
	case RESERVE_VLAN_ID_FOR_P4:
		port_num = 4;
		pkt_to_dev = ei_local->wangw;
		flag = FLAG_WAN_PORT;
		break;
	default:
		port_num = 0;
		pkt_to_dev = ei_local->vth[port_num];
		flag = FLAG_LAN_PORT;                    
		break;
}

memmove(rx_skb->data + VLAN_HLEN, rx_skb->data, 2 * VLAN_ETH_ALEN);
rx_skb->len  -= VLAN_HLEN;
rx_skb->data += VLAN_HLEN;
报文上送协议栈之前，通过报文vlanID区别port口和上送规则，并剔除VLANID字段。
*/
#ifdef CONFIG_RAETH_NAPI
static int rt2880_eth_recv(struct net_device* dev, int *work_done, int work_to_do)
{
	struct sk_buff	*skb, *rx_skb;
	unsigned int	length = 0;
	unsigned long	RxProcessed;

	int bReschedule = 0;
	END_DEVICE* 	ei_local = netdev_priv(dev);
#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
	int rx_ring_no=0;
#endif

#if defined (CONFIG_RAETH_SPECIAL_TAG)
	struct vlan_ethhdr *veth=NULL;
#endif

#ifdef CONFIG_PSEUDO_SUPPORT
	PSEUDO_ADAPTER *pAd;
#endif

	RxProcessed = 0;
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
	rx_dma_owner_idx0 = (rx_calc_idx0 + 1) % NUM_RX_DESC;
#else
	rx_dma_owner_idx0 = (sysRegRead(RAETH_RX_CALC_IDX0) + 1) % NUM_RX_DESC;
#endif

#if defined (CONFIG_32B_DESC)
#if defined (CONFIG_MIPS)
	dma_cache_sync(NULL, &ei_local->rx_ring0[rx_dma_owner_idx0], sizeof(struct PDMA_rxdesc), DMA_FROM_DEVICE);
#else
	dma_sync_single_for_cpu(NULL, &ei_local->rx_ring0[rx_dma_owner_idx0], sizeof(struct PDMA_rxdesc), DMA_FROM_DEVICE);
#endif
#endif
#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
	rx_dma_owner_idx1 = (rx_calc_idx1 + 1) % NUM_RX_DESC;
#else
	rx_dma_owner_idx1 = (sysRegRead(RX_CALC_IDX1) + 1) % NUM_RX_DESC;
#endif
#if defined (CONFIG_32B_DESC)
#if defined (CONFIG_MIPS)
	dma_cache_sync(NULL, &ei_local->rx_ring1[rx_dma_owner_idx1], sizeof(struct PDMA_rxdesc), DMA_FROM_DEVICE);
#else
	dma_sync_single_for_cpu(NULL, &ei_local->rx_ring0[rx_dma_owner_idx0], sizeof(struct PDMA_rxdesc), DMA_FROM_DEVICE);
#endif
#endif
#endif
	for ( ; ; ) {

#ifdef CONFIG_RAETH_NAPI
                if(*work_done >= work_to_do)
                        break;
                (*work_done)++;
#else
		if (RxProcessed++ > NUM_RX_MAX_PROCESS)
                {
                        // need to reschedule rx handle
                        bReschedule = 1;
                        break;
                }
#endif


#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
		if (ei_local->rx_ring1[rx_dma_owner_idx1].rxd_info2.DDONE_bit == 1)  {
		    rx_ring = ei_local->rx_ring1;
		    rx_dma_owner_idx = rx_dma_owner_idx1;
		//    printk("rx_dma_owner_idx1=%x\n",rx_dma_owner_idx1);
		    rx_ring_no=1;
		} else if (ei_local->rx_ring0[rx_dma_owner_idx0].rxd_info2.DDONE_bit == 1)  {
		    rx_ring = ei_local->rx_ring0;
		    rx_dma_owner_idx = rx_dma_owner_idx0;
		 //   printk("rx_dma_owner_idx0=%x\n",rx_dma_owner_idx0);
		    rx_ring_no=0;
		} else {
		    break;
		}
#else

		if (ei_local->rx_ring0[rx_dma_owner_idx0].rxd_info2.DDONE_bit == 1)  {
		    rx_ring = ei_local->rx_ring0;
		    rx_dma_owner_idx = rx_dma_owner_idx0;
		} else {
		    break;
		}
#endif

#if defined (CONFIG_32B_DESC)
		prefetch(&rx_ring[(rx_dma_owner_idx + 1) % NUM_RX_DESC]);
#endif
		/* skb processing */
		length = rx_ring[rx_dma_owner_idx].rxd_info2.PLEN0;
#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
		if(rx_ring_no==1) {
		    rx_skb = ei_local->netrx1_skbuf[rx_dma_owner_idx];
		    rx_skb->data = ei_local->netrx1_skbuf[rx_dma_owner_idx]->data;
		} else {
		    rx_skb = ei_local->netrx0_skbuf[rx_dma_owner_idx];
		    rx_skb->data = ei_local->netrx0_skbuf[rx_dma_owner_idx]->data;
		}
#else
		rx_skb = ei_local->netrx0_skbuf[rx_dma_owner_idx];
		rx_skb->data = ei_local->netrx0_skbuf[rx_dma_owner_idx]->data;
#endif
		rx_skb->len 	= length;

#if defined (CONFIG_RAETH_SCATTER_GATHER_RX_DMA)
		rx_skb->data += NET_IP_ALIGN;
#endif

		rx_skb->tail 	= rx_skb->data + length;

/*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.01*/
#if 0
#ifdef CONFIG_PSEUDO_SUPPORT
		if(rx_ring[rx_dma_owner_idx].rxd_info4.SP == 2) {
		    if(ei_local->PseudoDev!=NULL) {
			rx_skb->dev 	  = ei_local->PseudoDev;
			rx_skb->protocol  = eth_type_trans(rx_skb,ei_local->PseudoDev);
		    }else {
			printk("ERROR: PseudoDev is still not initialize but receive packet from GMAC2\n");
		    }
		}else{
		    rx_skb->dev 	  = dev;
		    rx_skb->protocol	  = eth_type_trans(rx_skb,dev);
		}
#else
		rx_skb->dev 	  = dev;
		rx_skb->protocol  = eth_type_trans(rx_skb,dev);
#endif
#endif

		struct net_device * pkt_to_dev = NULL;
		struct vlan_ethhdr *my_veth    = NULL;		
		unsigned int port_num = 0;		
		struct sk_buff	*skb_cp = NULL;
		unsigned short vlan_id = 0;
		enum port_flag flag = FLAG_BUTT;
		int to_wangw   = 0;
		int to_wanbr   = 0;
		int is_forward = 0;

		my_veth = (struct vlan_ethhdr *)(rx_skb->data);
		vlan_id = (ntohs(my_veth->h_vlan_TCI)) & VLAN_VID_MASK;

              /* BEGIN: modified by wuhaiding for BUG0002235, 20150923 */
		if(((my_veth->h_vlan_proto & 0xFF) == 0x81) && (1 == raeth_vthx_done)) 
             /* END: modified by wuhaiding for BUG0002235, 20150923 */
		{
            switch (vlan_id)
            {
                case RESERVE_VLAN_ID_FOR_P0:
                    port_num = 0;
                    pkt_to_dev = ei_local->vth[port_num];
                    flag = FLAG_LAN_PORT;
                    break;
                case RESERVE_VLAN_ID_FOR_P1:
                    port_num = 1;
                    pkt_to_dev = ei_local->vth[port_num];
                    flag = FLAG_LAN_PORT;
                    break;
                case RESERVE_VLAN_ID_FOR_P2:
                    port_num = 2;
                    pkt_to_dev = ei_local->vth[port_num];
                    flag = FLAG_LAN_PORT;
                    break;
                case RESERVE_VLAN_ID_FOR_P3:
                    port_num = 3;
                    pkt_to_dev = ei_local->vth[port_num];
                    flag = FLAG_LAN_PORT;
                    break;
                case RESERVE_VLAN_ID_FOR_P4:
                    port_num = 4;
                    pkt_to_dev = ei_local->wangw;
                    flag = FLAG_WAN_PORT;
                    break;
                default:
                    port_num = 0;
                    pkt_to_dev = ei_local->vth[port_num];
                    flag = FLAG_LAN_PORT;                    
                    break;
            }

            memmove(rx_skb->data + VLAN_HLEN, rx_skb->data, 2 * VLAN_ETH_ALEN);
            rx_skb->len  -= VLAN_HLEN;
            rx_skb->data += VLAN_HLEN;

            is_forward = 0;
            if (l2fp_en)
            {
                struct vlan_ethhdr *l2fp_veth = (struct vlan_ethhdr *)rx_skb->data;
                if (0 == is_multicast_ether_addr(l2fp_veth->h_dest))
                {    
                    rx_skb->src_port = port_num;                
                    if (l2fp_veth->h_vlan_proto == __constant_htons(ETH_P_8021Q))
                    {
                        rx_skb->src_vlanid = (ntohs(l2fp_veth->h_vlan_TCI)) & VLAN_VID_MASK;
                    }
                    else
                    {
                        rx_skb->src_vlanid = 0;
                    }
                    
                    l2fp_forward(rx_skb, ei_local, &is_forward, port_num, rx_skb->src_vlanid);
					if (is_forward)
					{
                        L2FP_DBG("pkt-parse-done:go fastpath.\n\n");
						goto skb_opt_a_done;
					}
                    L2FP_DBG("pkt-parse-done:go slowpath.\n\n");
                }
                else
                {
                    L2FP_DBG("pkt-parse-done:go slowpath(multicast-pkt).\n\n");
                }
            }

            if (FLAG_WAN_PORT == flag)
			{
				struct ethhdr * eth_gwbr = (struct ethhdr *)rx_skb->data;        
				if (is_multicast_ether_addr(eth_gwbr->h_dest))
				{
				    to_wangw = 1;
					to_wanbr = 1;
				}
				else if( 0 == compare_ether_addr(eth_gwbr->h_dest, ei_local->wangw->dev_addr))
				{			
				    to_wangw = 1;
					to_wanbr = 0;			    
				}
				else
				{
				    to_wangw = 0;
					to_wanbr = 1;			    
				}	

				if(1 == to_wangw && 1 == to_wanbr)
				{
		            skb_cp = skb_copy(rx_skb, GFP_ATOMIC);
		            if (unlikely(skb_cp == NULL))
		            {
						pkt_to_dev        = ei_local->wangw;					
					    rx_skb->dev 	  = pkt_to_dev;
						rx_skb->protocol  = eth_type_trans(rx_skb, pkt_to_dev);					
						struct net_device_stats *stats = VirtualIF_get_stats_vthx(pkt_to_dev);
						stats->rx_packets++;
						stats->rx_bytes  += rx_skb->len;

                        to_wangw = 1;
                        to_wanbr = 0;
		            }
		            else
		            {
						pkt_to_dev        = ei_local->wangw;						
						rx_skb->dev 	  = pkt_to_dev;
						rx_skb->protocol  = eth_type_trans(rx_skb, pkt_to_dev); 				
						struct net_device_stats *stats = VirtualIF_get_stats_vthx(pkt_to_dev);
						stats->rx_packets++;
						stats->rx_bytes  += rx_skb->len; 

						pkt_to_dev        = ei_local->wanbr;						
						skb_cp->dev 	  = pkt_to_dev;
						skb_cp->protocol  = eth_type_trans(skb_cp, pkt_to_dev); 				
						struct net_device_stats *stats2 = VirtualIF_get_stats_vthx(pkt_to_dev);
						stats2->rx_packets++;
						stats2->rx_bytes += skb_cp->len; 						
		            }				    
				}
				else if(1 == to_wangw)
				{
					pkt_to_dev        = ei_local->wangw;				
				    rx_skb->dev 	  = pkt_to_dev;
					rx_skb->protocol  = eth_type_trans(rx_skb, pkt_to_dev);					
					struct net_device_stats *stats = VirtualIF_get_stats_vthx(pkt_to_dev);
					stats->rx_packets++;
					stats->rx_bytes  += rx_skb->len;					
				}
				else if(1 == to_wanbr)
				{
					pkt_to_dev        = ei_local->wanbr;				
				    rx_skb->dev 	  = pkt_to_dev;
					rx_skb->protocol  = eth_type_trans(rx_skb, pkt_to_dev);					
					struct net_device_stats *stats = VirtualIF_get_stats_vthx(pkt_to_dev);
					stats->rx_packets++;
					stats->rx_bytes  += rx_skb->len; 					
				}
				else 
				{
					printk("==== to_wangw = 0, to_wanbr = 0, error!\n");
				}
			}
			else if (FLAG_LAN_PORT == flag)
			{
			    rx_skb->dev 	  = pkt_to_dev;
				rx_skb->protocol  = eth_type_trans(rx_skb, pkt_to_dev);
				
				struct net_device_stats *stats = VirtualIF_get_stats_vthx(pkt_to_dev);
				stats->rx_packets++;
				stats->rx_bytes += rx_skb->len; 
			}
			else 
			{
				rx_skb->dev 	  = dev;
				rx_skb->protocol  = eth_type_trans(rx_skb, dev);
			}
            
		}
		else
		{
				rx_skb->dev 	  = dev;
				rx_skb->protocol  = eth_type_trans(rx_skb, dev);
		}
    	/*END:  Sync from T5214EWL-OM(RT5350),2015.06.01*/

/*BEGIN:Modified by huangcd for mantis0001508(l2fp_opt),2015.02.28*/
skb_opt_a_done:
/*END:  Modified by huangcd for mantis0001508(l2fp_opt),2015.02.28*/

#ifdef CONFIG_RAETH_CHECKSUM_OFFLOAD
#if defined (CONFIG_PDMA_NEW)
		if(rx_ring[rx_dma_owner_idx].rxd_info4.L4VLD) {
			rx_skb->ip_summed = CHECKSUM_UNNECESSARY;
		}else {
		    rx_skb->ip_summed = CHECKSUM_NONE;
		}
#else
		if(rx_ring[rx_dma_owner_idx].rxd_info4.IPFVLD_bit) {
			rx_skb->ip_summed = CHECKSUM_UNNECESSARY;
		}else { 
		    rx_skb->ip_summed = CHECKSUM_NONE;
		}
#endif
#else
		    rx_skb->ip_summed = CHECKSUM_NONE;
#endif

#if defined(CONFIG_RA_CLASSIFIER)||defined(CONFIG_RA_CLASSIFIER_MODULE)
		/* Qwert+
		 */
		if(ra_classifier_hook_rx!= NULL)
		{
#if defined(CONFIG_RALINK_EXTERNAL_TIMER)
			ra_classifier_hook_rx(rx_skb, (*((volatile u32 *)(0xB0000D08))&0x0FFFF));
#else			
			ra_classifier_hook_rx(rx_skb, read_c0_count());
#endif			
		}
#endif /* CONFIG_RA_CLASSIFIER */

#if defined (CONFIG_RA_HW_NAT)  || defined (CONFIG_RA_HW_NAT_MODULE)
		if(ra_sw_nat_hook_rx != NULL) {
		    FOE_MAGIC_TAG(rx_skb)= FOE_MAGIC_GE;
		    *(uint32_t *)(FOE_INFO_START_ADDR(rx_skb)+2) = *(uint32_t *)&rx_ring[rx_dma_owner_idx].rxd_info4;
		    FOE_ALG(rx_skb) = 0;
		}
#endif

		/* We have to check the free memory size is big enough
		 * before pass the packet to cpu*/
#if defined (CONFIG_RAETH_SKB_RECYCLE_2K)
                skb = skbmgr_dev_alloc_skb2k();
#else
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,0)
        skb = alloc_skb(MAX_RX_LENGTH + NET_IP_ALIGN, GFP_ATOMIC);
#else
		skb = __dev_alloc_skb(MAX_RX_LENGTH + NET_IP_ALIGN, GFP_ATOMIC);
#endif
#endif

		if (unlikely(skb == NULL))
		{
			printk(KERN_ERR "skb not available...\n");
#ifdef CONFIG_PSEUDO_SUPPORT
			if (rx_ring[rx_dma_owner_idx].rxd_info4.SP == 2) {
				if (ei_local->PseudoDev != NULL) {
					pAd = netdev_priv(ei_local->PseudoDev);
					pAd->stat.rx_dropped++;
				}
			} else
#endif
				ei_local->stat.rx_dropped++;
                        bReschedule = 1;
			break;
		}
#if !defined (CONFIG_RAETH_SCATTER_GATHER_RX_DMA)
		skb_reserve(skb, NET_IP_ALIGN);
#endif

/*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.01*/
#if 0
#if defined (CONFIG_RAETH_SPECIAL_TAG)
		// port0: 0x8100 => 0x8100 0001
		// port1: 0x8101 => 0x8100 0002
		// port2: 0x8102 => 0x8100 0003
		// port3: 0x8103 => 0x8100 0004
		// port4: 0x8104 => 0x8100 0005
		// port5: 0x8105 => 0x8100 0006
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,21)
		veth = (struct vlan_ethhdr *)(rx_skb->mac_header);
#else
		veth = (struct vlan_ethhdr *)(rx_skb->mac.raw);
#endif
		/*donot check 0x81 due to MT7530 SPEC*/
		//if((veth->h_vlan_proto & 0xFF) == 0x81) 
		{
		    veth->h_vlan_TCI = htons( (((veth->h_vlan_proto >> 8) & 0xF) + 1) );
		    rx_skb->protocol = veth->h_vlan_proto = htons(ETH_P_8021Q);
		}
#endif
#endif
/*END:  Sync from T5214EWL-OM(RT5350),2015.06.01*/

/* ra_sw_nat_hook_rx return 1 --> continue
 * ra_sw_nat_hook_rx return 0 --> FWD & without netif_rx
 */
#if !defined(CONFIG_RA_NAT_NONE)
         if((ra_sw_nat_hook_rx == NULL) || 
	    (ra_sw_nat_hook_rx!= NULL && ra_sw_nat_hook_rx(rx_skb)))
#endif
         {
#if defined (CONFIG_RALINK_RT3052_MP2)
	       if(mcast_rx(rx_skb)==0) {
		   kfree_skb(rx_skb);
	       }else
#endif
#if defined (CONFIG_RAETH_LRO)
	       if (rx_skb->ip_summed == CHECKSUM_UNNECESSARY) {
		       lro_receive_skb(&ei_local->lro_mgr, rx_skb, NULL);
		       //LroStatsUpdate(&ei_local->lro_mgr,0);
	       } else
#endif
#ifdef CONFIG_RAETH_NAPI
                /*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.01*/
                #if 0 
                netif_receive_skb(rx_skb);
                #else
                if (is_forward != 1)
                {
					netif_receive_skb(rx_skb);
                    if(to_wangw && to_wanbr)
                    {
                        if (skb_cp) 
    					{
                             netif_receive_skb(skb_cp);
                        }
                    }
                }
                #endif
                /*END:  Sync from T5214EWL-OM(RT5350),2015.06.01*/                
#else
#ifdef CONFIG_RAETH_HW_VLAN_RX
	        if(ei_local->vlgrp && rx_ring[rx_dma_owner_idx].rxd_info2.TAG) {
			vlan_hwaccel_rx(rx_skb, ei_local->vlgrp, rx_ring[rx_dma_owner_idx].rxd_info3.VID);
		} else {
		    if (is_forward != 1)
            {
		        netif_rx(rx_skb);
		    }
		}
#else
#ifdef CONFIG_RAETH_CPU_LOOPBACK
                skb_push(rx_skb,ETH_HLEN);
                ei_start_xmit(rx_skb, dev, 1);
#else
                /*BEGIN:Sync from T5214EWL-OM(RT5350),2015.06.01*/
                #if 0
                netif_rx(rx_skb);
                #else
                if (is_forward != 1)
                {
                    netif_rx(rx_skb);
                    if(to_wangw && to_wanbr)
                    {
                        if (skb_cp) 
    					{
                            netif_rx(skb_cp);
                        }
                    }
                }
                #endif
                /*END:  Sync from T5214EWL-OM(RT5350),2015.06.01*/				
#endif
#endif
#endif
         }

#ifdef CONFIG_PSEUDO_SUPPORT
		if (rx_ring[rx_dma_owner_idx].rxd_info4.SP == 2) {
			if (ei_local->PseudoDev != NULL) {
				pAd = netdev_priv(ei_local->PseudoDev);
				pAd->stat.rx_packets++;
				pAd->stat.rx_bytes += length;
			}
		} else
#endif
		{
			ei_local->stat.rx_packets++;
			ei_local->stat.rx_bytes += length;
		}


#if defined (CONFIG_RAETH_SCATTER_GATHER_RX_DMA)
		rx_ring[rx_dma_owner_idx].rxd_info2.PLEN0 = MAX_RX_LENGTH;
		rx_ring[rx_dma_owner_idx].rxd_info2.LS0 = 0;
#endif
		rx_ring[rx_dma_owner_idx].rxd_info2.DDONE_bit = 0;
		rx_ring[rx_dma_owner_idx].rxd_info1.PDP0 = dma_map_single(NULL, skb->data, MAX_RX_LENGTH, PCI_DMA_FROMDEVICE);
#ifdef CONFIG_32B_DESC
		dma_cache_sync(NULL, &rx_ring[rx_dma_owner_idx], sizeof(struct PDMA_rxdesc), DMA_TO_DEVICE);
#endif

		/*  Move point to next RXD which wants to alloc*/
#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
		if(rx_ring_no==0) {
		    sysRegWrite(RAETH_RX_CALC_IDX0, rx_dma_owner_idx);
		    ei_local->netrx0_skbuf[rx_dma_owner_idx] = skb;
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
		    rx_calc_idx0 = rx_dma_owner_idx;
#endif
		}else {
		    sysRegWrite(RAETH_RX_CALC_IDX1, rx_dma_owner_idx);
		    ei_local->netrx1_skbuf[rx_dma_owner_idx] = skb;
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
		    rx_calc_idx1 = rx_dma_owner_idx;
#endif
		}
#else
		sysRegWrite(RAETH_RX_CALC_IDX0, rx_dma_owner_idx);
		ei_local->netrx0_skbuf[rx_dma_owner_idx] = skb;
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
		rx_calc_idx0 = rx_dma_owner_idx;
#endif
#endif

		
		/* Update to Next packet point that was received.
		 */
#if defined (CONFIG_RAETH_MULTIPLE_RX_RING)
		if(rx_ring_no==0) {
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
			rx_dma_owner_idx0 = (rx_dma_owner_idx + 1) % NUM_RX_DESC;
#else
			rx_dma_owner_idx0 = (sysRegRead(RAETH_RX_CALC_IDX0) + 1) % NUM_RX_DESC;
#endif
		}else {
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
			rx_dma_owner_idx1 = (rx_dma_owner_idx + 1) % NUM_RX_DESC;
#else
			rx_dma_owner_idx1 = (sysRegRead(RAETH_RX_CALC_IDX1) + 1) % NUM_RX_DESC;
#endif
		}
#else
#ifdef CONFIG_RAETH_RW_PDMAPTR_FROM_VAR
		rx_dma_owner_idx0 = (rx_dma_owner_idx + 1) % NUM_RX_DESC;
#else
		rx_dma_owner_idx0 = (sysRegRead(RAETH_RX_CALC_IDX0) + 1) % NUM_RX_DESC;
#endif
#endif
	}	/* for */

#if defined (CONFIG_RAETH_LRO)
	if (lro_flush_needed) {
		//LroStatsUpdate(&ei_local->lro_mgr,1);
		lro_flush_all(&ei_local->lro_mgr);
		lro_flush_needed = 0;
	}
#endif
	return bReschedule;
}
