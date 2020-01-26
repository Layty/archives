/*
 * serial.h
 *
 *  Created on: 2015年12月10日
 *      Author: Admin
 */

#ifndef SRC_SERIAL_H_
#define SRC_SERIAL_H_

#include "include.h"

#define CONFIG_RX_TMO_LIMIT 8
#define CONFIG_TX_TMO_LIMIT 20

#define CONFIG_SERIAL_BUFF_SIZE 32
#define serial_lowlevel_init()      do{\
                                        UART1_BRR2=0x1b;\
                                        UART1_BRR1=0xa0;\
                                        UART1_CR1=0;\
                                        UART1_CR1_M=0;          /*1起始位，8位数据*/\
                                        UART1_CR3=0;            /*一个停止位*/\
                                        UART1_CR2_TEN=0;        /*禁用发送*/\
                                        UART1_CR2_RIEN=1;       /*使能接收中断*/\
                                        UART1_CR2_TIEN=0;       /*禁用发送中断*/\
                                        UART1_CR2_REN=1;        /*使能接收*/\
                                        UART1_SR=0;             /*清空状态寄存器*/\
                                        CLK_PCKENR1|=0x4;       /*使能UART外设时钟*/\
                                    }while(0)
#define serial_tx_start()   {UART1_CR2_TEN=1;}
#define serial_tx_stop()    {UART1_CR2_TEN=0;}
#define serial_rx_start()   {UART1_CR2_REN=1;}
#define serial_rx_stop()    {UART1_CR2_REN=0;}
                                        
void serial_init();
u8 serial_send(u8 *dat,u8 len);
u8 serial_receive(u8 *buff,u8 len);
void serial_callback();
#endif /* SRC_SERIAL_H_ */
