
#include "app.h"
#include "rf_config.h"
#include "rf_app.h"

#include "uart_cmd.h"

#define rf_tick_time (0.0000032) // uSec
#define RF_TICK_100US (int)(0.0001/rf_tick_time)
#define RF_TICK_200US (int)(0.0002/rf_tick_time)
#define RF_TICK_300US (int)(0.0003/rf_tick_time)
#define RF_TICK_400US (int)(0.0004/rf_tick_time)
#define RF_TICK_500US (int)(0.0005/rf_tick_time)
#define RF_TICK_600US (int)(0.0006/rf_tick_time)
#define RF_TICK_700US (int)(0.0007/rf_tick_time)
#define RF_TICK_800US (int)(0.0008/rf_tick_time)
#define RF_TICK_900US (int)(0.0009/rf_tick_time)
#define RF_TICK_1MS (int)(0.001/rf_tick_time)
#define RF_TICK_2MS (int)(0.002/rf_tick_time)
#define RF_TICK_3MS (int)(0.003/rf_tick_time)
#define RF_TICK_4MS (int)(0.004/rf_tick_time)
#define RF_TICK_5MS (int)(0.005/rf_tick_time)
#define RF_TICK_6MS (int)(0.006/rf_tick_time)
#define RF_TICK_7MS (int)(0.007/rf_tick_time)
#define RF_TICK_8MS (int)(0.008/rf_tick_time)
#define RF_HOPPING_CH 16
#define RF_4M_TRANS_TIME (0.00465/rf_tick_time)
#define RF_6M_TRANS_TIME (0.0041/rf_tick_time)


//const uint8_t RF_CHANNEL_TABLE[RF_HOPPING_CH] = {6,24,43,66,10,28,49,69,14,33,52,72,18,37,57,75};
//const uint8_t RF_CHANNEL_TABLE[RF_HOPPING_CH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t RF_CHANNEL_TABLE[RF_HOPPING_CH] = {0, 80, 2, 82, 0, 80, 2, 82, 0, 80, 2, 82, 0, 80, 2, 82};

uint8_t *rfBufHead;
uint8_t *rfBufTail;
uint8_t *rfTaskPoint;

struct _pkt_info_s {
    uint16_t addr;
    uint16_t len;
};

#define PKT_6M_INFO_MAX 11 // len.(4+11*4=48) bytes Max, base on 8'
#define PKT_6M_ACK_LEN 48
#define PKT_4M_INFO_MAX 8 // len.(4+8*4=40) bytes Max, base on 8'
#define PKT_4M_ACK_LEN 40
#define PKT_SNED_PKT_INC 2
#define PKT_GET_INFOxM 5

typedef struct _sync_pkt_s {
    uint32_t type;
    //uint16_t reserve;
    uint32_t sTick;
} _sync_pkt_t;

typedef struct _info_pkt_s {
    uint32_t type;
    //uint16_t reserve;
    struct _pkt_info_s pkt[PKT_6M_INFO_MAX];
} _info_pkt_t;

typedef struct _send_pkt_info_s {
    uint16_t type;
    uint16_t reserve;
    struct _pkt_info_s pkt[PKT_6M_INFO_MAX + PKT_SNED_PKT_INC];
} _send_pkt_info_t;

_sync_pkt_t syncPkt;
_send_pkt_info_t infoPkt;
_send_pkt_info_t infoPktBackup;

uint8_t txInfoCnt = 0; // trans packet info count (<PKT_INFO_TX_MAX)
uint8_t infoCnt; //  info count at one packet (< PKT_INFO_RX_MAX)
uint8_t infoSync; // make sync get new data in rf_task_fifo each RF_TRANS_TIME
uint8_t getAckIrq; // make flag wait irq

uint8_t gCnt; // get packet count
struct _pkt_info_s getPkt[PKT_6M_INFO_MAX*PKT_GET_INFOxM]; // get packet buffer

