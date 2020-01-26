/*******Filename: hik_tset.c *******************************/
/*******Brief:  hikvision test interface *******************/
/*******Author: fenglilei@hikvison.com.cn *****************/
/*******Data: 2017-4-11 10:37:54 ***************************/

#include "../app.h"
#include "../uart_cmd.h"
#include "../rf_task.h"
#include "../rf_config.h"
#include "../rf_app.h"
#include "hik_test.h"
#include "sch_task.h"




static void handle(void *input_arg, void *output_arg){
    if(input_arg){
        if(*(char *)input_arg >= ' ' && *(char *)input_arg <= '~'){
            printf("%s\r\n", (char *)input_arg);        
        }else{
            printf("no data!\r\n");
        }
    }
}


#define MAX_P8_BIT  (64+73) 
#define MAX_P32_BIT  (256+73) // P(8)+A(40)+C(9)+L(8*32)+R(16)
#define MAX_P40_BIT  (320+73)
#define MAX_P48_BIT  (384+73) 
#define MAX_P56_BIT  (448+73) 
#define MAX_P64_BIT  (512+73) 
#define MAX_P240_BIT  (1920+73)
#define MAX_P256_BIT  (2048+73) // P(8)+A(40)+C(9)+L(8*256)+R(16)
#define MAX_TX_LOCK_TIME  (0.004) //Sec
//#define CONST_RF_PAYLOAD_SET


#if 1
void hik_tx_packet_limit(uint8_t ch, uint8_t limit, uint8_t pkt_length) {
    uint8_t count, data;
    uint8_t max_cont = 0;
    uint16_t uR16;
    uint16_t width = 0;
    uint16_t len;
    uint8_t *p;
    
    uint32_t rate_bit;
    uint32_t packet_total;
    uint32_t us_tx_packet;
    uint64_t begin_tick = 0;
    uint64_t end_tick = 0;
    double us_tx_period = 0.0;
    
#if HIK_DEBUG_TX
    char hik_msg[128];
//    printf("hik_msg ptr=%p\n", hik_msg);
    hik_test_t hik_test={};
    uint64_t bytes_count = 0;
    uint64_t bytes_count_pre = 0;
    double detal_time = 0;
    init_test(&hik_test);
    hik_test.record.first_start_tick = RF_TICK();
    reg_event("tx_display", 1, 0, handle, hik_msg, NULL);
#endif
    
    
#ifdef CONST_RF_PAYLOAD_SET // test const payload
    SPI_Write_Reg((WRITE_REG | 1), 0x0);
    SPI_Write_Reg((WRITE_REG | 2), 0x1);
    SPI_Write_Reg((WRITE_REG | 0x11), 0x20);
    SPI_Write_Reg((WRITE_REG | 0x12), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x13), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x14), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x15), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x1C), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x1D), 0x0);
#endif    
/*
#if RF_PAYLOAD_ACK
    SPI_Write_Reg((WRITE_REG | 1), 0x3F);
    SPI_Write_Reg((WRITE_REG | 0x1D), 0x2);
#endif
 */
