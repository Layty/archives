/* 
 * File:   uart_cmd.h
 * Author: Administrator
 *
 * Created on 2016?12?8?, ?? 10:33
 */

#ifndef UART_CMD_H
#define	UART_CMD_H

#define CMD_LEN 32

enum {
    CMD_TIMER = 0x10,
    CMD_ADC = 0x11,
    CMD_AES = 0x12,
    CMD_PWM = 0x13,
    CMD_SLEEP = 0x14,
    CMD_IDLE = 0x15,
    CMD_GPIO_OUT = 0x16,
    CMD_GPIO_IN = 0x17,
    CMD_PCA = 0x18,
    CMD_WDT = 0x19,
    CMD_SPI_M = 0x20,
    CMD_SPI_S = 0x21,
    CMD_RNG = 0x22,
    CMD_LBD = 0x23,
    CMD_RSSI = 0x24,
    CMD_RF = 0x30,
    CMD_FLASH = 0x31,
    CMD_UART = 0x32,

    CMD_START = 0x55,
    CMD_END = 0x7e,
};

/*
RF_INIT_250K    1   55 30 02 01 00 7E
RF_INIT_1M  1       55 30 02 01 01 7E
RF_INIT_2M  1       55 30 02 01 02 7E
RF_INIT_4M  1       55 30 02 01 03 7E
RF_INIT_6M  1       55 30 02 01 04 7E
RF_SW_ch0   1       55 30 02 02 00 7E
RF_SW_ch40  1       55 30 02 02 28 7E
RF_SW_ch80  1       55 30 02 02 50 7E
RF_txPN9_ch0    1   55 30 02 04 00 7E
RF_txPN9_ch40   1   55 30 02 04 28 7E
RF_txPN9_ch80   1   55 30 02 04 50 7E
RF_BER_ch0  1       55 30 02 03 00 7E
RF_BER_ch40 1       55 30 02 03 28 7E
RF_BER_ch80 1       55 30 02 03 50 7E
RF_PTX_ch0  1       55 30 02 0A 00 7E
RF_PTX_ch40 1       55 30 02 0A 28 7E
RF_PTX_ch80 1       55 30 02 0A 50 7E
RF_PRX_ch0  1       55 30 02 0B 00 7E
RF_PRX_ch40 1       55 30 02 0B 28 7E
RF_PRX_ch80 1       55 30 02 0B 50 7E

RF_DEVICE_MASTER_4M 1   55 30 03 14 00 03 7E
RF_DEVICE_SLAVE_4M  1   55 30 03 14 01 03 7E
RF_DEVICE_MASTER_6M 1   55 30 03 14 00 04 7E
RF_DEVICE_SLAVE_6M  1   55 30 03 14 01 04 7E

SEND_RF_TEST    1       55 30 05 15 00 00 10 00 7E
CLEAR_RF_FIFO   1       55 30 05 15 00 00 00 00 7E
RF_OUTPUT_12DBM 1       55 30 03 0C 01 37 7E
 */
enum {
    CMD_RF_INITIAL = 1,
    CMD_RF_SNGWAVE = 2,
    CMD_RF_BER = 3,
    CMD_RF_PN9 = 4,
    //
    CMD_RF_TX_PACKET = 10,
    CMD_RF_RX_PACKET = 11,
    CMD_RF_POWER = 12,
    // rf task control
    CMD_RF_SET_DEVIDE = 20,
    CMD_RF_SEND_TEST = 21,
};

char uart_cmd_buf[16];
char uart_msg_buf[48];

char B_CmdOK; // flage of get uart command    

void uart_get_command(char data);
void uart_command_process(void);

#endif	/* UART_CMD_H */

