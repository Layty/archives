/*==========================  AP模式下事件定义与注册  ============================================*/
wlan_mlme_event_handler_table     *iv_mlme_evtable[IEEE80211_MAX_VAP_MLME_EVENT_HANDLERS];  /* mlme event handlers */

static void osif_vap_setup(wlan_if_t vap, struct net_device *dev,enum ieee80211_opmode opmode)
wlan_vap_register_mlme_event_handlers(vap,(os_handle_t)osifp,&ap_mlme_evt_handler);
static wlan_mlme_event_handler_table ap_mlme_evt_handler = {
	osif_create_infra_complete,
    NULL,
    osif_auth_complete_ap,
    NULL,
    osif_assoc_complete_ap,
    osif_assoc_complete_ap,
    osif_deauth_complete_ap,
    osif_disassoc_complete_ap,
    NULL,                                 /* mlme_txchanswitch_complete */
    NULL,                                 /* mlme_repeater_cac_complete */
    osif_auth_indication_ap,
    osif_deauth_indication_ap,
    osif_assoc_indication_ap,
    osif_assoc_indication_ap,
    osif_leave_indication_ap,
    NULL,
    NULL,
    NULL,
    osif_node_authorized_indication_ap,   /* wlan_node_authorized_indication */
    NULL,                                 /* unprotected deauth indication */
} ;

static void osif_vap_setup(wlan_if_t vap, struct net_device *dev,enum ieee80211_opmode opmode)
wlan_vap_register_misc_event_handlers(vap,(os_handle_t)osifp,&ap_misc_evt_handler);
static wlan_misc_event_handler_table ap_misc_evt_handler = {
    osif_channel_change,
    osif_country_changed,
    osif_linkspeed,
    osif_michael_failure_indication,
    osif_replay_failure_indication,
    NULL,
    NULL,                               /* wlan_beacon_rssi_indication */
    NULL,
    NULL,
    NULL,
    NULL,                               /* wlan_ap_stopped */
#if ATH_SUPPORT_WAPI
    osif_rekey_indication_ap,
#endif
#if ATH_SUPPORT_IBSS_NETLINK_NOTIFICATION
    NULL,
#endif
#if UMAC_SUPPORT_RRM_MISC
    osif_chload,
    osif_nonerpcnt,
    osif_bgjoin,
    osif_cochannelap_cnt,
#endif
#if ATH_SUPPORT_HYFI_ENHANCEMENTS
   osif_buffull_warning,                                     /* wlan_buffull */
#endif
    osif_session_timeout,                   /* wlan_session_timeout */
#if ATH_SUPPORT_MGMT_TX_STATUS
   osif_mgmt_tx_status,
#endif
#if ATH_BAND_STEERING
    osif_band_steering_event , /* void (*bsteering_event)(os_handle_t,enum,char eventlen,char *data); */
#endif
#if ATH_SSID_STEERING
    osif_ssid_event,   /* ssid_event , void (*ssid_event)(os_handle_t, u_int8_t *macaddr); */
#endif
#if UMAC_SUPPORT_ACL
    osif_assocdeny_event,   /* assocdeny_event */
#endif /*UMAC_SUPPORT_ACL*/
#if ATH_TX_OVERFLOW_IND
    osif_wlan_tx_overflow_event,            /* wlan_tx_overflow */
#endif
    osif_ch_hop_channel_change , /*wlan_ch_hop_channel_change*/
    osif_recv_probereq,     /*  wlan_recv_probereq */
#if DBDC_REPEATER_SUPPORT
    NULL,                /*stavap_connection*/
#endif
#if ATH_SUPPORT_DFS && ATH_SUPPORT_STA_DFS
    NULL,                 /* wlan_sta_cac_started */
#endif
    NULL,
    osif_keyset_done_indication,/*keyset_done_indication*/
    osif_blklst_sta_auth_indication, /*blklst_sta_auth_indication*/
};


typedef struct _wlan_mlme_event_handler_table {
    /* MLME confirmation handler */
    void (*mlme_join_complete_infra)(os_handle_t, IEEE80211_STATUS);
    void (*mlme_join_complete_adhoc)(os_handle_t, IEEE80211_STATUS);
    void (*mlme_auth_complete)(os_handle_t, IEEE80211_STATUS);
    void (*mlme_assoc_req)(os_handle_t, wbuf_t wbuf);
    void (*mlme_assoc_complete)(os_handle_t, IEEE80211_STATUS, u_int16_t aid, wbuf_t wbuf);
    void (*mlme_reassoc_complete)(os_handle_t, IEEE80211_STATUS, u_int16_t aid, wbuf_t wbuf);
    void (*mlme_deauth_complete)(os_handle_t,u_int8_t *, IEEE80211_STATUS);
    void (*mlme_disassoc_complete)(os_handle_t, u_int8_t *, u_int32_t, IEEE80211_STATUS);
    void (*mlme_txchanswitch_complete)(os_handle_t,IEEE80211_STATUS);
    void (*mlme_repeater_cac_complete)(os_handle_t,IEEE80211_STATUS);

    /* MLME indication handler */
    void (*mlme_auth_indication)(os_handle_t, u_int8_t *macaddr, u_int16_t result);
    void (*mlme_deauth_indication)(os_handle_t, u_int8_t *macaddr, u_int16_t associd, u_int16_t reason_code);
    void (*mlme_assoc_indication)(os_handle_t, u_int8_t *macaddr, u_int16_t result, wbuf_t wbuf, wbuf_t resp_wbuf, bool reassoc);
    void (*mlme_reassoc_indication)(os_handle_t, u_int8_t *macaddr, u_int16_t result, wbuf_t wbuf, wbuf_t resp_wbuf, bool reassoc);
    void (*mlme_disassoc_indication)(os_handle_t, u_int8_t *macaddr, u_int16_t associd, u_int32_t reason_code);
    void (*mlme_ibss_merge_start_indication)(os_handle_t, u_int8_t *bssid);
    void (*mlme_ibss_merge_completion_indication)(os_handle_t, u_int8_t *bssid);
    void (*wlan_radar_detected) (os_handle_t, u_int32_t csa_delay);
    void (*wlan_node_authorized_indication) (os_handle_t, u_int8_t *macaddr);
    void (*mlme_unprotected_deauth_indication)(os_handle_t, u_int8_t *macaddr, u_int16_t associd, u_int16_t reason_code);
} wlan_mlme_event_handler_table;