//    SPI_Write_Reg((WRITE_REG | 1), 0x3F);
//    SPI_Write_Reg((WRITE_REG | 0x1D), 0x2);
    
    SetRFChannel(ch);
    SwitchToTxMode();
    // set test buffer
    for (uR16 = 0; uR16 < sizeof (rfTaskBuf); uR16++) {
        rfTaskBuf[uR16] = uR16 & 0xff;
        rfTaskBuf[uR16 + 1] = (uR16 >> 8) & 0xff;
        //rfTaskBuf[uR16] = 0x0F;
        //rfTaskBuf[uR16 + 1] = 0xF0;
        uR16++;
    }
    switch (rfStatus.rate) {
        case FSK_2G_4M:
            if(pkt_length == 0){
                width = 256;
            }else{
                width = pkt_length;
            }            
            max_cont = (int) (4000000 * MAX_TX_LOCK_TIME / MAX_P256_BIT);
            break;
        case FSK_2G_6M:
            //width = 240; // ((address+payload)*8 + 2 )/3 = Integer
            if(pkt_length == 0 && pkt_length>240){
                width = 240;
            }else{
                width = pkt_length;
            }            
            max_cont = (int) (6000000 * MAX_TX_LOCK_TIME / MAX_P240_BIT);
            break;
    }

    rate_bit = limit*8*1024;
    packet_total = rate_bit/(width*8);
    us_tx_packet = 1000*1000/packet_total;
    printf("rate_bit = %u, packet_total=%u, us_tx_packet=%u\r\n", rate_bit, packet_total, us_tx_packet);
    p = rfTaskBuf;
    len = sizeof (rfTaskBuf);
    count = 0;
    while (len) {
        if (len <= width){
            uR16 = len;
#if HIK_DEBUG_TX
            p = rfTaskBuf;
            len = sizeof (rfTaskBuf);
#endif
        }else{
            uR16 = width;
        }
/*
#ifdef CONST_RF_PAYLOAD_SET 
        sTxData[0] = WR_TX_PLOAD;
#else
        sTxData[0] = W_TX_PAYLOAD_NOACK_CMD;
#endif
*/
        
/*       
#if RF_PAYLOAD_ACK
        sTxData[0] = W_ACK_PAYLOAD_CMD;
#endif
 */

        
        
        //sTxData[0] = W_TX_PAYLOAD_NOACK_CMD;
        //sTxData[0] = W_ACK_PAYLOAD_CMD;
        sTxData[0] = W_TX_PAYLOAD_NOACK_CMD;
        SPI_Write_Buffer(sTxData, 1, p, uR16);
//        printf("write payload , status %02x !!! retr %02x \r\n",SPI_Read_Reg(STATUS), SPI_Read_Reg(SETUP_RETR) );
        
#if HIK_DEBUG_TX
        bytes_count_pre = bytes_count;
        bytes_count += uR16;


#endif        
        
        count++;
        
        begin_tick = RF_TICK();
        
        while (1) { // check TX FIFO state
#if HIK_DEBUG_TX
            if(SPI_Read_Reg(STATUS) & STATUS_TX_DS){

                if(hik_test.record.tick_start == 0){
                    hik_test.record.tick_start = RF_TICK();
                }
                hik_test.record.tick_end = RF_TICK();
                detal_time = (hik_test.record.tick_end-hik_test.record.tick_start)*3.2;
                if(detal_time > 1000000){                   
                    memset(hik_msg,0,sizeof(hik_msg));
                    if(bytes_count == 0){
                        printf("error bytes_count=0 uR16=%d\n", uR16);
                        printf("pre = 0x%4x%4x%4x%4x\n", (uint16_t)(bytes_count_pre>>48)&0xffff, (uint16_t)(bytes_count_pre>>32)&0xffff, (uint16_t)(bytes_count_pre>>16)&0xffff, (uint16_t)(bytes_count_pre)&0xffff );
                    }                   
                    //memcpy(hik_msg, &hik_test, sizeof(hik_test_t));
                    sprintf(hik_msg, "Tx: %f Mbit/s\r\n", (double)(bytes_count<<3)/1000000 );
                    sprintf(hik_msg, "start_time:%f, cur_time:%f   Transmit Speed: %f Mbit/s\r\n", (hik_test.record.first_start_tick)*3.2/1000, hik_test.record.tick_end*3.2/1000, (double)(bytes_count<<3)/detal_time );
                    init_test(&hik_test);
                    bytes_count = 0;
    //                delay_us(100000);            //delay 100ms to ensure the rx have received all packet
                }                                      
            }
#endif
            
            if (SPI_Read_Reg(STATUS)&(STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT)) {
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            }
            //
            data = SPI_Read_Reg(FIFO_STATUS);
            if (data & FIFO_STATUS_TX_EMPTY) {
                count = 0;
            }
            
            end_tick = RF_TICK();  
            us_tx_period = (end_tick - begin_tick)*TICK_UNIT; 
            if (max_cont > count && !(data & FIFO_STATUS_TX_FULL) && us_tx_period > us_tx_packet ) {
                break;
            }
            if (appData.InterruptFlag) break;
//            printf("status %02x retr %02x \r\n",SPI_Read_Reg(STATUS), SPI_Read_Reg(SETUP_RETR));
            delay_us(10);
        }
#if HIK_DEBUG_TX
        if (appData.InterruptFlag) break;
#endif		
        len -= uR16;
        p += uR16;
    }
    while (1) { // check TX FIFO state
        data = SPI_Read_Reg(FIFO_STATUS);
        if (data & FIFO_STATUS_TX_EMPTY) {
            break;
        }
        if (SPI_Read_Reg(STATUS)&(STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT)) {
            SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
        }
        delay_us(10);
    }