uint8_t losePktCnt; // lose packet count
struct _pkt_info_s losePkt;

//uint8_t rf_fifo_buf[256]; //

#define PKT_LEN_MODIFY
#define PKT_LEN_MAX 252   // 256-(_pkt_info_s)
#define PKT_LEN_6MRF 236  // 240-(_pkt_info_s)
#define RF_UNLINK_MAX 20
uint8_t rfLinkCnt = 0; //

typedef enum {
    SYNC_PKT = 0xE100, //
    RX_PKT = 0xE200, //
    ACK_PKT = 0xE300, //
} _pkt_type;

void init_rf_data_fifo(void) {
    rfBufHead = rfTaskBuf;
    rfBufTail = rfTaskBuf;
    rfTaskPoint = rfTaskBuf;
    gCnt = 0;
    memset(getPkt, 0, sizeof (getPkt));
    memset(&infoPkt, 0, sizeof (infoPkt));
    memset(&infoPktBackup, 0, sizeof (infoPkt));
    losePktCnt = 0;
    losePkt.len = 0;
    //
    if (rfStatus.rate == FSK_2G_6M) {
        rfStatus.pktTickMax = RF_6M_TRANS_TIME;
        rfStatus.pktInfoMax = PKT_6M_INFO_MAX;
        rfStatus.pktAckByte = PKT_6M_ACK_LEN;
    } else {
        rfStatus.pktTickMax = RF_4M_TRANS_TIME;
        rfStatus.pktInfoMax = PKT_4M_INFO_MAX;
        rfStatus.pktAckByte = PKT_4M_ACK_LEN;

    }
}

uint16_t get_rf_data_fifo_space(void) {
    if (rfBufHead > rfBufTail) {
        return sizeof (rfTaskBuf) - (rfBufHead - rfBufTail) - 1;
    } else if (rfBufHead < rfBufTail) {
        return (rfBufTail - rfBufHead) - 1;
    } else {
        return sizeof (rfTaskBuf) - 1;
    }
}

void write_rf_data_fifo(uint8_t* data, uint16_t len) {
    while (len) {
        *rfBufHead = *data;
        rfBufHead++;
        data++;
        if (rfBufHead >= rfTaskBuf + sizeof (rfTaskBuf)) {
            rfBufHead = rfTaskBuf;
        }
        len--;
    }
}

uint16_t get_rf_data_fifo_length(void) {
    if (rfBufHead > rfBufTail) {
        return rfBufHead - rfBufTail;
    } else if (rfBufHead < rfBufTail) {
        return sizeof (rfTaskBuf) - (rfBufTail - rfBufHead);
    } else {
        return 0;
    }
}

void read_rf_data_fifo(uint8_t* data, uint16_t len) {
    while (len) {
        *data = *rfBufTail;
        rfBufTail++;
        data++;
        if (rfBufTail >= rfTaskBuf + sizeof (rfTaskBuf)) {
            rfBufTail = rfTaskBuf;
        }
        len--;
    }
}

void rf_device_hop_channel(void) {
    SetRFChannel(RF_CHANNEL_TABLE[rfStatus.ch]);
    if (++rfStatus.ch == RF_HOPPING_CH) {
        rfStatus.ch = 0;
    }
}

void rf_device_set_sync_packet(void) {
    sTxData[0] = WR_TX_PLOAD;
    syncPkt.type = SYNC_PKT;
    syncPkt.sTick = RF_TICK();
    SPI_Write_Buffer(sTxData, 1, (uint8_t*) & syncPkt, sizeof (syncPkt));
}

void rf_device_set_ack_payload(void) {
    sTxData[0] = W_ACK_PAYLOAD_CMD;
    infoPkt.type = ACK_PKT;
    SPI_Write_Buffer(sTxData, 1, (uint8_t*) & infoPkt, rfStatus.pktAckByte);
    //if (infoPkt.pkt[0].len != 0) {
    //    _nop();
    //}
}

