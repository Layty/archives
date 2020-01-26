struct msdc_host
{
    struct msdc_hw              *hw;

    struct mmc_host             *mmc;           /* mmc structure */
    struct mmc_command          *cmd;
    struct mmc_data             *data;
    struct mmc_request          *mrq; 
    int                         cmd_rsp;
    int                         cmd_rsp_done;
    int                         cmd_r1b_done;

    int                         error; 
    spinlock_t                  lock;           /* mutex */
    struct semaphore            sem; 

    u32                         blksz;          /* host block size */
    u32                         base;           /* host base address */    
    int                         id;             /* host id */
    int                         pwr_ref;        /* core power reference count */

    u32                         xfer_size;      /* total transferred size */

    struct msdc_dma             dma;            /* dma channel */
    u32                         dma_addr;       /* dma transfer address */
    u32                         dma_left_size;  /* dma transfer left size */
    u32                         dma_xfer_size;  /* dma transfer size in bytes */
    int                         dma_xfer;       /* dma transfer mode */

    u32                         timeout_ns;     /* data timeout ns */
    u32                         timeout_clks;   /* data timeout clks */

    atomic_t                    abort;          /* abort transfer */

    int                         irq;            /* host interrupt */

    struct tasklet_struct       card_tasklet;

    struct completion           cmd_done;
    struct completion           xfer_done;
    struct pm_message           pm_state;

    u32                         mclk;           /* mmc subsystem clock */
    u32                         hclk;           /* host clock speed */		
    u32                         sclk;           /* SD/MS clock speed */
    u8                          core_clkon;     /* Host core clock on ? */
    u8                          card_clkon;     /* Card clock on ? */
    u8                          core_power;     /* core power */    
    u8                          power_mode;     /* host power mode */
    u8                          card_inserted;  /* card inserted ? */
    u8                          suspend;        /* host suspended ? */    
    u8                          reserved;
    u8                          app_cmd;        /* for app command */     
    u32                         app_cmd_arg;    
    u64                         starttime;
};

struct mmc_host
{
    struct mmc_card *card;
    u32 max_hw_segs;
    u32 max_phys_segs;
    u32 max_seg_size;
    u32 max_blk_size;
    u32 max_blk_count;
    u32 base;         /* host base address */
    u32 caps;         /* Host capabilities */
    u32 f_min;        /* host min. frequency */
    u32 f_max;        /* host max. frequency */
    u32 clk;          /* host clock speed */
    u32 sclk;         /* SD/MS clock speed */
    u32 blklen;       /* block len */
    u32 blkbits;      /* block bits */
    u32 ocr;          /* current ocr */
    u32 ocr_avail;    /* available ocr */    
    u32 timeout_ns;   /* data timeout ns */
    u32 timeout_clks; /* data timeout clks */
    u8  clksrc;       /* clock source */
    u8  id;           /* host id number */
    u8  boot_type;    /* boot type */
	bool card_detect_flag; /*flag for card detection*/
    void *priv;       /* private data */
    int (*blk_read)(struct mmc_host *host, uchar *dst, ulong src, ulong nblks);
    int (*blk_write)(struct mmc_host *host, ulong dst, uchar *src, ulong nblks);
};

/*------------------ mmc\host\mtk-mmc\sd.c --------------------------------------------------------*/
/* module init/exit 
	MT7620A SD卡驱动模块初始化函数
*/
/*--------------------------------------------------------------------------*/
static int __init mt_msdc_init(void)
{
    int ret;
/* +++ by chhung */
    u32 reg;

#if defined (CONFIG_MTD_ANY_RALINK)
    extern int ra_check_flash_type(void);
    if(ra_check_flash_type() == 2) { /* NAND */
	    printk("%s: !!!!! SDXC Module Initialize Fail !!!!!", __func__);
	    return 0;
    }
#endif
    mtk_sd_device.dev.platform_data = &msdc0_hw;
    printk("MTK MSDC device init.\n");
    platform_add_devices(mtk_mmc_devices, ARRAY_SIZE(mtk_mmc_devices));
	//设置MT7620A SD卡相关引脚模式，默认为GPIO模式，设置成SD MODE[寄存器0x60，GPIO模式寄存器，可查看芯片手册。
    reg = sdr_read32((volatile u32*)(RALINK_SYSCTL_BASE + 0x60)) & ~(0x3<<18);
#if defined (CONFIG_RALINK_MT7620)
    reg |= 0x1 << 18;
#endif
    sdr_write32((volatile u32*)(RALINK_SYSCTL_BASE + 0x60), reg);
/* end of +++ */
    ret = platform_driver_register(&mt_msdc_driver);
    if (ret) {
        printk(KERN_ERR DRV_NAME ": Can't register driver");
        return ret;
    }
    printk(KERN_INFO DRV_NAME ": MediaTek MT6575 MSDC Driver\n");

    msdc_debug_proc_init();
    return 0;
}