#if HIK_DEBUG_TX	
    del_event("tx_display");
#endif	
    sprintf(uart_msg_buf, " Transmitted %d Bytes\r\n", (int) sizeof (rfTaskBuf));
    appData.stringPointer = uart_msg_buf;
    delay_us(1000);
    while (WriteString() == false);
    entryStandbyMode();
}

#endif

typedef struct _sync_pkt_s {
    uint32_t type;
    //uint16_t reserve;
    uint32_t sTick;
} _sync_pkt_t;

#define rf_tick_time (0.0000032) // uSec

typedef enum {
    SYNC_PKT = 0xE100, //
    RX_PKT = 0xE200, //
    ACK_PKT = 0xE300, //
} _pkt_type;

void hik_rf_device_set_sync_packet(void) {
    _sync_pkt_t syncPkt;
    sTxData[0] = WR_TX_PLOAD;
    syncPkt.type = SYNC_PKT;
    syncPkt.sTick = RF_TICK();
    printf("send packet info, type : %08x sTick : %08x len=%d\r\n",syncPkt.type,syncPkt.sTick, (int)sizeof(syncPkt));
    SPI_Write_Buffer(sTxData, 1, (uint8_t*) & syncPkt, sizeof (syncPkt));
}

/*====================================================
 * Function: RF_task serch device to bind masker/slave
 *
 * Return: void
 * */
