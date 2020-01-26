====================================================================================
//wrap device table
typedef struct wrap_devt
{
    struct wrap_com 		*wdt_wc;        /*back ptr to wrap com*/
    wrap_devt_lock_t        	wdt_lock; 	/*lock for the dev table*/
    u_int8_t			wdt_dev_cnt;	/*device count*/
    u_int8_t			wdt_dev_cnt_vma;/*device count*/
    TAILQ_HEAD(,_osif_dev)	wdt_dev;	/*head for device list*/
    ATH_LIST_HEAD(,_osif_dev)	wdt_hash[WRAP_DEV_HASHSIZE]; /*head for device hash*/
    TAILQ_HEAD(,_osif_dev)	wdt_dev_vma;	/*head for device list*/
    ATH_LIST_HEAD(,_osif_dev)	wdt_hash_vma[WRAP_DEV_HASHSIZE]; /*head for device hash*/
} wrap_devt_t;

//wrap common struct
typedef struct wrap_com
{
    struct wrap_devt		wc_devt;	/*wrap device table*/
    struct nf_hook_ops 		*wc_nfops;	/*wrap nf hook ops*/
    struct proc_dir_entry 	*wc_proc_entry;
    const char                  *wc_proc_name;
    u_int8_t 			wc_isolation;
	int                     wc_use_cnt;	/*wrap comm use cnt*/
	char                    wc_addr[IEEE80211_ADDR_LEN];
    wlan_dev_t                  comhandle;
} wrap_com_t;

#if ATH_SUPPORT_WRAP
    u_int8_t                          iv_psta:1,        /* VAP type is PSTA */
                                      iv_mpsta:1,       /* VAP type is MPSTA */
                                      iv_wrap:1,        /* VAP type is WRAP */
                                      iv_mat:1,         /* VAP type is MAT */
                                      iv_wired_pvap:1;  /* proxy vap for wired sta */
    u_int8_t                          iv_mat_addr[IEEE80211_ADDR_LEN];      /* mat address for PSTA */
    bool                              iv_isolation;
#endif

#if ATH_SUPPORT_WRAP
    u_int8_t            iv_no_event_handler;
    int (*iv_wrap_mat_tx)(struct ieee80211vap *out, wbuf_t);
    int (*iv_wrap_mat_rx)(struct ieee80211vap *in, wbuf_t);
#endif

vap->iv_wrap_mat_tx = ath_wrap_mat_tx;
vap->iv_wrap_mat_rx = ath_wrap_mat_rx;

