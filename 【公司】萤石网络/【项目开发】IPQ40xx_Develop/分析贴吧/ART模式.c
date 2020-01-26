IPQ40xx的SDK使用qcmbr来启动art模式。

/*============================================= qcmbr ===============================================*/
1、使用TCP作为通信管道，最多容纳4个客户端。
（1）
static int socketListen(struct _Socket *pOSSock)
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	res = listen(sockfd, 4);
	
（2）
void Qcmbr_Run(int port)
// Clear the client socket records
for ( it=0; it < MCLIENT; it++ )
{
	_ClientSocket[it] = 0;
}	

2、通过TCP接收到命令后，通过ioctl传输到内核
void cmd_send2 (void *buf, int len, unsigned char *respdata, unsigned int *responseNeeded, unsigned int devid)
	/* 发送命令，buf中存储着命令 */
	initCfg.ifr.ifr_data = (void *)buf
    DbgPrintf("cmd_send2 bufLen[%d] rspLen[%u]..Beg\n",len, *responseNeeded);
    if (ioctl(initCfg.sock, SIOCIOCTLTX99, &initCfg.ifr) < 0) {
       err(1, "ioctl");
       return;
    }
	
	/* 接收回应，数据存放在initCfg.ifr.ifr_data */
	*(unsigned int *)responseBuf = ATH_XIOCTL_UNIFIED_UTF_RSP;
	initCfg.ifr.ifr_data = (void *)responseBuf;
	error = ioctl(initCfg.sock, SIOCIOCTLTX99, &initCfg.ifr);	
	
3、命令格式
int processDiagPacket(int client, unsigned char *inputMsg, unsigned int cmdLen)
	/* 字节0 */
	inputMsg[0]
	subSystemDispatch = 0x4B,
	
	/* 字节2与字节3 */
	else if (((inputMsg[3] << 8) | inputMsg[2]) == WLANCmdCode)
	WLANCmdCode = 22,

	/* 
		下发到qcmbr的命令，包含19字节的头尾
		起始16字节为命令项
		最后3字节为checksum
	*/
	processTLVPacket(inputMsg, cmdLen - 19 - ESCAPEcnt, respdata, &nrespdata, devid))


/*============================================= tx99驱动 ===============================================*/
1、驱动通过SIOCIOCTLTX99识别tx99发送命令
static int ath_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
	#if defined(ATH_TX99_DIAG) && (!defined(ATH_PERF_PWR_OFFLOAD))
		case SIOCIOCTLTX99:
			if (ol_ath_target_start(scn)) {
				qdf_print("failed to start the target\n");
				return -1;
			}

			QDF_PRINT_INFO(ic->ic_print_idx, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "Call Tx99 ioctl %d \n",cmd);
			error = tx99_ioctl(dev, ATH_DEV_TO_SC(scn->sc_dev), cmd, ifr->ifr_data);
			break;
	#else
		case SIOCIOCTLTX99:
			if (ol_ath_target_start(scn)) {
				qdf_print("failed to start the target\n");
				return -1;
			}

			error = utf_unified_ioctl(scn,ifr);
			break;
	#endif

2、连接fireware
int ol_ath_target_start(struct ol_ath_softc_net80211 *scn)
	/* 对fireware连接已经创建完成，无需重新关联 */
    if (!(scn->down_complete)) {
        /*
         * Target has already been started
         */
        return 0;
    }	
	
	/* 关联fireware */
    if (ol_target_init(scn, false))
        goto fail;

    if (ol_target_init_complete(scn))
        goto fail;
    ol_ath_mgmt_attach(ic);
    ol_ath_stats_attach(ic);
    if (ol_ath_thermal_mitigation_attach(scn, dev))
        goto fail;

    ieee80211com_set_cap(ic, IEEE80211_C_WEP);
    ieee80211com_set_cap(ic, IEEE80211_C_AES);
    ieee80211com_set_cap(ic, IEEE80211_C_AES_CCM);
    ieee80211com_set_cap(ic, IEEE80211_C_CKIP);
    ieee80211com_set_cap(ic, IEEE80211_C_TKIP);
    ieee80211com_set_cap(ic, IEEE80211_C_TKIPMIC)
	
	scn->down_complete = false;

3、执行命令
u_int8_t tx99_ioctl(ath_dev_t device, struct ath_softc *sc, int cmd, void *addr)


4、tx99命令格式
typedef struct tx99_wcmd {
    char                if_name[IFNAMSIZ];/**< Interface name */
    tx99_wcmd_type_t    type;             /**< Type of wcmd */
    tx99_wcmd_data_t    data;             /**< Data */       
} tx99_wcmd_t;

typedef enum {
    TX99_WCMD_ENABLE,
    TX99_WCMD_DISABLE,
    TX99_WCMD_SET_FREQ,
    TX99_WCMD_SET_RATE,
    TX99_WCMD_SET_RC,
    TX99_WCMD_SET_POWER,
    TX99_WCMD_SET_TXMODE,
    TX99_WCMD_SET_CHANMASK,
    TX99_WCMD_SET_TYPE,
    TX99_WCMD_SET_TESTMODE,
    TX99_WCMD_GET,
    TX99_WCMD_TEST,
} tx99_wcmd_type_t;

typedef struct tx99_wcmd_data {
    u_int32_t freq;     /* tx frequency (MHz) */
    u_int32_t htmode;   /* tx bandwidth (HT20/HT40) */
    u_int32_t htext;    /* extension channel offset (0(none), 1(plus) and 2(minus)) */
    u_int32_t rate;     /* Kbits/s */
    u_int32_t rc;       /* rate code */
    u_int32_t power;    /* (dBm) */
    u_int32_t txmode;   /* wireless mode, 11NG(8), auto-select(0) */
    u_int32_t chanmask; /* tx chain mask */
    u_int32_t type;
    u_int32_t testmode; /* tx data pattern, recv only */
} tx99_wcmd_data_t;

/*============================================= athtestcmd ===============================================*/
1、封装命令参数
void addParm (PARM_CODE parmCode, char *tlv1KeyWord, A_UINT32 value)
{
#ifdef USE_TLV2
	tlv2AddParms(2, parmCode, value);
#else
    addParameterToCommand((A_UINT8 *)tlv1KeyWord, (A_UINT8 *)&value);
#endif //USE_TLV2
}

addParameterToCommand封装在库中，无法查看命令的打包与解析源码。