void hik_test_ack(uint8_t mode) {
//    uint64_t tick;
    uint8_t staReg, data;
    uint64_t hik_tick = 0;
#define RF_MASTER 0
    
    if (mode == RF_MASTER) {
        SwitchToTxMode();  
        while(1) {
            if (IRQ_GET() == 0) { // Get IRQ
                if (IRQ_TICK() == 0) {
                    
                } else {
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
                //printf("----tx status=%02x\r\n", staReg);
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            } else if (RF_TICK() > hik_tick) {
                #define RF_TICK_1000MS (int)(1/rf_tick_time)
                hik_tick = RF_TICK() + RF_TICK_1000MS;
                hik_rf_device_set_sync_packet();
            }
        }
    } else { // RF_SLAVE //////////////
        //////////////////////////
        _sync_pkt_t *syncPoint = NULL;
        syncPoint = (_sync_pkt_t *) sRxData; //rf_fifo_buf;
        SwitchToRxMode();
        while(1) {
            if (IRQ_GET() == 0) { // Get IRQ
                if (IRQ_TICK() == 0) {
//                    tick = RF_TICK();
                } else {
//                    tick = IRQ_TICK();
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
                printf("rx status=%02x\r\n", staReg);
                if (staReg & MASK_RX_DR_FLAG) {
                    uint16_t len;

                    len = SPI_Read_Reg(R_RX_PL_WID_CMD);
                    if (len != 0) {
                        sTxData[0] = RD_RX_PLOAD;
                        SPI_Read_Buffer(sTxData, 1, sRxData, 8);
                        printf("receive packet info0----- ** , type : %08x sTick : %08x  len %d \r\n",syncPoint->type,syncPoint->sTick,len);
                    }
                }
                SPI_Write_RegBuf(FLUSH_RX, 0); //Flush RX FIFO
                SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
            } else if (RF_TICK() > hik_tick) {
                #define RF_TICK_7MS (int)(0.007/rf_tick_time)

                hik_tick = RF_TICK() + RF_TICK_7MS;
            }
        }
    }
}


void hik_tx_packet_limit_ack(uint8_t ch, uint8_t limit, uint8_t pkt_length) {
//    uint64_t tick;
    uint8_t staReg, data;

    


    uint16_t uR16;
    uint16_t width = 0;
    uint16_t len;
    uint8_t *p;
    
    uint32_t rate_bit;
    uint32_t packet_total;
    uint32_t us_tx_packet;
    uint64_t begin_tick = 0;
    uint64_t end_tick = 0;
    double us_tx_period = 0.0;
    
#if HIK_DEBUG_TX
    char hik_msg[128];
//    printf("hik_msg ptr=%p\n", hik_msg);
    hik_test_t hik_test={};
    uint64_t bytes_count = 0;
    double detal_time = 0;
    init_test(&hik_test);
    hik_test.record.first_start_tick = RF_TICK();
    reg_event("tx_display", 1, 0, handle, hik_msg, NULL);
#endif
    
    SetRFChannel(ch);
    SwitchToTxMode();
    // set test buffer
    for (uR16 = 0; uR16 < sizeof (rfTaskBuf); uR16++) {
        rfTaskBuf[uR16] = uR16 & 0xff;
        rfTaskBuf[uR16 + 1] = (uR16 >> 8) & 0xff;
        uR16++;
    }
    switch (rfStatus.rate) {
        case FSK_2G_4M:
            if(pkt_length == 0){
                width = 256;
            }else{
                width = pkt_length;
            }            
            break;
        case FSK_2G_6M:
            //width = 240; // ((address+payload)*8 + 2 )/3 = Integer
            if(pkt_length == 0 && pkt_length>240){
                width = 240;
            }else{
                width = pkt_length;
            }            
            break;
    }

    rate_bit = limit*8*1024;
    packet_total = rate_bit/(width*8);
    us_tx_packet = 1000*1000/packet_total;
    printf("rate_bit = %u, packet_total=%u, us_tx_packet=%u\r\n", rate_bit, packet_total, us_tx_packet);
    p = rfTaskBuf;
    len = sizeof (rfTaskBuf);
    while (len) {
        if (len <= width){
            uR16 = len;
#if HIK_DEBUG_TX
            p = rfTaskBuf;
            len = sizeof (rfTaskBuf);
#endif
        }else{
            uR16 = width;
        }

        sTxData[0] = WR_TX_PLOAD;
        SPI_Write_Buffer(sTxData, 1, p, uR16);
        

        bytes_count += uR16;
                
        begin_tick = RF_TICK();        
        while (1) { // check TX FIFO state
            if (IRQ_GET() == 0) { // Get IRQ
                if (IRQ_TICK() == 0) {
                    
                } else {
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
                //printf("----tx status=%02x\r\n", staReg);               
            }
         

            if(SPI_Read_Reg(STATUS) & STATUS_TX_DS){
                if(hik_test.record.tick_start == 0){
                    hik_test.record.tick_start = RF_TICK();
                }
                hik_test.record.tick_end = RF_TICK();
                detal_time = (hik_test.record.tick_end-hik_test.record.tick_start)*3.2;
                if(detal_time > 1000000){                   
                    memset(hik_msg,0,sizeof(hik_msg));                               
                    sprintf(hik_msg, "Tx: %f Mbit/s\r\n", (double)(bytes_count<<3)/1000000 );                  
                    init_test(&hik_test);
                    bytes_count = 0;
                }                                      
            }

            
            if (SPI_Read_Reg(STATUS)&(STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT)) {
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            }
            //
            data = SPI_Read_Reg(FIFO_STATUS);
           
            end_tick = RF_TICK();  
            us_tx_period = (end_tick - begin_tick)*TICK_UNIT; 
            if (!(data & FIFO_STATUS_TX_FULL) && us_tx_period > us_tx_packet ) {
                break;
            }
            if (appData.InterruptFlag) break;
            delay_us(10);
        }
#if HIK_DEBUG_TX
        if (appData.InterruptFlag) break;
#endif		
        len -= uR16;
        p += uR16;
    }
    while (1) { // check TX FIFO state
        data = SPI_Read_Reg(FIFO_STATUS);
        if (data & FIFO_STATUS_TX_EMPTY) {
            break;
        }
        if (SPI_Read_Reg(STATUS)&(STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT)) {
            SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
        }
        delay_us(10);
    }
#if HIK_DEBUG_TX	
    del_event("tx_display");
#endif	
}



static const uint32_t Bank1_Reg0_13_2G_4M[] = {
    0x0158F81F,
    0xD0582225,
    0x004C2D90,
    0xDC2438AB,
    0x03807CFA,
    0x02FC5F67,
    0x44857FF1,
    0x10E69FFF,
    0x00000000,
    0x00000096,
    0x03058000,
    0xFAE02E61,
    0x001B7305,
    0x36B48000
};

 uint32_t Hik_Bank1_Reg0_13[14]; // analog register table
static uint8_t RegArrFSK[][2] = {
    { 0, 0x0f}, // 0
    { 1, 0x3f}, // 1
    { 2, 0x3f}, // 2
    { 3, 0x03}, // 3
    { 4, 0x00}, // 4
    { 5, 0x17}, // 5
    { 6, 0x07}, // 6
    { 7, 0x07}, // 7
    //{ 8, 0x00}, // 8
    //{ 9, 0x00}, // 9
    { 12, 0xc3}, //10
    { 13, 0xc4}, //11
    { 14, 0xc5}, //12
    { 15, 0xc6}, //13
    //{ 17, 0x0}, //14
    //{ 18, 0x0}, //15
    //{ 19, 0x0}, //16
    //{ 20, 0x0}, //17
    //{ 21, 0x0}, //18
    //{ 22, 0x0}, //19
    //{ 23, 0x00}, //20
    { 28, 0x3f}, //21
    { 29, 0x3f} //22
};

static unsigned char Bank1_Reg14_Tab[] = {
    0x41,
    0x20,
    0x08,
    0x04,
    0x81,
    0x20,
    0xCF,
    0xF7,
    0xFE,
    0xFF,
    0xFF
};

static const uint8_t RX0_Address[] = {
    0x15,
    0x59,
    0x23,
    0xC6,
    0x46, //0x36
};
static const uint8_t RX1_Address[] = {
    0x10,
    0x56,
    0x24,
    0xCD,
    0x58, //0x78
};



void Hik_initFSKReg(char *uart_cmd_buf) {
    int fskMode = FSK_2G_4M;
    int i;
    uint32_t* p32;
    uint8_t* buf = &sTxData[1];
    uint8_t data;
    
    uint8_t pipe_number;
    char* addr;
    

    // spi config
    CE_LOW();
    CSN_HIGH();
    memset(sTxData, 0, sizeof (sTxData));
    memset(sRxData, 0, sizeof (sRxData));
    // select table
    switch (fskMode) {
        case FSK_2G_4M:
            p32 = (uint32_t*) Bank1_Reg0_13_2G_4M;
            break;
        default:
            break;
    }
    memcpy(Hik_Bank1_Reg0_13, p32, sizeof (Hik_Bank1_Reg0_13));
    rfStatus.rate = fskMode;

    //********************config bank=0******************
    if (rfStatus.rate >= FSK_5G_250K) {
        buf[0] = 0x00;
        buf[1] = 0x1B;
        buf[2] = 0x73;
        buf[3] = 0x05;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
        SPI_Write_Reg(ACTIVATE_CMD, 0x53);
        delay_us(80);
        buf[0] = 0x00;
        buf[1] = 0x1B;
        buf[2] = 0x73;
        buf[3] = 0x05;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
    } else {
        buf[0] = 0x00;
        buf[1] = 0x12;
        buf[2] = 0x73;
        buf[3] = 0x05;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
        SPI_Write_Reg(ACTIVATE_CMD, 0x53);
        delay_us(80);
        buf[0] = 0x00;
        buf[1] = 0x12;
        buf[2] = 0x73;
        buf[3] = 0x05;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
    }
    SwitchCFG(0);
    delay_us(80);

    //********************Write Bank0 register******************
    for (i = (sizeof (RegArrFSK) / 2 - 3); i >= 0; i--) {
        SPI_Write_Reg((WRITE_REG | RegArrFSK[i][0]), RegArrFSK[i][1]);
    }
    
#if 1
    if(uart_cmd_buf[3] == 1) {
        addr = &uart_cmd_buf[4];
        memcpy(buf, addr, 5);
        SPI_Write_RegBuf((WRITE_REG | 16), 5);
        
        memcpy(buf, addr, 5);  
        SPI_Write_RegBuf((WRITE_REG | 10), 5);
    } else if (uart_cmd_buf[3] == 2) {
        pipe_number = uart_cmd_buf[4];
        addr = &uart_cmd_buf[5];
        if(pipe_number<2){
            memcpy(buf, addr, 5);
            SPI_Write_RegBuf(WRITE_REG | (pipe_number+10), 5);
        } else if(pipe_number>=2 && pipe_number < 6) {
            memcpy(buf, addr, 1);
            SPI_Write_RegBuf(WRITE_REG | (pipe_number+10), 1);
        } else {
            printf("error pipe number!\r\n");
        }
    } else {
        printf("error set addr!\r\n");
    }
#else
    //reg 10 - Rx0 addr
    memcpy(buf, RX0_Address, 5);
    SPI_Write_RegBuf((WRITE_REG | 10), 5);
    //reg 11 - Rx1 addr
    memcpy(buf, RX1_Address, 5);
    SPI_Write_RegBuf((WRITE_REG | 11), 5);
//#if HIK_DEBUG
    //reg 16 - TX addr
//    memcpy(buf, RX1_Address, 5);    
//#else
    //reg 16 - TX addr
    memcpy(buf, RX0_Address, 5);
//#endif    
    
    SPI_Write_RegBuf((WRITE_REG | 16), 5);
#endif    
    

    data = SPI_Read_Reg(0x1D);
    if (data == 0) {
        SPI_Write_Reg(ACTIVATE_CMD, 0x73); // Active
    }
    for (i = sizeof (RegArrFSK) / 2 - 1; i >= sizeof (RegArrFSK) / 2 - 2; i--) {
        SPI_Write_Reg((WRITE_REG | RegArrFSK[i][0]), (uint8_t) RegArrFSK[i][1]);
    }

    //********************Write Bank1 register******************
    SwitchCFG(1);
    for (i = 0; i <= 13; i++) {
        readAnalogTable(i, buf);
        SPI_Write_RegBuf((WRITE_REG | i), 4);
    }

    memcpy(buf, Bank1_Reg14_Tab, 11);
    SPI_Write_RegBuf((WRITE_REG | 14), 11);
    delay_us(500);
    SwitchCFG(0);
    //********************Calibrate******************
    KModCal();
    IFFilterCal();
    //**********************************
    SwitchCFG(0);
    //set in KmodCal()//SetAirRate(rfStatus.rate);
    entryStandbyMode(); //SwitchToRxMode(); //CE_LOW();
}





uint8_t init_test(hik_test_t *test){
    //test->record.first_start_tick = 0;
    test->record.tick_end = 0;
    test->record.tick_start = 0;
    test->record.bytes = 0;
    test->test_type = TEST_NON;
    return 0;
}

/****************TX test*****************/

uint8_t record_tick(uint64_t *tick_start){
    if(tick_start){
        *tick_start = RF_TICK();
        return 0;
    }else{
        return -1;
    }
}

double cal_speed(hik_test_t test, int bytes){
    if(test.record.tick_end-test.record.tick_start>2000){                         //if the tick no more than 20, the result maybe irreccurt.
        return (bytes*8)/((test.record.tick_end-test.record.tick_start)*TICK_UNIT);       //TICK_UNIT must be us
    }
    return -1;
}






/***************RX test*****************/