int read_rf_fifo(void) {
    uint16_t len;

    len = SPI_Read_Reg(R_RX_PL_WID_CMD);
    if (len != 0) {
        if (rfStatus.rate > FSK_2G_1M) {
            len = len * 8;
        }
        sTxData[0] = RD_RX_PLOAD;
        SPI_Read_Buffer(sTxData, 1, sRxData, len);
        return true;
    }
    return false;
}

/*=================================================
 * Function: RF_task application with linked type
 *
 * Return: void
 * */
void rf_application(void) {
    _info_pkt_t *pktPoint; //rf_fifo_buf;
    _sync_pkt_t *syncPoint; //rf_fifo_buf;
    struct _pkt_info_s *infoPoint = (struct _pkt_info_s *) sRxData; //rf_fifo_buf;
    uint8_t *pTemp = rfTaskBuf;
    uint16_t len = 0;
    volatile int i, cnt;
    uint8_t staReg, data;
    volatile uint64_t tick = 0;

    if (rfStatus.device == RF_MASTER) {
        pktPoint = (_info_pkt_t *) sRxData; //rf_fifo_buf;
        syncPoint = (_sync_pkt_t *) sTxData; //rf_fifo_buf;

        while (tick != RF_TICK()) {
            tick = RF_TICK();
        }
        if (IRQ_GET() == 0) { // Get IRQ
            if (IRQ_TICK() != 0) {
                tick = IRQ_TICK();
                IRQ_TICK_CLR();
            }
            staReg = 0x60; // check status reg, sometimes changed late.
            while (1) {
                data = SPI_Read_Reg(STATUS);
                if (staReg == data)
                    break;
                else
                    staReg = data;
            }

            if (staReg & MASK_RX_DR_FLAG) {
                SPI_Write_Reg(WRITE_REG | STATUS, MASK_RX_DR_FLAG);
                // read ack payload
                if (read_rf_fifo() == true && pktPoint->type == ACK_PKT) {
                    rfLinkCnt = true;
                    getAckIrq = true;
                    rfStatus.tick = tick + rfStatus.pktTickMax; // tick next packet
                    for (i = 0; i < PKT_SNED_PKT_INC + rfStatus.pktInfoMax; i++) {
                        if (infoPkt.pkt[i].len != 0) {
                            for (cnt = 0; cnt < rfStatus.pktInfoMax; cnt++) {
                                if (pktPoint->pkt[cnt].addr == infoPkt.pkt[i].addr) {
                                    if (pktPoint->pkt[cnt].len == infoPkt.pkt[i].len) {
                                        getPkt[gCnt] = infoPkt.pkt[i];
                                        if (gCnt < (PKT_GET_INFOxM * rfStatus.pktInfoMax) - 1) {
                                            gCnt++;
                                        } else {
                                            //RF_IO_TRIGGER_A;
                                            gCnt = (PKT_GET_INFOxM * rfStatus.pktInfoMax) - 1;
                                            //RF_IO_TRIGGER_A;
                                        }
                                        if (infoPkt.pkt[i].addr == losePkt.addr) {
                                            losePkt.len = 0;
                                            losePktCnt = 0;
                                        }
                                        infoPkt.pkt[i].len = 0;
                                        infoPkt.pkt[i].addr = 0;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                if ((staReg & MASK_MAX_RT_FLAG) != 0 || getAckIrq == false) {
                    if (txInfoCnt) {
                        txInfoCnt--;
                    } else {
                        txInfoCnt = rfStatus.pktInfoMax + PKT_SNED_PKT_INC - 1;
                    }
                    goto _RF_APP_MASTER_TX_SYNC;
                }
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            }
        } else if (tick > rfStatus.tick) {
            if (++rfLinkCnt > RF_UNLINK_MAX) {
                rfStatus.mode = UNLINK;
                sprintf(uart_msg_buf, "disconnect \r\n");
                appData.stringPointer = uart_msg_buf;
            } else {
                CE_LOW();
                rf_device_hop_channel();
                rfStatus.tick = tick + rfStatus.pktTickMax;
_RF_APP_MASTER_TX_SYNC: //
                SwitchToTxMode();
                syncPkt.type = SYNC_PKT;
                syncPkt.sTick = tick & 0xffffffff;
                sTxData[0] = WR_TX_PLOAD;
                SPI_Write_Buffer(sTxData, 1, (uint8_t*) & syncPkt, sizeof (syncPkt));
                infoCnt = 0; // set new TX packet
                infoSync = true;
                getAckIrq = false;
            }
        } else if (infoCnt < rfStatus.pktInfoMax) {
            if (get_rf_data_fifo_length() == 0) {
                infoSync = false;
            }
            data = SPI_Read_Reg(FIFO_STATUS);
            if ((data & FIFO_STATUS_TX_FULL) || (getAckIrq == false && infoCnt != 0)) {
                goto _RF_APP_MASTER_TX_END;
            }
            len = infoPkt.pkt[txInfoCnt].len;
            if (len == 0 && infoSync == true) {
                if (losePktCnt != 0) {
                    infoPkt.pkt[txInfoCnt] = losePkt;
                    if (--losePktCnt == 0) {
                        losePkt.len = 0;
                    }
                    len = infoPkt.pkt[txInfoCnt].len;
                    pTemp = rfTaskBuf + infoPkt.pkt[txInfoCnt].addr;
                } else if (rfBufHead != rfTaskPoint) {
                    if (rfBufHead > rfTaskPoint) {
                        len = rfBufHead - rfTaskPoint;
                    } else {
                        len = (int) (&rfTaskBuf[sizeof (rfTaskBuf)] - rfTaskPoint);
                    }
                    if (rfStatus.rate == FSK_2G_6M) {
                        if (len > PKT_LEN_6MRF) {
                            len = PKT_LEN_6MRF;
                        }
                    } else {
                        if (len > PKT_LEN_MAX) { // 4M/6M FIFO MAX.LEN
                            len = PKT_LEN_MAX;
                        }
                    }
                    if (len != 0) {
                        pTemp = rfTaskPoint;
                        rfTaskPoint += len;
                        if (rfTaskPoint >= &rfTaskBuf[sizeof (rfTaskBuf)]) {
                            rfTaskPoint = rfTaskBuf;
                            len = &rfTaskBuf[sizeof (rfTaskBuf)] - pTemp;
                        }
                        infoPkt.pkt[txInfoCnt].addr = pTemp - rfTaskBuf;
                        infoPkt.pkt[txInfoCnt].len = len;
                    }
                }
            } else {
                for (i = 0; i < PKT_SNED_PKT_INC + rfStatus.pktInfoMax; i++) {
                    if (infoPkt.pkt[txInfoCnt].len) {
                        pTemp = rfTaskBuf + infoPkt.pkt[txInfoCnt].addr;
                        if (infoCnt != 0) {
                            RF_IO_TRIGGER_B;
                            if (losePkt.len == 0) {
                                losePktCnt = 2; // lose Packet re-send count
                                losePkt = infoPkt.pkt[txInfoCnt];
                            }
                            RF_IO_TRIGGER_B;
                        }
                        break;
                    } else {
                        if (++txInfoCnt >= PKT_SNED_PKT_INC + rfStatus.pktInfoMax)
                            txInfoCnt = 0;
                    }
                }
            }
            if (len) {
                if (infoCnt < rfStatus.pktInfoMax) {
                    sTxData[0] = W_TX_PAYLOAD_NOACK_CMD;
                    memcpy(&sTxData[1], &infoPkt.pkt[txInfoCnt], sizeof (struct _pkt_info_s));
                    if (rfStatus.rate == FSK_2G_6M) {
                        len = PKT_LEN_6MRF;
                    } else {
                        len = PKT_LEN_MAX;
                    }
                    SPI_Write_Buffer(sTxData, sizeof (struct _pkt_info_s) + 1, pTemp, len);
                    infoCnt++;
                    if (++txInfoCnt >= PKT_SNED_PKT_INC + rfStatus.pktInfoMax)
                        txInfoCnt = 0;
                }
            }
        }
_RF_APP_MASTER_TX_END: //
        if (gCnt) {
            for (i = 0; i < gCnt; i++) {
                if (getPkt[i].len) {
                    pTemp = rfTaskBuf + getPkt[i].addr;
                    if (pTemp == rfBufTail) {
                        rfBufTail += getPkt[i].len;
                        getPkt[i].len = 0;
                        if (rfBufTail >= rfTaskBuf + sizeof (rfTaskBuf)) {
                            rfBufTail = rfTaskBuf;
                        }
                    } else {
                        // clear old packet
                        if (pTemp < rfBufTail) {
                            if ((rfBufTail - pTemp) < (sizeof (rfTaskBuf) / 2)) {
                                //RF_IO_TRIGGER_A;
                                getPkt[i].len = 0;
                                //RF_IO_TRIGGER_A;
                            }
                        } else {
                            if ((pTemp - rfBufTail) > (sizeof (rfTaskBuf) / 2)) {
                                //RF_IO_TRIGGER_A;
                                getPkt[i].len = 0;
                                //RF_IO_TRIGGER_A;
                            }
                        }
                    }
                }
            }
            for (i = 0; i < gCnt; i++) {
                if (getPkt[i].len == 0) {
                    getPkt[i].addr = 0;
                    if (i < (PKT_GET_INFOxM * rfStatus.pktInfoMax) - 1) {
                        getPkt[i] = getPkt[i + 1];
                        getPkt[i + 1].len = 0;
                    }
                } else {
                    if (getPkt[i].addr == getPkt[i + 1].addr) {
                        getPkt[i + 1].len = 0;
                    }
                }
            }
            if (getPkt[gCnt - 1].len == 0)
                gCnt--;
        }
    } else { //RF_SLAVE /////////////////
        ////////////////////////////
        pktPoint = (_info_pkt_t *) sTxData; //rf_fifo_buf;
        syncPoint = (_sync_pkt_t *) sRxData; //rf_fifo_buf;

        while (tick != RF_TICK()) {
            tick = RF_TICK();
        }
        if ((IRQ_GET() == 0) && (tick < rfStatus.tick - RF_TICK_400US)) {
            if (IRQ_TICK() != 0) {
                tick = IRQ_TICK();
                IRQ_TICK_CLR();
            }
            staReg = 0x40; // check status reg, sometimes changed late.
            while (1) {
                data = SPI_Read_Reg(STATUS);
                if (staReg == data)
                    break;
                else
                    staReg = data;
            }
            if (staReg & MASK_RX_DR_FLAG) {
_READ_APP_SLAVE_FIFO: // read rx fifo
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
                if (read_rf_fifo() == true) {
                    if (syncPoint->type == SYNC_PKT) {
                        RF_IO_TRIGGER_B;
                        rfStatus.tick = tick + rfStatus.pktTickMax + RF_TICK_200US;
                        RF_IO_TRIGGER_B;
                        if (getAckIrq == false) {
                            rfLinkCnt = true;
                            getAckIrq = true;
                        } else {
                            if (++getAckIrq > 7) { // ack error max to hopping
                                goto _HOP_APP_SLAVE_START;
                            }
                            data = SPI_Read_Reg(FIFO_STATUS);
                            if (data & FIFO_STATUS_TX_EMPTY) {
                                BSP_LEDToggle(BSP_LED_7);
                                memcpy(&infoPkt, &infoPktBackup, sizeof (infoPkt));
                                rf_device_set_ack_payload();
                                BSP_LEDToggle(BSP_LED_7);
                            }
                        }
                    } else if (infoCnt < rfStatus.pktInfoMax) {
                        len = infoPoint->len;
                        if (len <= PKT_LEN_MAX && len != 0 && infoPoint->addr < sizeof (rfTaskBuf)) {
                            infoPkt.pkt[infoCnt].len = len;
                            infoPkt.pkt[infoCnt].addr = infoPoint->addr;
                            if (gCnt < (PKT_GET_INFOxM * rfStatus.pktInfoMax) - 1) {
                                getPkt[gCnt] = infoPkt.pkt[infoCnt];
                                gCnt++;
                            } else {
                                _nop();
                            }
                            infoCnt++;
                            memcpy(&rfTaskBuf[infoPoint->addr], &sRxData[sizeof (struct _pkt_info_s)], len);
                        }
                    }
                    _nop();
                }
            } else {
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            }
        } else if (tick > rfStatus.tick) {
_HOP_APP_SLAVE_START: // read rx fifo
            if (++rfLinkCnt > RF_UNLINK_MAX) {
                rfStatus.mode = UNLINK;
                sprintf(uart_msg_buf, "disconnect. \r\n");
                appData.stringPointer = uart_msg_buf;
            } else {
                getAckIrq = false;
                rfStatus.tick = tick + rfStatus.pktTickMax;
                SPI_Read_Reg(FIFO_STATUS);
                //
                CE_LOW();
                rf_device_hop_channel();
                //SwitchToRxMode();
                SPI_Write_RegBuf(FLUSH_TX, 0); //Flush TX FIFO
                SPI_Write_Reg(WRITE_REG | CONFIG, 0x0f); // PRX
                CE_HIGH();
                //
                rf_device_set_ack_payload();
                infoCnt = 0;
                memcpy(&infoPktBackup, &infoPkt, sizeof (infoPkt));
                memset(&infoPkt, 0, sizeof (infoPkt));
            }
        } else {
            data = SPI_Read_Reg(FIFO_STATUS);
            if ((data & FIFO_STATUS_RX_EMPTY) == 0 && (tick < rfStatus.tick - RF_TICK_300US)) {
                goto _READ_APP_SLAVE_FIFO;
            }
        }
        if (gCnt) {
            i = gCnt;
            while (i) {
                i--;
                if (getPkt[i].len) {
                    pTemp = rfTaskBuf + getPkt[i].addr;
                    if (pTemp == rfBufHead) {
                        RF_IO_TRIGGER_B;
                        rfBufHead += getPkt[i].len;
                        getPkt[i].len = 0;
                        if (rfBufHead >= &(rfTaskBuf[sizeof (rfTaskBuf)])) {
                            rfBufHead = rfTaskBuf;
                        }
                        RF_IO_TRIGGER_B;
                    } else {
                        // clear old packet
                        if (pTemp < rfBufHead) {
                            if ((rfBufHead - pTemp) < (sizeof (rfTaskBuf) / 2)) {
                                //RF_IO_TRIGGER_A;
                                getPkt[i].len = 0;
                                //RF_IO_TRIGGER_A;
                            }
                        } else {
                            if ((pTemp - rfBufHead) > (sizeof (rfTaskBuf) / 2)) {
                                //RF_IO_TRIGGER_A;
                                getPkt[i].len = 0;
                                //RF_IO_TRIGGER_A;
                            }
                        }
                    }
                }
            }
            for (i = 0; i < gCnt; i++) {
                if (i >= (PKT_GET_INFOxM * rfStatus.pktInfoMax) - 1) {
                    break;
                }
                if (getPkt[i].len == 0) {
                    getPkt[i].len = getPkt[i + 1].len;
                    getPkt[i].addr = getPkt[i + 1].addr;
                    getPkt[i + 1].len = 0;
                }
            }
            if (getPkt[gCnt - 1].len == 0) {

                gCnt--;
            }
        }
    }
}

/*====================================================
 * Function: RF_task serch device to bind masker/slave
 *
 * Return: void
 * */
void rf_device_serch(void) {
    _info_pkt_t *pktPoint; //rf_fifo_buf;
    _sync_pkt_t *syncPoint; //rf_fifo_buf;
    uint64_t tick;
    uint8_t staReg, data;

    if (rfStatus.device == RF_MASTER) {
        pktPoint = (_info_pkt_t *) sRxData; //rf_fifo_buf;
        syncPoint = (_sync_pkt_t *) sTxData; //rf_fifo_buf;
        if (IRQ_GET() == 0) { // Get IRQ
            if (IRQ_TICK() == 0) {
                tick = RF_TICK();
            } else {
                tick = IRQ_TICK();
                IRQ_TICK_CLR();
            }
            staReg = 0x60;
            while (1) {
                data = SPI_Read_Reg(STATUS);
                if (staReg == data)
                    break;
                else
                    staReg = data;
            }
            if (staReg & MASK_RX_DR_FLAG) {
                data = read_rf_fifo();
                if (data == true && pktPoint->type == ACK_PKT) {
                    //RF_IO_TRIGGER_B;
                    rfLinkCnt = RF_UNLINK_MAX - 1; // set first link time as short
                    rfStatus.mode = LINKED;
                    rfStatus.tick = tick + rfStatus.pktTickMax; // tick next packet
                    memset(&infoPkt, 0, sizeof (infoPkt));
                    sprintf(uart_msg_buf, "Linked. \r\n");
                    appData.stringPointer = uart_msg_buf;
                    //RF_IO_TRIGGER_B;
                }
            }
            SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
        } else if (RF_TICK() > rfStatus.tick) {
            CE_LOW();
            rf_device_hop_channel();
            SwitchToTxMode();
            rfStatus.tick = RF_TICK() + RF_TICK_1MS;
            rf_device_set_sync_packet();
        }
    } else { // RF_SLAVE //////////////
        //////////////////////////
        pktPoint = (_info_pkt_t *) sTxData; //rf_fifo_buf;
        syncPoint = (_sync_pkt_t *) sRxData; //rf_fifo_buf;
        if (IRQ_GET() == 0) { // Get IRQ
            if (IRQ_TICK() == 0) {
                tick = RF_TICK();
            } else {
                tick = IRQ_TICK();
                IRQ_TICK_CLR();
            }
            staReg = 0x40;
            while (1) {
                data = SPI_Read_Reg(STATUS);
                if (staReg == data)
                    break;
                else
                    staReg = data;
            }
            if (staReg & MASK_RX_DR_FLAG) {
                if (read_rf_fifo() == true && syncPoint->type == SYNC_PKT) {
                    //RF_IO_TRIGGER_B;
                    rfLinkCnt = RF_UNLINK_MAX - 1; // set first link time as short
                    rfStatus.mode = LINKED;
                    rfStatus.tick = tick + rfStatus.pktTickMax; // tick next packet
                    infoCnt = 0;
                    SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
                    sprintf(uart_msg_buf, "Linked. \r\n");
                    appData.stringPointer = uart_msg_buf;
                    //RF_IO_TRIGGER_B;
                    goto _SET_SERCH_SLAVE_ACK;
                }
            }
            SPI_Write_RegBuf(FLUSH_RX, 0); //Flush RX FIFO
            SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
        } else if (RF_TICK() > rfStatus.tick) {
            CE_LOW();
            rf_device_hop_channel();
            SwitchToRxMode();
            rfStatus.tick = RF_TICK() + RF_TICK_7MS;
_SET_SERCH_SLAVE_ACK: //
            memset(&infoPkt, 0, sizeof (infoPkt));
            rf_device_set_ack_payload();
        }
    }
}


