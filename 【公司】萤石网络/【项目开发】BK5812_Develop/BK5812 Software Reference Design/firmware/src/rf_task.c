#include "app.h"
#include "rf_task.h"
#include "rf_app.h"
#include "rf_config.h"
#include "uart_cmd.h"

uint32_t sendDataLen = 0; // uart command RF Test Data length
uint32_t rfTempData = 0; // RF test Data 0=reStart 0xffffffff=End
uint32_t rfGetData = 0; // RF test read new Data value
uint32_t rfGetError = 0; // RF test error count

uint64_t rfTestTick = 0;

/*************************************************************

 rf task

 ***************************************************************/
void rf_task_init(uint8_t type, uint8_t rate) {
    // init.RF
    initFSKReg(rate);
    // init.rfStatus
    rfStatus.device = type;
    rfStatus.rate = rate;
    rfStatus.ch = 0;
    rfStatus.mode = INSTANDBY;
    rfStatus.tick = 0;
    // clear FIOF
    init_rf_data_fifo();
    rfTempData = 0;
    sendDataLen = 0;
}



void rf_task(void) {
    uint16_t i, j;
    static uint16_t initDelay;

    if (rfStatus.mode == LINKED) {
        rf_application();
    } else if (rfStatus.mode == UNLINK) {
        rf_device_serch();
    } else {
        return;
    }
    // uart command send test
    if (rfStatus.device == RF_MASTER) { // MASTER
        if (sendDataLen) {
            i = get_rf_data_fifo_space();
            if (i >= (sizeof (rfTempData) * 4)) {
                //RF_IO_TRIGGER_A;
                write_rf_data_fifo((uint8_t*) & rfTempData, sizeof (rfTempData));
                rfTempData++;
                write_rf_data_fifo((uint8_t*) & rfTempData, sizeof (rfTempData));
                rfTempData++;
                write_rf_data_fifo((uint8_t*) & rfTempData, sizeof (rfTempData));
                rfTempData++;
                write_rf_data_fifo((uint8_t*) & rfTempData, sizeof (rfTempData));
                rfTempData++;
                sendDataLen -= sizeof (rfTempData) * 4;
                //RF_IO_TRIGGER_A;
            }
            if (sendDataLen == 0) {
                rfTempData = 0xffffffff;
                write_rf_data_fifo((uint8_t*) & rfTempData, sizeof (rfTempData));
                //RF_IO_TRIGGER_A;
                rfTestTick = RF_TICK() - rfTestTick;
                rfTestTick /= (0.001 / 0.0000032); // tick =3.2uS
                sprintf(uart_msg_buf, "Send %dmSec.\r\n", (int)rfTestTick&0xffffffff);
                appData.stringPointer = uart_msg_buf;
                //RF_IO_TRIGGER_A;
            }
        }
    } else { // SLAVE
        if (rfGetData == 0xffffffff) {
            if (--initDelay == 0) {
                rfGetData = 0;
                //RF_IO_TRIGGER_A;
                init_rf_data_fifo();
                //RF_IO_TRIGGER_A;
            }
        } else {
            i = get_rf_data_fifo_length();
            j = 32;
            while (i >= sizeof (rfGetData)) {
                read_rf_data_fifo((uint8_t*) & rfGetData, sizeof (rfGetData));
                if (rfGetData == 0) {
                    //RF_IO_TRIGGER_A;
                    rfTempData = 0;
                    rfGetError = 0;
                    sendDataLen = sizeof (rfGetData);
                    //RF_IO_TRIGGER_A;
                } else if (rfGetData == 0xffffffff) {
                    //RF_IO_TRIGGER_A;
                    sprintf(uart_msg_buf, "Read %d Bytes, %d Err.\r\n", sendDataLen, rfGetError);
                    appData.stringPointer = uart_msg_buf;
                    initDelay = 5000;
                    i = 0;
                    break;
                    //RF_IO_TRIGGER_A;
                } else {
                    sendDataLen += sizeof (rfGetData);
                    rfTempData++;
                    if (rfGetData != rfTempData) {
                        RF_IO_TRIGGER_A;
                        rfGetError++;
                        RF_IO_TRIGGER_A;
                    }
                    rfTempData = rfGetData;
                }
                i -= sizeof (rfGetData);
                if (--j == 0)
                    break;
            }
        }
    }
}

