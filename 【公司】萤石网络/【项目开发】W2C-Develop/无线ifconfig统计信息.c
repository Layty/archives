/*
* Return netdevice statistics.
*/
static struct net_device_stats *
    osif_getstats(struct net_device *dev)
{
    osif_dev  *osifp = ath_netdev_priv(dev);
    wlan_if_t vap = osifp->os_if;
    struct net_device_stats *stats = &osifp->os_devstats;
    struct ieee80211_stats *vapstats;
    const struct ieee80211_mac_stats *unimacstats ;
    const struct ieee80211_mac_stats *multimacstats;

    if (!vap || osifp->is_delete_in_progress || (vap->iv_opmode == IEEE80211_M_MONITOR)) {
	return stats;
    }

#if QCA_PARTNER_DIRECTLINK_RX
    /* get netdevice statistics through partner API */
    if (qdf_unlikely(osifp->is_directlink)) {
	    return osif_getstats_partner(dev);
    }
#endif /* QCA_PARTNER_DIRECTLINK_RX */

    vapstats = wlan_get_stats(vap);
    unimacstats = wlan_mac_stats(vap, 0);
    multimacstats = wlan_mac_stats(vap, 1);
    stats->rx_packets = (unsigned long)(unimacstats->ims_rx_packets + multimacstats->ims_rx_packets);
    stats->tx_packets = (unsigned long)(unimacstats->ims_tx_packets + multimacstats->ims_tx_packets);

    /* XXX total guess as to what to count where */
    /* update according to private statistics */

    stats->tx_bytes = (unsigned long)(unimacstats->ims_tx_bytes+ multimacstats->ims_tx_bytes);

    stats->tx_errors = vapstats->is_tx_nodefkey
        + vapstats->is_tx_noheadroom
        + vapstats->is_crypto_enmicfail
        + vapstats->is_tx_not_ok
        ;
    stats->tx_dropped = vapstats->is_tx_nobuf
        + vapstats->is_tx_nonode
        + vapstats->is_tx_unknownmgt
        + vapstats->is_tx_badcipher
        + vapstats->is_tx_nodefkey
        ;

    stats->rx_bytes = (unsigned long)(unimacstats->ims_rx_bytes+ multimacstats->ims_rx_bytes);

	/* 关注接收错包 */
	stats->rx_errors = vapstats->is_rx_tooshort
			+ unimacstats->ims_rx_wepfail
			+ multimacstats->ims_rx_wepfail
			+ vapstats->is_rx_decap
			+ vapstats->is_rx_nobuf
            + unimacstats->ims_rx_decryptcrc
            + multimacstats->ims_rx_decryptcrc
            + unimacstats->ims_rx_ccmpmic
            + multimacstats->ims_rx_ccmpmic
            + unimacstats->ims_rx_tkipmic
            + multimacstats->ims_rx_tkipmic
            + unimacstats->ims_rx_tkipicv
            + multimacstats->ims_rx_tkipicv
            + unimacstats->ims_rx_wpimic
            + multimacstats->ims_rx_wpimic
			;
	stats->rx_crc_errors = 0;
	
	/* 接收的丢包数等于接收的错误数 */
    stats->rx_dropped = stats->rx_errors;


    return stats;
}