vap->iv_wrap_mat_tx = ol_if_wrap_mat_tx;
vap->iv_wrap_mat_rx = ol_if_wrap_mat_rx;
============================== wrapd 创建vap =================
./wrapd_api.c:739:	if ((i = ioctl(wrapg->ioctl_sock, SIOC80211IFCREATE, &ifr)) != 0) {
./wrapd_api.c:740:        wrapd_printf("ioctl(SIOC80211IFCREATE)");


./wrapd_api.c:759:	if (ioctl(wrapg->ioctl_sock, SIOC80211IFDESTROY, &ifr) < 0){
./wrapd_api.c:760:        wrapd_printf("ioctl(SIOC80211IFDESTROY)");



============================== wrapd 删除vap ====================
ll@ll-virtual-machine:~/szs_test/qca-networking-2016-spf-4-0_qca_oem/qca-networking-2016-spf-4-0_qca_oem.git/qsdk/build_dir/target-mips_r2_uClibc-0.9.33.2/qca-wrapd-g$ grep -rn "ioctl" .
Binary file ./ipkg-ar71xx/qca-wrapd-10.4/usr/sbin/wrapd matches
./bridge.h:39:	int ioctl_sock;
./bridge.c:7:#include <sys/ioctl.h>
./bridge.c:12:#include "linux_ioctl.h"
./bridge.c:141:    data->ioctl_sock=0;
./bridge.c:142:    if((data->ioctl_sock=socket(AF_LOCAL, SOCK_STREAM, 0))<0)
./bridge.c:144:    	wrapd_printf("error in ioctl socket opening");
Binary file ./wrapd_br.o matches
Binary file ./wrapd_api.o matches
Binary file ./main.o matches
./wrapd_br.c:7:#include <sys/ioctl.h>
./wrapd_br.c:27:static int br_dev_ioctl(struct input *data, unsigned long arg0,
./wrapd_br.c:42:    return ioctl(data->ioctl_sock, SIOCDEVPRIVATE, &ifr);
./wrapd_br.c:76:    else if(data->ioctl_sock >= 0)
./wrapd_br.c:80:	    read_complete = br_dev_ioctl(data, BRCTL_GET_FDB_ENTRIES, (unsigned long)entry, chunk, offset);
./wrapd_br.c:110:    if(data->ioctl_sock >= 0)
./wrapd_br.c:113:        n=br_dev_ioctl(data, BRCTL_GET_PORT_LIST, (unsigned long)indices, MAX_PORTS, 0);
./wrapd_api.h:50:    int ioctl_sock;
Binary file ./wrapd matches
./main.c:485:	data->ioctl_sock=0;
./wrapd_api.c:8:#include <sys/ioctl.h>
./wrapd_api.c:13:#include "linux_ioctl.h"
./wrapd_api.c:155:	if ((ioctl(wrapg->ioctl_sock, IEEE80211_IOCTL_KICKMAC, &iwr)) < 0)
./wrapd_api.c:157:		wrapd_printf("Error:ioctl IEEE80211_IOCTL_KICKMAC");
./wrapd_api.c:173:	if (ioctl(wrapg->ioctl_sock, IEEE80211_IOCTL_GETPARAM, &iwr) < 0) {
./wrapd_api.c:174:		wrapd_printf("ioctl IEEE80211_IOCTL_GETPARAM err, ioctl(%d) op(%d)",
./wrapd_api.c:193:    if (ioctl(wrapg->ioctl_sock, SIOCGIFNAME, &ifr) != 0) {
./wrapd_api.c:194:        wrapd_printf("ioctl SIOCGIFNAME err");
./wrapd_api.c:283:    if(wrapg->ioctl_sock)
./wrapd_api.c:284:        close(wrapg->ioctl_sock);
./wrapd_api.c:400:    if (ioctl(wrapg->ioctl_sock, SIOCGIWRANGE, &iwr) < 0) {
./wrapd_api.c:401:        wrapd_printf("ERROR : ioctl[SIOCGIWRANGE]\n");
./wrapd_api.c:465:    if (ioctl(wrapg->ioctl_sock, SIOCGIFNAME, &ifr) != 0) {
./wrapd_api.c:466://        wrapd_printf("ioctl SIOCGIFNAME err");
./wrapd_api.c:478:    struct extended_ioctl_wrapper extended_cmd;
./wrapd_api.c:493:    if (ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr) != 0) {
./wrapd_api.c:494:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_ast_info err",ret);
./wrapd_api.c:739:	if ((i = ioctl(wrapg->ioctl_sock, SIOC80211IFCREATE, &ifr)) != 0) {
./wrapd_api.c:740:        wrapd_printf("ioctl(SIOC80211IFCREATE)");
./wrapd_api.c:759:	if (ioctl(wrapg->ioctl_sock, SIOC80211IFDESTROY, &ifr) < 0){
./wrapd_api.c:760:        wrapd_printf("ioctl(SIOC80211IFDESTROY)");
./wrapd_api.c:1604:    if (ioctl(fd, SIOCGIFFLAGS, &ifr) != 0) {
./wrapd_api.c:1605:        wrapd_printf("%s: ioctl(SIOCGIFFLAGS) failed", if_name);
./wrapd_api.c:1615:    if (ioctl(fd, SIOCSIFFLAGS, &ifr) != 0) {
./wrapd_api.c:1616:        wrapd_printf("%s: ioctl(SIOCGIFFLAGS) failed up=%d", if_name, up);
./wrapd_api.c:1662:    struct extended_ioctl_wrapper extended_cmd;
./wrapd_api.c:1693:        if(ioctl(wrapg->ioctl_sock, SIOCGIFHWADDR, &ifr) < 0)
./wrapd_api.c:1724:    if ((ret = ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr)) != 0) {
./wrapd_api.c:1725:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_proxy_noack_war err", ret);
./wrapd_api.c:1736:    if ((ret = ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr)) != 0) {
./wrapd_api.c:1737:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_mpsta mac addr err", ret);
./wrapd_api.c:1913:    if(ioctl(wrapg->ioctl_sock, SIOCGIFHWADDR, &ifr) < 0)
./wrapd_api.c:2524:	struct extended_ioctl_wrapper extended_cmd;
./wrapd_api.c:2537:    if ((ret = ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr)) != 0) {
./wrapd_api.c:2538:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_force_client_mcast err", ret);
./wrapd_api.c:2548:    if ((ret = ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr)) != 0) {
./wrapd_api.c:2549:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_max_priority_active_radio err", ret);
./wrapd_api.c:2570:        if(ioctl(wrapg->ioctl_sock, SIOCSIFHWADDR, &ifr) < 0)
./wrapd_api.c:2645:	struct extended_ioctl_wrapper extended_cmd;
./wrapd_api.c:2656:    if ((ret = ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr)) != 0) {
./wrapd_api.c:2657:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_max_priority_active_radio err", ret);
./wrapd_api.c:2686:    if ((ret = ioctl(wrapg->ioctl_sock, SIOCGATHEXTENDED, &ifr)) != 0) {
./wrapd_api.c:2687:        wrapd_printf("ret=%d ioctl SIOCGATHEXTENDED get_force_client_mcast err", ret);
./wrapd_api.c:2697:    if(ioctl(wrapg->ioctl_sock, SIOCSIFHWADDR, &ifr) < 0)
./wrapd_api.c:2903:    wrapg->ioctl_sock = socket(PF_INET, SOCK_DGRAM, 0);
./wrapd_api.c:2904:    if (wrapg->ioctl_sock < 0) {
./wrapd_api.c:2911:        close(wrapg->ioctl_sock);
./wrapd_api.c:2921:        close(wrapg->ioctl_sock);



