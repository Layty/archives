
#include "app.h"
#include "framework/driver/spi/static/drv_spi_static.h"
#include "rf_config.h"
#include "rf_app.h"
#include "uart_cmd.h"

void delay_us(uint32_t usec) {
    uint64_t t;
    t = RF_TICK()+((usec * 10) / 32);
    while (t > RF_TICK()) {
    }
}

//
const uint32_t Bank1_Reg0_13_2G_250K[] = {
    0x0158F803,
    0xD0582225,
    0x004C2D90,
    0xDC241557,
    0x03807CFA,
    0x42FC5F67,
    0x44857FF1,
    0x10E69FFF,
    0x00000000,
    0x00000096,
    0x03054000,
    0xFAD92E21,
    0x001B7305,
    0x36B48004
};
const unsigned char Bank1_Reg14_2G_250K[] = {
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
//
const uint32_t Bank1_Reg0_13_2G_1M[] = {
    0x0158F81F,
    0xD0582225,
    0x004C2D90,
    0xDC241457,
    0x36807CFA,
    0x02F44F5F,
    0x44857FF1,
    0x10E69FFF,
    0x00000000,
    0x00000096,
    0x03058000,
    0xFAE02E21,
    0x001B7305,
    0x36B48004
};
const unsigned char Bank1_Reg14_2G_1M[] = {
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
//
const uint32_t Bank1_Reg0_13_2G_2M[] = {
    0x0158F81F,
    0xD0582225,
    0x004C2D90,
    0xDC241457,
    0x36807CFA,
    0x02F44F5F,
    0x44857FF1,
    0x10E69FFF,
    0x00000000,
    0x00000096,
    0x03058000,
    0xFAE02E21,
    0x001B7305,
    0x36B48005
};
const unsigned char Bank1_Reg14_2G_2M[] = {
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
//
const uint32_t Bank1_Reg0_13_2G_4M[] = {
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
const unsigned char Bank1_Reg14_2G_4M[] = {
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
//
const uint32_t Bank1_Reg0_13_2G_6M[] = {
    0x0158F81F,
    0xD0582225,
    0x004C2D90,
    0xDC2438EB,
    0x03807CFA,
    0x02FC5F67,
    0x44857FF1,
    0x10E69FFF,// <-R7.bit12=1; 0x10F69FFF,
    0x00000000,
    0x00000096,
    0x43058000,
    0xFAE02E61,
    0x001B7305,
    0x36B48000
};
const unsigned char Bank1_Reg14_2G_6M[] = {
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

uint8_t RegArrFSK[][2] = {
    { 0, 0x0f}, // 0
    { 1, 0x3f}, // 1
    { 2, 0x3f}, // 2
    { 3, 0x03}, // 3
    { 4, 0x00}, // 4
    { 5, 0x17}, // 5
    { 6, 0x07}, // 6
    { 7, 0x07}, // 7
    { 8, 0x00}, // 8
    { 9, 0x00}, // 9
    { 12, 0xc3}, //10
    { 13, 0xc4}, //11
    { 14, 0xc5}, //12
    { 15, 0xc6}, //13
    { 17, 0x0}, //14
    { 18, 0x0}, //15
    { 19, 0x0}, //16
    { 20, 0x0}, //17
    { 21, 0x0}, //18
    { 22, 0x0}, //19
    { 23, 0x00}, //20
    { 28, 0x3f}, //21
    { 29, 0x3f} //22
};

const uint8_t RX0_Address[] = {
    0x15,
    0x59,
    0x23,
    0xC6,
    0x46,//0x36
};
const uint8_t RX1_Address[] = {
    0x10,
    0x56,
    0x24,
    0xCD,
    0x58,//0x78
};

uint32_t* Bank1_Reg0_13; // analog register table
uint8_t* Bank1_Reg14; // analog register.14 value
_rf_cfg_t rfStatus; // rf confige & task status

void SwitchCFG(char _cfg);

///////////////////////////////////////////////////////////////////////////////
//                  SPI access                                               //
///////////////////////////////////////////////////////////////////////////////

void SPI_Write_Reg(uint8_t reg, uint8_t data) {
    CSN_LOW();
    sTxData[0] = reg;
    sTxData[1] = data;
    DRV_SPI0_BufferAddWriteD(sTxData, 2);
    CSN_HIGH();
    return;
}

uint8_t SPI_Read_Reg(uint8_t reg) {
    CSN_LOW();
    sTxData[0] = reg;
    sTxData[1] = 0;
    DRV_SPI0_BufferAddWriteD(sTxData, 1);
    DRV_SPI0_BufferAddReadD(sRxData, 1);
    CSN_HIGH();
    return sRxData[0];
}

void SPI_Write_RegBuf(uint8_t reg, uint8_t bytes) {
    CSN_LOW();
    sTxData[0] = reg;
    DRV_SPI0_BufferAddWriteD(sTxData, bytes + 1);
    CSN_HIGH();
    return;
}

void SPI_Read_RegBuf(uint8_t reg, uint8_t bytes) {
    CSN_LOW();
    sTxData[0] = reg;
    DRV_SPI0_BufferAddWriteD(sTxData, 1);
    DRV_SPI0_BufferAddReadD(sRxData, bytes);
    CSN_HIGH();
    return;
}

void SPI_Write_Buffer(uint8_t* cmd, uint8_t len, uint8_t *pBuf, uint16_t bytes) {
    CSN_LOW();
    DRV_SPI0_BufferAddWriteD(cmd, len);
    DRV_SPI0_BufferAddWriteD(pBuf, bytes);
    CSN_HIGH();
    return;
}

void SPI_Read_Buffer(uint8_t* cmd, uint8_t len, uint8_t *pBuf, uint16_t bytes) {
    CSN_LOW();
    DRV_SPI0_BufferAddWriteD(cmd, len);
    DRV_SPI0_BufferAddReadD(pBuf, bytes);
    CSN_HIGH();
    return;
}

void readAnalogTable(uint8_t reg, uint8_t* buf) {
    int i;

    if (reg <= 8) {
        for (i = 0; i < 4; i++) { // SPI MSB->LSB
            buf[i] = (Bank1_Reg0_13[reg] >> (8 * (i))) & 0xff;
        }
    } else {
        for (i = 0; i < 4; i++) { // SPI LSB->MSB
            buf[i] = (Bank1_Reg0_13[reg] >> (8 * (3 - i))) & 0xff;
        }
    }
}

/**************************************************
Function: SwitchToRxMode();
Description:
    switch to Rx mode
 **************************************************/
void SwitchToRxMode(void) {
    CE_LOW();
    SPI_Write_RegBuf(FLUSH_TX, 0); //Flush TX FIFO
    SPI_Write_RegBuf(FLUSH_RX, 0); //Flush TX FIFO
    SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
    // PRX
    SPI_Write_Reg(WRITE_REG | CONFIG, 0x0F);
    CE_HIGH();
}

/**************************************************
Function: SwitchToTxMode();
Description:
    switch to Tx mode
 **************************************************/
void SwitchToTxMode(void) {
    CE_LOW();
    SPI_Write_RegBuf(FLUSH_TX, 0); //Flush TX FIFO
    SPI_Write_RegBuf(FLUSH_RX, 0); //Flush TX FIFO
    SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
    // PTX
    SPI_Write_Reg(WRITE_REG | CONFIG, 0x0E);
    CE_HIGH();
}

/**************************************************
Function: rfPowerOn();
Description:
 Power On
 **************************************************/
void rfPowerOn(void) {
    uint8_t* buf = &sTxData[1];

    SwitchCFG(0);
    SPI_Write_Reg(WRITE_REG | CONFIG, 0x0E);
    SwitchCFG(1);
    readAnalogTable(7, buf);
    buf[2] &= 0xFD; //
    SPI_Write_RegBuf((WRITE_REG | 7), 4);
    delay_us(10); // wait 10u Sec
    buf[2] |= 0x02; //
    SPI_Write_RegBuf((WRITE_REG | 7), 4);
    readAnalogTable(10, buf);
    buf[0] &= 0xEF; //
    SPI_Write_RegBuf((WRITE_REG | 10), 4);
    delay_us(1); // wait 1u Sec
    readAnalogTable(7, buf);
    buf[2] |= 0x02; //
    SPI_Write_RegBuf((WRITE_REG | 7), 4);
    SwitchCFG(0);
}

///////////////////////////////////////////////////////////////////////////////
//                  BK5812 Control                                           //
///////////////////////////////////////////////////////////////////////////////

void SwitchCFG(char _cfg) //1:analog 0:digital
{
    uint8_t data;
    if (_cfg != rfStatus.bank)
        rfStatus.bank = _cfg;
    else {
        goto _SwitchCFG_chk;
    }
_SwitchCFG_set: //
    SPI_Write_Reg(ACTIVATE_CMD, 0x53);
_SwitchCFG_chk: //
    data = SPI_Read_Reg(STATUS);
    data &= 0x80;
    if ((data && (_cfg == 0)) || ((data == 0) && (_cfg))) {
        goto _SwitchCFG_set;
    }
}

void SetAirRate(uint8_t rate) {
    rate &= 0x07;
    rate = ((rate << 3) & 0xff) + 0x07;
    SPI_Write_Reg((WRITE_REG | RF_SETUP), rate);
}

void SetRFChannel(uint8_t ch) {
    SPI_Write_Reg((WRITE_REG | RF_CH), ch);
}

void IFFilterCal(void) {
    uint8_t* buf = &sTxData[1];

    CE_LOW();
    SwitchCFG(0);
    SPI_Write_Reg(WRITE_REG | CONFIG, 0x0F);
    CE_HIGH();
    SwitchCFG(1);
    readAnalogTable(3, buf);
    buf[2] |= 0x40;
    SPI_Write_RegBuf((WRITE_REG | 3), 4);
    delay_us(20);
    readAnalogTable(3, buf);
    SPI_Write_RegBuf((WRITE_REG | 3), 4);
    SwitchCFG(0);
}

void KModCal(void) {
    uint8_t* buf = &sTxData[1];
    uint32_t fd_read = 0;

    SetAirRate(rfStatus.rate);
    set_SingleWave(0);
    SwitchCFG(1);
    readAnalogTable(12, buf);
    buf[0] |= 0x92; //bit 7/4/1=1 Enter in Tx PN9 to resolve pll unlocking issue
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    delay_us(100);
    buf[0] &= 0xEF; //bit 4=0 Exit Tx PN9
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    readAnalogTable(1, buf);
    buf[3] |= 0x02; //bit 1=1 Disable VCO auto calibration
    SPI_Write_RegBuf((WRITE_REG | 1), 4);
    readAnalogTable(11, buf);
    buf[2] |= 0x01; // bit 16:8=100(H) initial fm_gain
    buf[1] = 0x0;
    SPI_Write_RegBuf((WRITE_REG | 11), 4);
    readAnalogTable(12, buf);
    buf[0] |= 0x82; // bit 7,1 =1
    buf[0] &= 0xFE; // bit 0=0
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    delay_us(20);
    buf[0] |= 0x01;
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    //
    delay_us(200000); // wait 100mSec
    //
    SPI_Read_RegBuf(8, 4);
    fd_read |= sRxData[0];
    fd_read += (sRxData[1]&0x1F)*256;
    switch (rfStatus.rate) {
        case FSK_2G_250K:
            fd_read = 160 * 256 / fd_read;
            break;
        case FSK_2G_1M:
            fd_read = 320 * 256 / fd_read;
            break;
        case FSK_2G_2M:
            fd_read = 640 * 256 / fd_read;
            break;
        case FSK_2G_4M:
            fd_read = 1900 * 256 / fd_read;
            break;
        case FSK_2G_6M:
            fd_read = 1650 * 256 / fd_read;
            break;
    }
    readAnalogTable(11, buf);
    buf[1] = fd_read & 0xff;
    if (fd_read & 0x100) {
        buf[2] |= 0x01;
    } else {
        buf[2] &= 0xFE;
    }
    SPI_Write_RegBuf((WRITE_REG | 11), 4);
    //exit 
    readAnalogTable(12, buf);
    buf[0] |= 0x82; //bit0=0 
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    readAnalogTable(1, buf);
    SPI_Write_RegBuf((WRITE_REG | 1), 4);
    readAnalogTable(12, buf);
    buf[0] |= 0x80; //bit1=0 
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    readAnalogTable(7, buf);
    SPI_Write_RegBuf((WRITE_REG | 7), 4);
    readAnalogTable(6, buf);
    SPI_Write_RegBuf((WRITE_REG | 6), 4);
    readAnalogTable(9, buf);
    SPI_Write_RegBuf((WRITE_REG | 9), 4);
    readAnalogTable(12, buf);
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    SwitchCFG(0);
}


///////////////////////////////////////////////////////////////////////////////
//                  BK5812 initialization                                    //
///////////////////////////////////////////////////////////////////////////////

void initFSKReg(uint8_t fskMode) {
    int i;
    uint8_t* buf = &sTxData[1];
    uint8_t data;

    // spi config
    CE_LOW();
    CSN_HIGH();
    memset(sTxData, 0, sizeof (sTxData));
    memset(sRxData, 0, sizeof (sRxData));
    // select table
    if (fskMode == FSK_2G_1M) {
        Bank1_Reg0_13 = (uint32_t*) Bank1_Reg0_13_2G_1M;
        Bank1_Reg14 = (unsigned char*) Bank1_Reg14_2G_1M;
    } else if (fskMode == FSK_2G_2M) {
        Bank1_Reg0_13 = (uint32_t*) Bank1_Reg0_13_2G_2M;
        Bank1_Reg14 = (unsigned char*) Bank1_Reg14_2G_2M;
    } else if (fskMode == FSK_2G_4M) {
        Bank1_Reg0_13 = (uint32_t*) Bank1_Reg0_13_2G_4M;
        Bank1_Reg14 = (unsigned char*) Bank1_Reg14_2G_4M;
    } else if (fskMode == FSK_2G_6M) {
        Bank1_Reg0_13 = (uint32_t*) Bank1_Reg0_13_2G_6M;
        Bank1_Reg14 = (unsigned char*) Bank1_Reg14_2G_6M;
    } else { //
        Bank1_Reg0_13 = (uint32_t*) Bank1_Reg0_13_2G_250K;
        Bank1_Reg14 = (unsigned char*) Bank1_Reg14_2G_250K;
        fskMode = FSK_2G_250K;
    }
    rfStatus.rate = fskMode;

    //********************config bank=0******************
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
    SwitchCFG(0);
    delay_us(80);

    //********************Write Bank0 register******************
    for (i = 20; i >= 0; i--) {
        SPI_Write_Reg((WRITE_REG | RegArrFSK[i][0]), RegArrFSK[i][1]);
    }
    //reg 10 - Rx0 addr
    memcpy(buf, RX0_Address, 5);
    SPI_Write_RegBuf((WRITE_REG | 10), 5);
    //reg 11 - Rx1 addr
    memcpy(buf, RX1_Address, 5);
    SPI_Write_RegBuf((WRITE_REG | 11), 5);
    //reg 16 - TX addr
    memcpy(buf, RX0_Address, 5);
    SPI_Write_RegBuf((WRITE_REG | 16), 5);

    data = SPI_Read_Reg(0x1D);
    if (data == 0) {
        SPI_Write_Reg(ACTIVATE_CMD, 0x73); // Active
    }
    for (i = 22; i >= 21; i--) {
        SPI_Write_Reg((WRITE_REG | RegArrFSK[i][0]), (uint8_t) RegArrFSK[i][1]);
    }

    //********************Write Bank1 register******************
    SwitchCFG(1);
    for (i = 0; i <= 13; i++) {
        readAnalogTable(i, buf);
        SPI_Write_RegBuf((WRITE_REG | i), 4);
    }

    memcpy(buf, Bank1_Reg14, 11);
    SPI_Write_RegBuf((WRITE_REG | 14), 11);
    delay_us(500);
    SwitchCFG(0);
    //********************Calibrate******************
    KModCal();
    IFFilterCal();

    //**********************************
    SwitchCFG(0);
    SetAirRate(rfStatus.rate);
    SwitchToRxMode();
}

void set_SingleWave(uint8_t ch) {
    uint8_t *buf = &sTxData[1];
    int i;

    SetRFChannel(ch);
_SingleWave_Str: //
    SwitchToTxMode();
    //SPI_Read_Buf(STATUS, buf, 1);
    delay_us(10);
    // send package 8 byte
    buf[0] = 0x0;
    buf[1] = 0x61;
    buf[2] = 0x62;
    buf[3] = 0x63;
    buf[4] = 0x64;
    buf[5] = 0x65;
    buf[6] = 0x66;
    buf[7] = 0x67;
    SPI_Write_RegBuf(W_TX_PAYLOAD_NOACK_CMD, 8);
    // wait tx int
    i = 0;
    while (1) {
        SPI_Read_Reg(FIFO_STATUS);
        delay_us(100);
        if (SPI_Read_Reg(STATUS) & (MASK_TX_DS_FLAG | MASK_MAX_RT_FLAG)) {
            SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
            break;
        }
        if (i > 10)
            goto _SingleWave_Str;
    }

    delay_us(500); //Delay(100);
    SwitchCFG(1);
    // Bank1.12<7>=1
    readAnalogTable(12, buf);
    buf[0] |= 0x80;
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    // Bank1.9<1>=1
    readAnalogTable(9, buf);
    if (rfStatus.rate < FSK_2G_2M) {
        buf[2] |= 0x10;
    } else if (rfStatus.rate > FSK_2G_2M) {
        buf[2] |= 0x40;
    } else {
        buf[2] |= 0x20;
    }
    SPI_Write_RegBuf((WRITE_REG | 9), 4);
    // Bank1.6<15>=0
    readAnalogTable(6, buf);
    buf[2] &= 0x7f;
    SPI_Write_RegBuf((WRITE_REG | 6), 4);
    // Bank1.7<16>=0
    readAnalogTable(7, buf);
    buf[1] &= 0xfe;
    SPI_Write_RegBuf((WRITE_REG | 7), 4);
    // Bank1.12<1>=1 & <7>=1
    readAnalogTable(12, buf);
    buf[0] |= 0x82;
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    SwitchCFG(0);
}

void setRFPower(uint8_t PahpEN, uint8_t PwrLVL) {
    uint8_t *buf = &sTxData[1];

    //0x10F69FFF <-> 0x10E69FFF
    SwitchCFG(1);
    // Bank1.7<12>=1
    readAnalogTable(7, buf);
    if (PahpEN) {
        buf[2] |= 0x10;
    } else {

        buf[2] &= 0xEF;
    }
    SPI_Write_RegBuf((WRITE_REG | 7), 4);

    // Bank1.6<21:16>=PwrLVL
    readAnalogTable(6, buf);
    buf[1] &= 0xC0;
    buf[1] |= PwrLVL;
    SPI_Write_RegBuf((WRITE_REG | 6), 4);
    SwitchCFG(0);
}

void set_Pn9Tx(uint8_t ch) {
    int i;
    uint8_t *buf = &sTxData[1];

    SetRFChannel(ch);
_set_Pn9Tx_Str: //
    SwitchToTxMode();
    // clear int.FLAG
    delay_us(10);
    // send package 8 byte
    buf[0] = 0x0;
    buf[1] = 0x61;
    buf[2] = 0x62;
    buf[3] = 0x63;
    buf[4] = 0x64;
    buf[5] = 0x65;
    buf[6] = 0x66;
    buf[7] = 0x67;
    SPI_Write_RegBuf(W_TX_PAYLOAD_NOACK_CMD, 8);
    // wait tx int
    i = 0;
    while (1) {
        SPI_Read_Reg(FIFO_STATUS);
        delay_us(1000);
        if (SPI_Read_Reg(STATUS) & (MASK_TX_DS_FLAG | MASK_MAX_RT_FLAG)) {
            SPI_Write_Reg(WRITE_REG | STATUS, 0x70);
            break;
        }

        if (i > 10)
            goto _set_Pn9Tx_Str;
    }
    //
    delay_us(500); //Delay(100);
    SwitchCFG(1);
    // Bank1.<1>=1
    readAnalogTable(1, buf);
    buf[3] |= 0x02;
    SPI_Write_RegBuf((WRITE_REG | 1), 4);
    // Bank12.<4>=1
    readAnalogTable(12, buf);
    buf[0] |= 0x10;
    SPI_Write_RegBuf((WRITE_REG | 12), 4);
    SwitchCFG(0);
}

void read_BER(uint8_t ch) {
    uint8_t *buf = &sTxData[1];
    uint32_t berData;

    SetRFChannel(ch);
    SwitchToRxMode();
    //
    delay_us(100);
    // set RX_ADDR_P0
    if (rfStatus.rate == FSK_2G_4M) {
        buf[4] = 0x00;
        buf[3] = 0x00;
        buf[2] = 0xC3;
        buf[1] = 0xC0;
        buf[0] = 0x88;
        //
    } else if (rfStatus.rate == FSK_2G_6M) {
        buf[4] = 0x00;
        buf[3] = 0x00;
        buf[2] = 0xED;
        buf[1] = 0x76;
        buf[0] = 0x6F;
    } else {
        buf[4] = 0x02;
        buf[3] = 0x95;
        buf[2] = 0xE5;
        buf[1] = 0xDC;
        buf[0] = 0x0E;
    }
    SPI_Write_RegBuf((WRITE_REG | RX_ADDR_P0), 5);
    // set SETUP_AW
    if (rfStatus.rate == FSK_2G_4M || rfStatus.rate == FSK_2G_6M) {
        SPI_Write_Reg((WRITE_REG | SETUP_AW), 0x01);
    } else {
        SPI_Write_Reg((WRITE_REG | SETUP_AW), 0x03);
    }
    //
    SwitchCFG(1);
    // Read BER.
    while (1) {
        // read Bank1.12
        readAnalogTable(12, buf);
        // Bank1.12<27>=1
        buf[3] = 0x08;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
        delay_us(10000);
        // Bank1.12<5>=1
        buf[0] |= 0x20;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
        //delay
        delay_us(1000000);
        buf[0] |= 0x40;
        SPI_Write_RegBuf((WRITE_REG | 12), 4);
        //
        delay_us(1000000);
        SPI_Read_RegBuf(0, 4);
        memcpy(&berData, &sRxData[0], sizeof (berData));
        SPI_Read_RegBuf(1, 4);
        sprintf(uart_msg_buf, "Total number = %x, error = %x\r\n", berData, *((uint32_t*) & sRxData[0]));
        appData.stringPointer = uart_msg_buf;
        while (WriteString() == false);
        if (appData.InterruptFlag) break;
    }
    SwitchCFG(0);
}

#define MAX_P32_BIT  (256+73) // P(8)+A(40)+C(9)+L(8*32)+R(16)
#define MAX_P256_BIT  (2048+73) // P(8)+A(40)+C(9)+L(8*256)+R(16)
#define MAX_TX_LOCK_TIME  (0.004) //Sec
//#define CONST_RF_PAYLOAD_SET

void tx_packet_test(uint8_t ch) {
    uint8_t count, data;
    uint8_t max_cont = 0;
    uint16_t uR16;
    uint16_t width = 0;
    uint16_t len;
    uint8_t *p;
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
        case FSK_2G_250K:
            width = 32;
            max_cont = (int) (250000 * MAX_TX_LOCK_TIME / MAX_P32_BIT);
            break;
        case FSK_2G_1M:
            width = 32;
            max_cont = (int) (1000000 * MAX_TX_LOCK_TIME / MAX_P32_BIT);
            break;
        case FSK_2G_2M:
            width = 32;
            max_cont = (int) (2000000 * MAX_TX_LOCK_TIME / MAX_P32_BIT);
            break;
        case FSK_2G_4M:
            width = 256;
            max_cont = (int) (4000000 * MAX_TX_LOCK_TIME / MAX_P256_BIT);
            break;
        case FSK_2G_6M:
            width = 240; // ((address+payload)*8 + 2 )/3 = Integer
            max_cont = (int) (6000000 * MAX_TX_LOCK_TIME / MAX_P256_BIT);
            break;
    }

    p = rfTaskBuf;
    len = sizeof (rfTaskBuf);
    count = 0;
    while (len) {
        if (len < width)
            uR16 = len;
        else
            uR16 = width;
#ifdef CONST_RF_PAYLOAD_SET 
        sTxData[0] = WR_TX_PLOAD;
#else
        sTxData[0] = W_TX_PAYLOAD_NOACK_CMD;
#endif
        SPI_Write_Buffer(sTxData, 1, p, uR16);
        count++;
        while (1) { // check TX FIFO state
            if (SPI_Read_Reg(STATUS)&(STATUS_RX_DR | STATUS_TX_DS | STATUS_MAX_RT)) {
                SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            }
            //
            data = SPI_Read_Reg(FIFO_STATUS);
            if (data & FIFO_STATUS_TX_EMPTY) {
                count = 0;
            }
            if (max_cont > count && !(data & FIFO_STATUS_TX_FULL)) {

                break;
            }
        }
        len -= uR16;
        p += uR16;
    }
    sprintf(uart_msg_buf, " Transmitted %d Bytes\r\n", (int) sizeof (rfTaskBuf));
    appData.stringPointer = uart_msg_buf;
    while (WriteString() == false);
}

void rx_packet_test(uint8_t ch) {
    uint16_t width;
    uint8_t *p;
    int bytes = 0, count = 0;
#ifdef CONST_RF_PAYLOAD_SET  // test const payload
    SPI_Write_Reg((WRITE_REG | 1), 0x0);
    SPI_Write_Reg((WRITE_REG | 2), 0x1);
    SPI_Write_Reg((WRITE_REG | 0x11), 0x20); //32
    SPI_Write_Reg((WRITE_REG | 0x12), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x13), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x14), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x15), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x1C), 0x0);
    SPI_Write_Reg((WRITE_REG | 0x1D), 0x0);
#endif
    SetRFChannel(ch);
    SwitchToRxMode();
    while (rfStatus.rate<=FSK_2G_6M) {
        if (SPI_Read_Reg(STATUS) & STATUS_RX_DR) {
            SPI_Write_Reg((WRITE_REG | STATUS), 0x70);
            p = rfTaskBuf;
            width = SPI_Read_Reg(R_RX_PL_WID_CMD);
            if (rfStatus.rate >= FSK_2G_4M) {
                width = width * 8;
            }
            sTxData[0] = RD_RX_PLOAD;
            SPI_Read_Buffer(sTxData, 1, p, width);
            count = 1;
            bytes += width;
        }

        if (count != 0) {
            count++;
            if (count > 2000) {
                sprintf(uart_msg_buf, " Received %d Bytes\r\n", bytes);
                appData.stringPointer = uart_msg_buf;
                while (WriteString() == false);
                bytes = 0;
                count = 0;
            }
        }
        if (appData.InterruptFlag) break;
        delay_us(100);
    }
}