/*------------------ mmc\host\mtk-mmc\borad.h ----------------------------------*/
#define MSDC_CD_PIN_EN      (1 << 0)  /* card detection pin is wired   */
#define MSDC_WP_PIN_EN      (1 << 1)  /* write protection pin is wired */
#define MSDC_RST_PIN_EN     (1 << 2)  /* emmc reset pin is wired       */
#define MSDC_SDIO_IRQ       (1 << 3)  /* use internal sdio irq (bus)   */
#define MSDC_EXT_SDIO_IRQ   (1 << 4)  /* use external sdio irq         */
#define MSDC_REMOVABLE      (1 << 5)  /* removable slot                */
#define MSDC_SYS_SUSPEND    (1 << 6)  /* suspended by system           */
#define MSDC_HIGHSPEED      (1 << 7)  /* high-speed mode support       */
#define MSDC_UHS1           (1 << 8)  /* uhs-1 mode support            */
#define MSDC_DDR            (1 << 9)  /* ddr mode support              */

#define MSDC_SMPL_RISING    (0)
#define MSDC_SMPL_FALLING   (1)

#define MSDC_CMD_PIN        (0)
#define MSDC_DAT_PIN        (1)
#define MSDC_CD_PIN         (2)
#define MSDC_WP_PIN         (3)
#define MSDC_RST_PIN        (4)

mt_msdc_init
	|-mt_msdc_driver[platform_driver_register(&mt_msdc_driver)]
		@-msdc_drv_probe
			|-mmc_alloc_host
				|-mmc_rescan[INIT_DELAYED_WORK(&host->detect, mmc_rescan)]
					|-mmc_attach_sd
						|-mmc_sd_attach_bus_ops
							@-mmc_sd_ops

							
							
##### drivers\mmc\host\mtk-mmc\sd.c #####							
static void msdc_tasklet_card(unsigned long arg)
{
    struct msdc_host *host = (struct msdc_host *)arg;
    struct msdc_hw *hw = host->hw;
    u32 base = host->base;
    u32 inserted;	
    u32 status = 0;
    //u32 change = 0;

    spin_lock(&host->lock);

    if (hw->get_cd_status) { // NULL
        inserted = hw->get_cd_status();
    } else {
        status = sdr_read32(MSDC_PS);
        inserted = (status & MSDC_PS_CDSTS) ? 0 : 1;
    }

#if 0
    change = host->card_inserted ^ inserted;
    host->card_inserted = inserted;
	
    if (change && !host->suspend) {
        if (inserted) {
            host->mmc->f_max = HOST_MAX_MCLK;  // work around          	
        }     	
        mmc_detect_change(host->mmc, msecs_to_jiffies(20));
    }
#else  /* Make sure: handle the last interrupt */
    host->card_inserted = inserted;    
    
    if (!host->suspend) {
        host->mmc->f_max = HOST_MAX_MCLK;    	
        mmc_detect_change(host->mmc, msecs_to_jiffies(20));
    }   
    
    IRQ_MSG("card found<%s>", inserted ? "inserted" : "removed");  	
#endif

    spin_unlock(&host->lock);
}


/* ops.get_cd */
static int msdc_ops_get_cd(struct mmc_host *mmc)
{
    struct msdc_host *host = mmc_priv(mmc);
    u32 base = host->base;    
    unsigned long flags;
    int present = 1;

    /* for sdio, MSDC_REMOVABLE not set, always return 1 */
    if (!(host->hw->flags & MSDC_REMOVABLE)) {
        /* For sdio, read H/W always get<1>, but may timeout some times */       	    	
#if 1
        host->card_inserted = 1;       
        return 1;
#else
        host->card_inserted = (host->pm_state.event == PM_EVENT_USER_RESUME) ? 1 : 0; 
        INIT_MSG("sdio ops_get_cd<%d>", host->card_inserted);
        return host->card_inserted; 
#endif
    }

    /* MSDC_CD_PIN_EN set for card */
    if (host->hw->flags & MSDC_CD_PIN_EN) {
        spin_lock_irqsave(&host->lock, flags);
#if 0        
        present = host->card_inserted;  /* why not read from H/W: Fix me*/
#else
        present = (sdr_read32(MSDC_PS) & MSDC_PS_CDSTS) ? 0 : 1; 
        host->card_inserted = present;  
#endif        
        spin_unlock_irqrestore(&host->lock, flags);
    } else {
        present = 0; /* TODO? Check DAT3 pins for card detection */
    }

    INIT_MSG("ops_get_cd return<%d>", present);
    return present;
}

struct msdc_hw msdc0_hw = {
	.clk_src        = 0,
	.cmd_edge       = MSDC_SMPL_FALLING,
	.data_edge      = MSDC_SMPL_FALLING,
	.clk_drv        = 4,
	.cmd_drv        = 4,
	.dat_drv        = 4,
	.data_pins      = 4,
	.data_offset    = 0,
	.flags          = MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE | MSDC_HIGHSPEED,
//	.flags          = MSDC_SYS_SUSPEND | MSDC_WP_PIN_EN | MSDC_CD_PIN_EN | MSDC_REMOVABLE,
};
