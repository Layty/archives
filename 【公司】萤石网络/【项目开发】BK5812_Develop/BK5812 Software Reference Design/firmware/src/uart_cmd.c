#include "app.h"
#include "uart_cmd.h"
#include "rf_task.h"
#include "rf_config.h"
#include "rf_app.h"


void uart_rf_cmd_application(void);

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
            break;

        default:
            sprintf(uart_msg_buf, "error, tick\r\n");
            appData.stringPointer = uart_msg_buf;
            break;
    }
}

void uart_rf_cmd_application(void) {
    switch (uart_cmd_buf[3]) {
        case CMD_RF_INITIAL:
            if (uart_cmd_buf[4] == FSK_2G_2M) {
                sprintf(uart_msg_buf, "init.FSK_2G_2M\r\n");
            } else if (uart_cmd_buf[4] == FSK_2G_4M) {
                sprintf(uart_msg_buf, "init.FSK_2G_4M\r\n");
            } else if (uart_cmd_buf[4] == FSK_2G_6M) {
                sprintf(uart_msg_buf, "init.FSK_2G_6M\r\n");
            } else if (uart_cmd_buf[4] == FSK_2G_1M) {
                sprintf(uart_msg_buf, "init.FSK_2G_1M\r\n");
            } else {
                uart_cmd_buf[4] = 0;
                sprintf(uart_msg_buf, "init.FSK_2G_250K\r\n");
            }
            appData.stringPointer = uart_msg_buf;
            initFSKReg(uart_cmd_buf[4]);
            break;

        case CMD_RF_SNGWAVE:
            sprintf(uart_msg_buf, "CH%d Single Wave. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            set_SingleWave(uart_cmd_buf[4]);
            break;

        case CMD_RF_POWER:
            uart_cmd_buf[4] &= 1;
            uart_cmd_buf[4] &= 0x3f;
            sprintf(uart_msg_buf, "Set PahpEN=%d PwrLVL=%x\r\n", uart_cmd_buf[4], uart_cmd_buf[5]);
            appData.stringPointer = uart_msg_buf;
            setRFPower(uart_cmd_buf[4], uart_cmd_buf[5]);
            break;

        case CMD_RF_PN9:
            sprintf(uart_msg_buf, "Set PN9 CH%d. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            set_Pn9Tx(uart_cmd_buf[4]);
            break;

        case CMD_RF_BER:
            sprintf(uart_msg_buf, "Read BER CH%d. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            read_BER(uart_cmd_buf[4]);
            break;

        case CMD_RF_TX_PACKET:
            sprintf(uart_msg_buf, "Tx packet CH%d. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            tx_packet_test(uart_cmd_buf[4]);
            break;

        case CMD_RF_RX_PACKET:
            sprintf(uart_msg_buf, "Rx packet CH%d. \r\n", uart_cmd_buf[4]);
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            rx_packet_test(uart_cmd_buf[4]);
            break;

        case CMD_RF_SET_DEVIDE:
            if (uart_cmd_buf[5] != FSK_2G_6M) {
                uart_cmd_buf[5] = FSK_2G_4M;
            }
            if (uart_cmd_buf[4] & 1) {
                if (uart_cmd_buf[5] == FSK_2G_4M) {
                    sprintf(uart_msg_buf, "2G_4M Slave Device. \r\n");
                } else {
                    sprintf(uart_msg_buf, "2G_6M Slave Device. \r\n");
                }
            } else {
                if (uart_cmd_buf[5] == FSK_2G_4M) {
                    sprintf(uart_msg_buf, "2G_4M Master Device. \r\n");
                } else {
                    sprintf(uart_msg_buf, "2G_6M Master Device. \r\n");
                }
            }
            appData.stringPointer = uart_msg_buf;
            while (WriteString() == false);
            rf_task_init(uart_cmd_buf[4] & 1, uart_cmd_buf[5]);
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
            appData.stringPointer = uart_msg_buf;
            init_rf_data_fifo();
            rfTestTick = RF_TICK();
            rfTempData = 0;
            break;

            //            application_rf_error:
        default:
            sprintf(uart_msg_buf, "no RF app!\r\n");
            appData.stringPointer = uart_msg_buf;
            break;
    }
}

