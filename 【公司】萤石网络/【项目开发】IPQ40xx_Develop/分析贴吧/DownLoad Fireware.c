======================= offload 下发固件到target设备 ================
int ol_ath_download_firmware(struct ol_ath_softc_net80211 *scn)

第一步：
传输flash中的校准数据
	/* Write EEPROM or Flash data to Target RAM */
	QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "\n Flash Download Address  %x \n",address);
	status = ol_transfer_bin_file(scn, ATH_FLASH_FILE, address, FALSE);
	if(status != EOK) {
		flash_download_fail = 1;
	}

第二步：
传输OTP文件
	QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, KERN_INFO"%s: Download OTP, flash download ADDRESS 0x%x\n",__func__,address);
	address = BMI_SEGMENTED_WRITE_ADDR;
#if defined(ATH_CONFIG_FW_SIGN)
	status = ol_transfer_bin_file(scn, ATH_OTP_FILE, address, FALSE);
#else
	status = ol_transfer_bin_file(scn, ATH_OTP_FILE, address, TRUE);
#endif

第三步：
传输boarddata文件
	QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, KERN_INFO"%s: BOARDDATA DOWNLOAD TO address 0x%x\n",__func__, address);
	/* Flash is either not available or invalid */
	if (ol_transfer_bin_file(scn, ATH_BOARD_DATA_FILE, address, FALSE) != EOK) {
		phase = 1 ;
		goto failed;
	}
	
第四步：
传输固件即驱动程序
    /* Download Target firmware - TODO point to target specific files in runtime */
    address = BMI_SEGMENTED_WRITE_ADDR;
#if defined(ATH_CONFIG_FW_SIGN)
    if(ol_transfer_bin_file(scn, ATH_FIRMWARE_FILE, address, FALSE) != EOK) {
#else
    if (ol_transfer_bin_file(scn, ATH_FIRMWARE_FILE, address, TRUE) != EOK) {
#endif

failed:
   QDF_PRINT_INFO(QDF_PRINT_IDX_SHARED, QDF_MODULE_ID_ANY, QDF_TRACE_LEVEL_INFO, "%s : ol_transfer_bin_file failed. phase:%d \n",__func__,phase);