/*=======================================   注册事件函数定义 =================================================*/
int
wlan_vap_register_mlme_event_handlers(wlan_if_t vaphandle,
                                 os_handle_t oshandle,
                                 wlan_mlme_event_handler_table *evtable)
{
    return ieee80211_vap_register_mlme_events(vaphandle, oshandle, evtable);
}

int
ieee80211_vap_register_mlme_events(struct ieee80211vap *vap, os_handle_t oshandle, wlan_mlme_event_handler_table *evtab)
{
    int i;
    /* unregister if there exists one already */
    ieee80211_vap_unregister_mlme_events(vap,oshandle,evtab);
    IEEE80211_VAP_LOCK(vap);
    for (i=0;i<IEEE80211_MAX_VAP_MLME_EVENT_HANDLERS; ++i) {
        if ( vap->iv_mlme_evtable[i] == NULL) {
            vap->iv_mlme_evtable[i] = evtab;
            vap->iv_mlme_arg[i] = oshandle;
            IEEE80211_VAP_UNLOCK(vap);
            return 0;
        }
    }
    IEEE80211_VAP_UNLOCK(vap);
    IEEE80211_DPRINTF(vap, IEEE80211_MSG_MLME, "%s mlme evtable is full.\n", __func__);
    return ENOMEM;
}


==============================   使用事件    ==============================
int mlme_recv_auth_ap(struct ieee80211_node *ni,
                       u_int16_t algo, u_int16_t seq, u_int16_t status_code,
                       u_int8_t *challenge, u_int8_t challenge_length, wbuf_t wbuf,
                       const struct ieee80211_rx_status *rs)
if (indicate ) {
	IEEE80211_DELIVER_EVENT_MLME_AUTH_INDICATION(vap, ni->ni_macaddr,
													 indication_status);
}

#define IEEE80211_DELIVER_EVENT_MLME_AUTH_INDICATION( _vap,_macaddr,_indication_status)  do {        \
        int i;                                                                                       \
        for(i=0;i<IEEE80211_MAX_VAP_MLME_EVENT_HANDLERS; ++i) {                                      \
            if ((_vap)->iv_mlme_evtable[i] &&                                                        \
            (_vap)->iv_mlme_evtable[i]->mlme_auth_indication) {                                      \
                (* (_vap)->iv_mlme_evtable[i]->mlme_auth_indication)                                 \
                                           ((_vap)->iv_mlme_arg[i], _macaddr, _indication_status);   \
             }                                                                                       \
        }                                                                                            \
    } while(0)
		

static void
osif_auth_indication_ap(os_handle_t osif, u_int8_t *macaddr,
							u_int16_t status)
{
	struct net_device *dev = ((osif_dev *)osif)->netdev;
#if UMAC_SUPPORT_ACFG
	acfg_event_data_t *acfg_event = NULL;
#endif
	struct ev_msg msg;
	union iwreq_data wrqu;

#if UMAC_SUPPORT_ACFG
	acfg_event = (acfg_event_data_t *)kmalloc(sizeof(acfg_event_data_t), GFP_ATOMIC);
	if (acfg_event == NULL)
		return;

	acfg_event->result = status;
	IEEE80211_ADDR_COPY(acfg_event->addr, macaddr);
	acfg_event->downlink = 1;
	acfg_send_event(dev, ((osif_dev *)osif)->os_handle, WL_EVENT_TYPE_AUTH_AP, acfg_event);
#endif

	msg.status = status;
	IEEE80211_ADDR_COPY(msg.addr, macaddr);
	memset(&wrqu, 0, sizeof(wrqu));
	wrqu.data.flags = IEEE80211_EV_AUTH_IND_AP;
	wrqu.data.length = sizeof(msg);
	WIRELESS_SEND_EVENT(dev, IWEVCUSTOM, &wrqu, (char *)&msg);
#if UMAC_SUPPORT_ACFG
	kfree(acfg_event);
#endif
	return;
}
