#include "app.h"
#include "uart_cmd.h"
#include "rf_task.h"
#include "rf_config.h"
#include "rf_app.h"
#include "hik/hik_test.h"

void uart_rf_cmd_application(void);
void uart_hik_cmd_application(void);
void rf_channel_set_table(uint8_t* ch_tbl);

/*************************************************************
 0x55(head)
 + cmd_module(specifies which module will be tested,1byte)
 + cmdlen(length of command (cmd_test_item+cmd_parameters),1byte)
 + cmd_test_item(1byte)
 + cmd_parameters(cmdlen-1 bytes)
 + 0x7e(end)
 ***************************************************************/
void uart_get_command(char data) {
    static uint8_t N_CmdLen = 0;
    static uint8_t bCmdSt = false;
    static uint8_t bCmdEd = true;
    int i;
    _nop();
    _nop();
    if ((CMD_START == data) && bCmdEd) {
        for (i = 0; i < N_CmdLen; i++) {
            uart_cmd_buf[i] = 0;
        }
        bCmdSt = true;
        bCmdEd = false;
        N_CmdLen = 0;
    }

    if (bCmdSt == true) {
        uart_cmd_buf[N_CmdLen] = data;
        if (++N_CmdLen >= sizeof (uart_cmd_buf)) {
            N_CmdLen = sizeof (uart_cmd_buf) - 1;
        }
    }

    if (CMD_END == data) {
        if (bCmdSt == true && N_CmdLen == (uart_cmd_buf[2] + 4)) {
            bCmdSt = false;
            bCmdEd = true;
            B_CmdOK = true;
        }
    }
}

void uart_command_process(void) {

    switch (uart_cmd_buf[1]) {
        case CMD_RF:
            uart_rf_cmd_application();
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            break;
            
        case CMD_HIK:
            uart_hik_cmd_application();
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            break;            
        default:
            sprintf(uart_msg_buf, "CMD error!!\r\n");
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            break;
    }
}

