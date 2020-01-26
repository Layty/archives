/* 
 * File:   bk_config.h
 * Author: Administrator
 *
 * Created on 2016?12?8?, ?? 9:39
 */

#ifndef BK_CONFIG_H
#define	BK_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    //************************FSK COMMAND and REGISTER****************************************//
    // SPI(BK5811) commands
#define READ_REG        0x00  // Define read command to register
#define WRITE_REG       0x20  // Define write command to register
#define RD_RX_PLOAD     0x61  // Define RX payload register address
#define WR_TX_PLOAD     0xA0  // Define TX payload register address
#define FLUSH_TX        0xE1  // Define flush TX register command
#define FLUSH_RX        0xE2  // Define flush RX register command
#define REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define W_TX_PAYLOAD_NOACK_CMD  0xb0
#define W_ACK_PAYLOAD_CMD   0xa8
#define ACTIVATE_CMD        0x50
#define R_RX_PL_WID_CMD     0x60
#define NOP             0xFF  // Define No Operation, might be used to read status register
    // SPI(BK5811) registers(addresses)
#define CONFIG          0x00  // 'Config' register address
#define EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define SETUP_AW        0x03  // 'Setup address width' register address
#define SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH           0x05  // 'RF channel' register address
#define RF_SETUP        0x06  // 'RF setup' register address
#define STATUS          0x07  // 'Status' register address
#define OBSERVE_TX      0x08  // 'Observe TX' register address
#define CD              0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define TX_ADDR         0x10  // 'TX address' register address
#define RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define PAYLOAD_WIDTH   0x1f  // 'payload length of 256 bytes modes register address
    //interrupt status
#define STATUS_RX_DR 0x40
#define STATUS_TX_DS 0x20
#define STATUS_MAX_RT 0x10
#define MASK_IRQ_FLAGS      0x70

#define MASK_MAX_RT_FLAG    0x10
#define MASK_RX_DR_FLAG     0x40
#define MASK_TX_DS_FLAG     0x20

#define RX_P_NO             0x0e

#define STATUS_TX_FULL 0x01

    //FIFO_STATUS
#define FIFO_STATUS_TX_REUSE 0x40
#define FIFO_STATUS_TX_FULL 0x20
#define FIFO_STATUS_TX_EMPTY 0x10

#define FIFO_STATUS_RX_FULL 0x02
#define FIFO_STATUS_RX_EMPTY 0x01

#define RF_IO_TRIGGER_A BSP_LEDToggle(BSP_LED_5)
#define RF_IO_TRIGGER_B BSP_LEDToggle(BSP_LED_6)


#define CE_LOW() BSP_LEDOff(BSP_LED_3)
#define CE_HIGH() BSP_LEDOn(BSP_LED_3)
#define CSN_LOW() BSP_LEDOff(BSP_LED_4)
#define CSN_HIGH() BSP_LEDOn(BSP_LED_4)    

#define IRQ_GET() PLIB_PORTS_PinGet(PORTS_ID_0, PORT_CHANNEL_E, 8) //BSP_SwitchStateGet(BSP_SWITCH_3)
#define IRQ_TICK() appData.irqTick
#define IRQ_TICK_CLR() appData.irqTick=0
#define RF_TICK() (appData.rfTick*32768+DRV_TMR0_CounterValueGet())

    typedef enum {
        FSK_2G_250K = 0, //
        FSK_2G_1M, //
        FSK_2G_2M, //
        FSK_2G_4M, //
        FSK_2G_6M, //
    } _fsk_type;

    typedef enum {
        RF_MASTER, //
        RF_SLAVE, //
    } _rf_device;

    typedef struct _rf_cfg_s {
        uint8_t device;
        uint8_t mode;
        uint8_t rate;
        uint8_t ch;
        uint8_t bank;   
        //
        uint8_t pktInfoMax;
        uint8_t pktAckByte;
        uint16_t pktTickMax;
        //
        uint64_t tick;
    } _rf_cfg_t;

    extern _rf_cfg_t rfStatus;

    uint8_t sTxData[256];
    uint8_t sRxData[sizeof (sTxData)];

    void SPI_Write_Reg(uint8_t reg, uint8_t data);
    uint8_t SPI_Read_Reg(uint8_t reg);
    void SPI_Write_RegBuf(uint8_t reg, uint8_t bytes);
    void SPI_Write_Buffer(uint8_t* cmd, uint8_t len, uint8_t *pBuf, uint16_t bytes);
    void SPI_Read_Buffer(uint8_t* cmd, uint8_t len, uint8_t *pBuf, uint16_t bytes);

    void SwitchToRxMode(void);
    void SwitchToTxMode(void);
    void SetAirRate(uint8_t rate);
    void SetRFChannel(uint8_t ch);
    void setRFPower(uint8_t PahpEN, uint8_t PwrLVL);
    void initFSKReg(uint8_t fskMode);
    
    void set_SingleWave(uint8_t ch);
    void set_Pn9Tx(uint8_t ch);
    void read_BER(uint8_t ch);
    void tx_packet_test(uint8_t ch);
    void rx_packet_test(uint8_t ch);

#ifdef	__cplusplus
}
#endif

#endif	/* BK_CONFIG_H */

