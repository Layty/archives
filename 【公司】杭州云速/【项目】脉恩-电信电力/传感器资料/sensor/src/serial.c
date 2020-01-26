/*
 * serial.c
 *
 *  Created on: 2015Äê12ÔÂ10ÈÕ
 *      Author: Admin
 */
#include <string.h>
#include "serial.h"

static u8 rx_buff[CONFIG_SERIAL_BUFF_SIZE];
static u8 rx_index=0;
static u8 rx_tmo=0xff,tx_tmo=0xff;

void serial_init()
{
    memset(rx_buff,0,CONFIG_SERIAL_BUFF_SIZE);
    serial_lowlevel_init();
}
void serial_callback()
{
    if(rx_tmo>0)
        rx_tmo--;
    if(tx_tmo>0)
        tx_tmo--;
}
u8 serial_send(u8 *dat,u8 len)
{
    u8 ret=0;
    serial_tx_start();
    for(u8 i=0;i<len;i++)
    {
        tx_tmo=CONFIG_TX_TMO_LIMIT;
//        while(tx_tmo>0);
        while(!UART1_SR_TXE)
        {
            if(tx_tmo<=0)goto end;
        }
        UART1_DR=*dat++;
        while(!UART1_SR_TC)
        {
            if(tx_tmo<=0)goto end;
        }
        ret++;
    }
end:

    serial_tx_stop();
    return ret;
}
u8 serial_receive(u8 *buff,u8 len)
{
    u8 ret;
    while(rx_tmo>0)
    {
//        if(len<=rx_index)
//        {
//            break;
//        }
    }
    ret=rx_index;
    if(ret>0)
    {
        serial_rx_stop();
        memcpy(buff,rx_buff,ret);
        rx_index=0;
        serial_rx_start();
    }
    return ret;
}


#pragma vector=UART1_R_RXNE_vector
__interrupt void UART1_R_RXNE_Handler()
{
    u8 sr=UART1_SR;
    if(sr&0x28)
    {
        rx_buff[rx_index++]=UART1_DR;
        rx_tmo=CONFIG_RX_TMO_LIMIT;
        if(rx_index>=CONFIG_SERIAL_BUFF_SIZE)rx_index=0;
    }
}