void uart_rf_cmd_application(void) {
    uint8_t u8i;
    extern uint32_t fd_read;
    switch (uart_cmd_buf[3]) {
        case CMD_RF_INITIAL:
            if (uart_cmd_buf[4] == FSK_5G_250K) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_5G_250K fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_5G_1M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_5G_1M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_5G_2M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_5G_2M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_5G_4M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_5G_4M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_5G_6M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_5G_6M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_2G_1M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_2G_1M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_2G_2M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_2G_2M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_2G_4M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_2G_4M fd_read=%0x\r\n", fd_read);
            } else if (uart_cmd_buf[4] == FSK_2G_6M) {
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_2G_6M fd_read=%0x\r\n", fd_read);
            } else {
                uart_cmd_buf[4] = FSK_2G_250K;
                initFSKReg(uart_cmd_buf[4]);
                sprintf(uart_msg_buf, "init.FSK_2G_250K fd_read=%0x\r\n", fd_read);
            }
            rf_task_init(RF_MASTER, uart_cmd_buf[4]);
            break;

        case CMD_RF_SNGWAVE:
            set_SingleWave(uart_cmd_buf[4]);
            sprintf(uart_msg_buf, "CH%d Single Wave. \r\n", uart_cmd_buf[4]);
            break;

        case CMD_RF_POWER:
            if (rfStatus.rate >= FSK_5G_250K) {
                uart_cmd_buf[4] &= 0x0F;
                setRFPower(uart_cmd_buf[4], 0);
                sprintf(uart_msg_buf, "Set Pwrm1v=%d\r\n", uart_cmd_buf[4]);
            } else {
                uart_cmd_buf[4] &= 1;
                uart_cmd_buf[5] &= 0x3f;
                setRFPower(uart_cmd_buf[4], uart_cmd_buf[5]);
                sprintf(uart_msg_buf, "Set PahpEN=%d PwrLVL=%x\r\n", uart_cmd_buf[4], uart_cmd_buf[5]);
                printf("CMD_RF_POWER\r\n");
            }
            break;

        case CMD_RF_CH_TABLE:
            rf_channel_set_table((uint8_t*) & uart_cmd_buf[4]);
            sprintf(uart_msg_buf, "Set ch table={%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d}\r\n",
                    uart_cmd_buf[4], uart_cmd_buf[5], uart_cmd_buf[6], uart_cmd_buf[7],
                    uart_cmd_buf[8], uart_cmd_buf[9], uart_cmd_buf[10], uart_cmd_buf[11],
                    uart_cmd_buf[12], uart_cmd_buf[13], uart_cmd_buf[14], uart_cmd_buf[15],
                    uart_cmd_buf[16], uart_cmd_buf[17], uart_cmd_buf[18], uart_cmd_buf[19]);
            break;

        case CMD_RF_PN9:
            set_Pn9Tx(uart_cmd_buf[4]);
            sprintf(uart_msg_buf, "Set PN9 CH%d.\r\n", uart_cmd_buf[4]);
            break;

        case CMD_RF_BER:
            sprintf(uart_msg_buf, "Read BER CH%d. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            read_BER(uart_cmd_buf[4]);
            sprintf(uart_msg_buf, " ");
            break;

        case CMD_RF_REG_READ:
            u8i = uart_cmd_buf[5];
            switch (uart_cmd_buf[4]) {
                case 0:
                    if (u8i > 0x1D) goto CMD_RF_REG_READ_FAIL;
                    SwitchCFG(0);
                    uart_cmd_buf[6] = SPI_Read_Reg(u8i);
                    sprintf(uart_msg_buf, "Read Bank0 Reg%d=0x%02X\r\n", u8i, (uint8_t) uart_cmd_buf[6]);
                    break;
                case 1:
                    if (u8i > 13) goto CMD_RF_REG_READ_FAIL;
                    sprintf(uart_msg_buf, "Read Bank1 Reg%d=0x%08X\r\n", u8i, Bank1_Reg0_13[u8i]);
                    break;
CMD_RF_REG_READ_FAIL:
                default:
                    sprintf(uart_msg_buf, "RF CMD %d Fail\r\n", uart_cmd_buf[3]);
                    break;
            }
            break;

        case CMD_RF_REG_WRITE:
            u8i = uart_cmd_buf[5];
            switch (uart_cmd_buf[4]) {
                case 0:
                    if (u8i > 0x1D) goto CMD_RF_REG_WRITE_FAIL;
                    SwitchCFG(0);
                    SPI_Write_Reg((WRITE_REG | u8i), uart_cmd_buf[6]);
                    sprintf(uart_msg_buf, "Write Bank0 Reg%d=0x%02X\r\n", u8i, uart_cmd_buf[6]);
                    break;
                case 1:
                    if (u8i > 13) goto CMD_RF_REG_WRITE_FAIL;
                    memcpy(&Bank1_Reg0_13[u8i], & uart_cmd_buf[6], sizeof (uint32_t));
                    SwitchCFG(1);
                    //writeAnalogTable((uint8_t) u8i, (uint8_t*) & uart_cmd_buf[6]);
                    readAnalogTable(u8i, (uint8_t*) & uart_cmd_buf[6]);
                    SPI_Write_RegBuf((WRITE_REG | u8i), 4);
                    sprintf(uart_msg_buf, "Write Bank1 Reg%d=0x%08X\r\n", u8i, Bank1_Reg0_13[u8i]);
                    SwitchCFG(0);
                    break;
CMD_RF_REG_WRITE_FAIL:
                default:
                    sprintf(uart_msg_buf, "RF CMD %d Fail\r\n", uart_cmd_buf[3]);
                    break;
            }
            break;

        case CMD_RF_TX_PACKET:
            tx_packet_test(uart_cmd_buf[4]);
            sprintf(uart_msg_buf, "Tx packet CH%d. \r\n", uart_cmd_buf[4]);
            break;

        case CMD_RF_RX_PACKET:
            sprintf(uart_msg_buf, "Rx packet CH%d. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            rx_packet_test(uart_cmd_buf[4]);
            sprintf(uart_msg_buf, " ");
            break;

        case CMD_RF_SET_DEVIDE:
            if (uart_cmd_buf[4] == 1) {
                if (uart_cmd_buf[5] == FSK_2G_4M) {
                    sprintf(uart_msg_buf, "2G_4M Slave Device. \r\n");
                } else if (uart_cmd_buf[5] == FSK_2G_6M) {
                    sprintf(uart_msg_buf, "2G_6M Slave Device. \r\n");
                } else if (uart_cmd_buf[5] == FSK_5G_4M) {
                    sprintf(uart_msg_buf, "52G_4M Slave Device. \r\n");
                } else if (uart_cmd_buf[5] == FSK_5G_6M) {
                    sprintf(uart_msg_buf, "5G_6M Slave Device. \r\n");
                } else {
                    sprintf(uart_msg_buf, " Error Device. \r\n");
                    break;
                }
            } else {
                uart_cmd_buf[4] = 0;
                if (uart_cmd_buf[5] == FSK_2G_4M) {
                    sprintf(uart_msg_buf, "2G_4M Master Device. \r\n");
                } else if (uart_cmd_buf[5] == FSK_2G_6M) {
                    sprintf(uart_msg_buf, "2G_6M Master Device. \r\n");
                } else if (uart_cmd_buf[5] == FSK_5G_4M) {
                    sprintf(uart_msg_buf, "5G_4M Master Device. \r\n");
                } else if (uart_cmd_buf[5] == FSK_5G_6M) {
                    sprintf(uart_msg_buf, "5G_6M Master Device. \r\n");
                } else {
                    sprintf(uart_msg_buf, " Error Device. \r\n");
                    break;
                }
            }
            // init.RF
            if (rfStatus.rate != uart_cmd_buf[5]) {
                initFSKReg(uart_cmd_buf[5]);
            }
            rf_task_init(uart_cmd_buf[4], uart_cmd_buf[5]);
            rfStatus.mode = UNLINK;
            break;

        case CMD_RF_SEND_TEST:
            uart_cmd_buf[4] &= 0xFE;
            memcpy(&sendDataLen, &uart_cmd_buf[4], sizeof (sendDataLen));
            if (sendDataLen == 0) {
                sprintf(uart_msg_buf, "Clear RF FIFO. \r\n");
            } else {
                sprintf(uart_msg_buf, "Send %d bytes. \r\n", sendDataLen);
            }
            init_rf_data_fifo();
            rfTestTick = RF_TICK();
            rfTempData = 0;
            break;

            //            application_rf_error:
        default:
            sprintf(uart_msg_buf, "RF CMD Fail!\r\n");
            break;
    }
}


void uart_hik_cmd_application(void) {
    uint8_t* buf = &sTxData[1];
    int i;
    uint8_t addr_lsb = 0;
    uint8_t command = uart_cmd_buf[3];
    uint8_t pipe_number;
    char* addr;
    uint8_t mode = 0;
            
    switch (command) {
        case CMD_HIK_SET_TX_ADDR:
            addr = &uart_cmd_buf[4];
            memcpy(buf, addr, 5);
            SPI_Write_RegBuf((WRITE_REG | 16), 5);
            memcpy(buf, addr, 5);  
            SPI_Write_RegBuf((WRITE_REG | 10), 5);            
            //Hik_initFSKReg(uart_cmd_buf);            
            if(0){
            rf_task_init(RF_MASTER, FSK_2G_4M);
            set_SingleWave(0);
            }
         
            break;
        case CMD_HIK_SET_RX_ADDR:
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

            //Hik_initFSKReg(uart_cmd_buf);            
            if(0){
            rf_task_init(RF_MASTER, FSK_2G_4M);
            set_SingleWave(0);
            }
            //sprintf(uart_msg_buf, "CH%d Single Wave. \r\n", uart_cmd_buf[4]);    
            break;
        case CMD_HIK_SHOW_ADDR:
            for(i =10; i<12; i++){
                SPI_Read_RegBuf(i, 5);
                printf("REG[%d] = %02x:%02x:%02x:%02x:%02x\r\n",i,sRxData[0],sRxData[1],sRxData[2],sRxData[3],sRxData[4]);
            }
            for(i = 12;i < 16; i++){
                SPI_Read_RegBuf(i, 1);
                addr_lsb = sRxData[0];
                SPI_Read_RegBuf(11, 5);
                printf("REG[%d] = %02x:%02x:%02x:%02x:%02x\r\n",i,addr_lsb,sRxData[1],sRxData[2],sRxData[3],sRxData[4]);
            }
            SPI_Read_RegBuf(16, 5);
            printf("REG[%d] = %02x:%02x:%02x:%02x:%02x\r\n",i,sRxData[0],sRxData[1],sRxData[2],sRxData[3],sRxData[4]);
            printf("REG[%d] = %02x\r\n", SETUP_RETR, SPI_Read_Reg(SETUP_RETR));
            
            break;
#if 1            
        case CMD_HIK_SEND_TXACK:
            mode = uart_cmd_buf[4];
            hik_test_ack(mode);
            break;
#endif            
        case CMD_HIK_DEBUG:
/*
            SPI_Read_RegBuf(i, 1);
            printf("REG[%d] = %02x\r\n",i,sRxData[0]);
*/
            break;
 #if 1           
        case CMD_HIK_TX_LIMIT:
            hik_tx_packet_limit(uart_cmd_buf[4], uart_cmd_buf[5], uart_cmd_buf[6] );
            break;
#endif         
        case CMD_HIK_TX_LIMIT_ACK:
            hik_tx_packet_limit_ack(uart_cmd_buf[4], uart_cmd_buf[5], uart_cmd_buf[6] );
            break;            
        default:
            sprintf(uart_msg_buf, "RF CMD Fail!\r\n");
            break;
    }    
    
}